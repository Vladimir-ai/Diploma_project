#include "path_processor.h"
#include "FeatureDetector/fast_feature_detector.h"
#include "VideoReader/open_cv_video_reader.h"


module_path_processor::path_processor::path_processor(abstract_feature_tracker *feature_tracker,
                                                      abstract_feature_detector *feature_detector,
                                                      abstract_video_reader *video_reader)
{
  m_feature_tracker = feature_tracker;
  m_feature_detector = feature_detector;
  m_video_reader = video_reader;
}


module_path_processor::path_processor::~path_processor()
{
  delete m_feature_tracker;
  delete m_feature_detector;
  delete m_video_reader;
}


Eigen::MatrixXi module_path_processor::path_processor::get_curr_frame()
{
  return m_current_img;
}


void module_path_processor::path_processor::start(submodule_type running_type)
{
  m_running_type = running_type;
  m_is_running = true;
}


void module_path_processor::path_processor::pause()
{
  m_is_running = false;
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
