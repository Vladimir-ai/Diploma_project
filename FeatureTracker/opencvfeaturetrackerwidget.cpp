#include "opencvfeaturetracker.h"
#include "opencvfeaturetrackerwidget.h"

OpencvFeatureTrackerWidget::OpencvFeatureTrackerWidget(func_error_handler error_handler):AbstractInfoQtFrame(error_handler)
{
}


submodule_feature_detector::abstract_feature_detector *OpencvFeatureTrackerWidget::get_feature_detector()
{
  return nullptr;
}


submodule_feature_tracker::abstract_feature_tracker *OpencvFeatureTrackerWidget::get_feature_tracker()
{
  return new OpencvFeatureTracker();
}


submodule_video_reader::abstract_video_reader *OpencvFeatureTrackerWidget::get_video_reader()
{
  return nullptr;
}


submodule_pose_estimator::abstract_pose_estimator *OpencvFeatureTrackerWidget::get_pose_estimator()
{
  return nullptr;
}


submodule_type OpencvFeatureTrackerWidget::get_type()
{
  return OPENCV_FEATURE_TRACKER;
}


string OpencvFeatureTrackerWidget::get_name()
{
  return "Lukas-Kanade";
}
