#include "path_processor.h"
#include "FeatureDetector/fast_feature_detector.h"
#include "VideoReader/open_cv_video_reader.h"

#include <thread>


module_path_processor::PathProcessor::PathProcessor(abstract_feature_tracker *feature_tracker,
                                                      abstract_feature_detector *feature_detector,
                                                      abstract_video_reader *video_reader)
{
  m_feature_tracker = feature_tracker;
  m_feature_detector = feature_detector;
  m_video_reader = video_reader;
}


module_path_processor::PathProcessor::~PathProcessor()
{
  if (m_feature_tracker != nullptr)
  {
    delete m_feature_tracker;
  }
  delete m_feature_detector;
  delete m_video_reader;
}


Mat module_path_processor::PathProcessor::get_curr_frame()
{
  Mat img_copy;

  m_img_mutex.lock();
  if (m_current_img.size != 0)
  {
    img_copy = m_current_img.clone();
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
  m_running_mutex.lock();
  m_video_reader->stop();
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
      while (!m_is_stopped && !m_video_reader->is_finished())
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
  (void) job_count;

  if (!m_video_reader->is_finished())
  {
    auto frame = m_video_reader->read_next_frame();
    if (m_logger)
    {
      m_logger->log_info("Reading video frame: " + std::to_string(m_video_reader->get_current_frame_num()));
    }

    m_img_mutex.lock();
    cvtColor(frame, m_current_img, cv::COLOR_BGR2RGB);
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
