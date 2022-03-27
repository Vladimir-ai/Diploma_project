#include "path_processor.h"



module_path_processor::path_processor::path_processor(abstract_feature_tracker *feature_tracker,
                                                      abstract_feature_detector *feature_detector,
                                                      abstract_video_reader *video_reader,
                                                      submodule_type running_type)
{
  m_feature_tracker = feature_tracker;
  m_feature_detector = feature_detector;
  m_video_reader = video_reader;
  m_running_type = running_type;
}

module_path_processor::path_processor::~path_processor()
{
  delete m_feature_tracker;
  delete m_feature_detector;
  delete m_video_reader;
}
