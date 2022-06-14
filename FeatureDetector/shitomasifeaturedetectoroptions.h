#ifndef SHITOMASIFEATUREDETECTOROPTIONS_H
#define SHITOMASIFEATUREDETECTOROPTIONS_H

#include <Common/qt_gui_properties.h>



class ShiTomasiFeatureDetectorOptions : public AbstractInfoQtFrame
{
public:
  ShiTomasiFeatureDetectorOptions(Statistics *stat = nullptr,
                                  AbstractLogger *logger = nullptr,
                                  FuncErrorHandler err_handler = nullptr);

  // AbstractInfoQtFrame interface
public:
  SubmoduleFeatureDetector::IAbstractFeatureDetector *get_feature_detector();
  SubmoduleFeatureTracker::IAbstractFeatureTracker *get_feature_tracker();
  SubmoduleVideoReader::IAbstractVideoReader *get_video_reader();
  SubmodulePoseEstimator::IAbstractPoseEstimator *get_pose_estimator();
  submodule_type get_type();
  string get_name();
};

#endif // SHITOMASIFEATUREDETECTOROPTIONS_H
