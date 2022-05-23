#include "path_processor.h"
#include "FeatureDetector/fast_feature_detector.h"
#include "VideoReader/open_cv_video_reader.h"

#include <thread>


module_path_processor::PathProcessor::PathProcessor(abstract_feature_tracker *feature_tracker,
                                                    abstract_feature_detector *feature_detector,
                                                    abstract_video_reader *video_reader,
                                                    abstract_pose_estimator *pose_estimator,
                                                    PathDrawer *path_drawer)
{
  m_feature_tracker = feature_tracker;
  m_feature_detector = feature_detector;
  m_video_reader = video_reader;
  m_pose_estimator = pose_estimator;

  m_path_drawer = path_drawer;
}


void module_path_processor::PathProcessor::set_logger(abstract_logger *logger)
{
  m_logger = logger;
  m_path_drawer->set_logger(logger);
}


/* frame is equal to current, need it to not block mutex */
void module_path_processor::PathProcessor::draw_trace(Mat &frame, const vector<Point2f> &src_keypoints, const vector<Point2f> &dst_keypoints)
{
  const auto curr_size = dst_keypoints.size();
  const Scalar red(0, 0, 255);
  const Scalar green(0, 255, 0);

  for_each(dst_keypoints.begin(), dst_keypoints.end(),
           [frame](const Point2f& keypoint){ circle(frame, keypoint, 2, Scalar(0, 0, 255), -1); });

  if (curr_size == src_keypoints.size())
  {
    for(size_t idx = 0; idx < curr_size; idx++)
    {
      line(frame, dst_keypoints[idx], src_keypoints[idx], green, 2);
    }
  }
}


module_path_processor::PathProcessor::~PathProcessor()
{
  if (m_feature_tracker != nullptr)
  {
    delete m_feature_tracker;
  }
  delete m_feature_detector;
  delete m_video_reader;
  delete m_pose_estimator;
}


Mat module_path_processor::PathProcessor::get_curr_frame()
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


bool module_path_processor::PathProcessor::is_started()
{
  return m_is_running;
}


bool module_path_processor::PathProcessor::is_stopped()
{
  return m_is_stopped;
}


void module_path_processor::PathProcessor::start(submodule_type running_type)
{
  m_running_type = running_type;
  m_is_running = true;

  std::thread thread(&PathProcessor::thread_running, this, 1);
  thread.detach();
}


void module_path_processor::PathProcessor::step()
{
  m_running_mutex.lock();
  m_is_running = true;
  m_running_mutex.unlock();

  m_cond_variable.notify_one();
}


void module_path_processor::PathProcessor::pause()
{
  m_running_mutex.lock();
  m_is_running = false;
  m_running_mutex.unlock();
}


void module_path_processor::PathProcessor::continue_running()
{
  m_running_mutex.lock();
  m_is_running = true;
  m_running_mutex.unlock();

  m_cond_variable.notify_one();
}

void module_path_processor::PathProcessor::stop()
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


void module_path_processor::PathProcessor::thread_running(uint8_t job_count)
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


void module_path_processor::PathProcessor::process_frame(uint8_t job_count)
{
  Mat translation_mat;
  (void) job_count;

  if (!m_video_reader->is_finished())
  {
    auto frame = m_video_reader->read_next_frame();
    vector<Point2f> src_keypoints_copy;
    vector<Point2f> dst_keypoints_copy;

    LOG_INFO(m_logger, "Reading video frame: " + std::to_string(m_video_reader->get_current_frame_num()));

    m_feature_detector->detect_features(frame, m_keypoints[m_current_switch]);
    LOG_INFO(m_logger, "Detecting features: " + std::to_string(m_keypoints[m_current_switch].size()));

    src_keypoints_copy = m_keypoints[!m_current_switch];
    dst_keypoints_copy = m_keypoints[m_current_switch];

    /* If haven't found any keypoints, raise error */
    if (m_keypoints[!m_current_switch].size() > 0)
    {
      if (m_feature_tracker->get_type() == FEATURE_TRACKER_WITH_FRAME)
      {
        m_feature_tracker->track_features(m_img[!m_current_switch], frame, src_keypoints_copy, dst_keypoints_copy);
        LOG_INFO(m_logger, "Detecting features: " + std::to_string(m_keypoints[m_current_switch].size()));
      }
      else
      {
        m_feature_tracker->track_features(src_keypoints_copy, dst_keypoints_copy);
      }

      /* Call it here to block mutex for the least possible time */
      draw_trace(frame, src_keypoints_copy, dst_keypoints_copy);

      if (src_keypoints_copy.size() > 0 && dst_keypoints_copy.size() > 0)
      {
        translation_mat = m_pose_estimator->find_matrix(src_keypoints_copy, dst_keypoints_copy);
      }

      m_path_drawer->update_pos(translation_mat);
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


abstract_feature_detector *module_path_processor::construct_feature_detector(submodule_type detector_type)
{
  abstract_feature_detector *ret_value = nullptr;

  switch (detector_type) {
    case FAST_DETECTOR:
      ret_value = new fast_feature_detector();
      break;

    default:
      break;
  }

  return ret_value;
}


abstract_feature_tracker *module_path_processor::construct_feature_tracker(submodule_type tracker_type)
{
  abstract_feature_tracker *ret_value = nullptr;

  switch (tracker_type) {
    default:
      break;
  }

  return ret_value;
}


abstract_video_reader *module_path_processor::construct_video_reader(submodule_type reader_type, string path)
{
  abstract_video_reader *ret_value = nullptr;

  switch (reader_type) {
    case OPENCV_READER:
      ret_value = new open_cv_video_reader(path);
      break;
    default:
      break;
  }

  return ret_value;
}
