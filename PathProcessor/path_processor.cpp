#include "path_processor.h"
#include "FeatureDetector/fast_feature_detector.h"
#include "VideoReader/open_cv_video_reader.h"

#include <thread>


ModulePathProcessor::PathProcessor::PathProcessor(IAbstractFeatureTracker *feature_tracker,
                                                  IAbstractFeatureDetector *feature_detector,
                                                  IAbstractVideoReader *video_reader,
                                                  IAbstractPoseEstimator *pose_estimator,
                                                  PathDrawer *path_drawer,
                                                  Statistics *stat)
{
  m_feature_tracker = feature_tracker;
  m_feature_detector = feature_detector;
  m_video_reader = video_reader;
  m_pose_estimator = pose_estimator;

  m_path_drawer = path_drawer;
  m_stat = stat;
}


void ModulePathProcessor::PathProcessor::set_logger(AbstractLogger *logger)
{
  m_logger = logger;
  m_path_drawer->set_logger(logger);
}


/* frame is equal to current, need it to not block mutex */
void ModulePathProcessor::PathProcessor::draw_trace(Mat &frame, const vector<Point2f> &src_keypoints, const vector<Point2f> &dst_keypoints)
{
  const auto curr_size = dst_keypoints.size();
  const Scalar red(0, 0, 255);
  const Scalar green(0, 255, 0);

  if (curr_size == src_keypoints.size())
  {
    for(size_t idx = 0; idx < curr_size; idx++)
    {
      line(frame, dst_keypoints[idx], src_keypoints[idx], green, 2, LINE_AA);
    }
  }

  for_each(src_keypoints.begin(), src_keypoints.end(),
           [frame](const Point2f& keypoint){ circle(frame, keypoint, 3, Scalar(0, 0, 255), -1, FILLED | LINE_AA); });
}


ModulePathProcessor::PathProcessor::~PathProcessor()
{
  if (m_feature_tracker != nullptr)
  {
    delete m_feature_tracker;
  }
  delete m_feature_detector;
  delete m_video_reader;
  delete m_pose_estimator;
}


Mat ModulePathProcessor::PathProcessor::get_curr_frame()
{
  Mat img_copy;

  m_img_mutex.lock();
  if (m_img[m_current_switch].size != 0)
  {
    img_copy = m_img[m_current_switch].clone();
  }
  m_img_mutex.unlock();

  return img_copy;
}


bool ModulePathProcessor::PathProcessor::is_started()
{
  return m_is_running;
}


bool ModulePathProcessor::PathProcessor::is_stopped()
{
  return m_is_stopped;
}


void ModulePathProcessor::PathProcessor::start(submodule_type running_type)
{
  m_running_type = running_type;
  m_is_running = true;

  if (m_path_drawer)
  {
    m_path_drawer->reset();
  }

  if (m_stat)
  {
    m_stat->m_general_stat.values.clear();
  }

  std::thread thread(&PathProcessor::thread_running, this, 1);
  thread.detach();
}


void ModulePathProcessor::PathProcessor::step()
{
  m_running_mutex.lock();
  m_is_running = true;
  m_running_mutex.unlock();

  m_cond_variable.notify_one();
}


void ModulePathProcessor::PathProcessor::pause()
{
  m_running_mutex.lock();
  m_is_running = false;
  m_running_mutex.unlock();
}


void ModulePathProcessor::PathProcessor::continue_running()
{
  m_running_mutex.lock();
  m_is_running = true;
  m_running_mutex.unlock();

  m_cond_variable.notify_one();
}

void ModulePathProcessor::PathProcessor::stop()
{
  m_is_stopped = true;
  m_is_running = false;

  m_running_mutex.lock();
  if (m_video_reader)
  {
    m_video_reader->stop();
  }
  m_running_mutex.unlock();
}


void ModulePathProcessor::PathProcessor::thread_running(uint8_t job_count)
{
  (void)job_count;

  m_running_mutex.lock();
  m_is_running = true;
  m_is_stopped = false;
  m_running_mutex.unlock();

  mutex running_lock;

  std::unique_lock lock(running_lock);

  switch (m_running_type)
  {
    case submodule_type::CONTINUOUSLY:
      while (!m_is_stopped && m_video_reader && !m_video_reader->is_finished())
      {
        if (!m_is_running)
        {
          m_cond_variable.wait(lock);
        }

        process_frame(job_count);
      }
      break;

    case submodule_type::STEP_BY_STEP:
      while (!m_is_stopped && !m_video_reader->is_finished())
      {
        if (!m_is_running)
        {
          m_cond_variable.wait(lock);
        }

        process_frame(job_count);

        m_running_mutex.lock();
        m_is_running = false;
        m_running_mutex.unlock();
      }
      break;

    default:
      break;
    }

  lock.release();

  m_running_mutex.lock();
  m_is_running = false;
  m_is_stopped = true;
  m_running_mutex.unlock();
}


void ModulePathProcessor::PathProcessor::process_frame(uint8_t job_count)
{
  Mat R = Mat::eye(3, 3, CV_64F), t = Mat::zeros(3, 1, CV_64F);
  (void) job_count;

  if (!m_video_reader->is_finished())
  {
    auto frame = m_video_reader->read_next_frame();
    vector<Point2f> src_keypoints_copy;
    vector<Point2f> dst_keypoints_copy;

    LOG_INFO(m_logger, "Reading video frame: " + std::to_string(m_video_reader->get_current_frame_num()));


    const clock_t begin_time = clock();

    m_keypoints[m_current_switch].clear();
    m_feature_detector->detect_features(frame, m_keypoints[m_current_switch]);
    LOG_INFO(m_logger, "Detecting features: " + std::to_string(m_keypoints[m_current_switch].size()));

    src_keypoints_copy = m_keypoints[!m_current_switch]; // always previous
    dst_keypoints_copy = m_keypoints[m_current_switch];  // always current

    /* If haven't found any keypoints, raise error */
    if (m_keypoints[!m_current_switch].size() > 0)
    {
      do
      {
        if (m_feature_tracker->get_type() == FEATURE_TRACKER_WITH_FRAME)
        {
          if (!m_feature_tracker->track_features(m_img[!m_current_switch], frame, src_keypoints_copy, dst_keypoints_copy))
          {
            break;
          }
          LOG_INFO(m_logger, "Detecting features: " + std::to_string(m_keypoints[m_current_switch].size()));
        }
        else
        {
          if (!m_feature_tracker->track_features(src_keypoints_copy, dst_keypoints_copy))
          {
            break;
          }
        }


        if (src_keypoints_copy.size() > 0 && dst_keypoints_copy.size() > 0)
        {
          m_pose_estimator->find_matrix(src_keypoints_copy, dst_keypoints_copy, R, t);
        }

        if (m_stat)
        {
          m_stat->m_general_stat.values.push_back(double(clock() - begin_time) / CLOCKS_PER_SEC);
        }

        /* Call it here to block mutex for the least possible time */
        draw_trace(frame, src_keypoints_copy, dst_keypoints_copy);

        std::stringstream ss;

        ss << "Translation x= " << t.at<double>(0)
           <<", y = " << t.at<double>(1)
           << ", z = " << t.at<double>(2);

        LOG_INFO(m_logger, ss.str());

        m_path_drawer->update_pos(R, t);
      }while(0);
    }
    else
    {
      /* IDK how to correctly handle it, do nothing */
    }

    m_img_mutex.lock();
    m_img[m_current_switch] = frame;
    m_current_switch = !m_current_switch;
    m_img_mutex.unlock();
  }
}
