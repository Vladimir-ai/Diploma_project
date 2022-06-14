#include "shitomasifeaturedetectoroptions.h"

ShiTomasiFeatureDetectorOptions::ShiTomasiFeatureDetectorOptions(Statistics *stat,
                                                                 AbstractLogger *logger,
                                                                 FuncErrorHandler err_handler):AbstractInfoQtFrame(stat, logger, err_handler)
{

}

SubmoduleFeatureDetector::IAbstractFeatureDetector *ShiTomasiFeatureDetectorOptions::get_feature_detector()
{
  return nullptr;
}

SubmoduleFeatureTracker::IAbstractFeatureTracker *ShiTomasiFeatureDetectorOptions::get_feature_tracker()
{
  return nullptr;
}

SubmoduleVideoReader::IAbstractVideoReader *ShiTomasiFeatureDetectorOptions::get_video_reader()
{
  return nullptr;
}

SubmodulePoseEstimator::IAbstractPoseEstimator *ShiTomasiFeatureDetectorOptions::get_pose_estimator()
{
  return nullptr;
}

submodule_type ShiTomasiFeatureDetectorOptions::get_type()
{
  return DYNAMIC_LOADED_DETECTOR;
}


string ShiTomasiFeatureDetectorOptions::get_name()
{
  return "Shi-Tomasi";
}
