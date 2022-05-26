#ifndef OPENCVFEATURETRACKERWIDGET_H
#define OPENCVFEATURETRACKERWIDGET_H

#include <Common/qt_gui_properties.h>



class OpencvFeatureTrackerWidget: public AbstractInfoQtFrame
{
public:
  OpencvFeatureTrackerWidget(Statistics *stat, AbstractLogger *logger, FuncErrorHandler error_handler);

  // AbstractInfoQtFrame interface
  SubmoduleFeatureDetector::IAbstractFeatureDetector *get_feature_detector();
  SubmoduleFeatureTracker::IAbstractFeatureTracker *get_feature_tracker();
  SubmoduleVideoReader::IAbstractVideoReader *get_video_reader();
  SubmodulePoseEstimator::IAbstractPoseEstimator *get_pose_estimator();
  submodule_type get_type();
  string get_name();
};

#endif // OPENCVFEATURETRACKERWIDGET_H
