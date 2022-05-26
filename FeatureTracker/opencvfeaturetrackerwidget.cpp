#include "opencvfeaturetracker.h"
#include "opencvfeaturetrackerwidget.h"

OpencvFeatureTrackerWidget::OpencvFeatureTrackerWidget(
    Statistics *stat, AbstractLogger *logger, FuncErrorHandler error_handler): AbstractInfoQtFrame(stat, logger, error_handler)
{
}


SubmoduleFeatureDetector::IAbstractFeatureDetector *OpencvFeatureTrackerWidget::get_feature_detector()
{
  return nullptr;
}


SubmoduleFeatureTracker::IAbstractFeatureTracker *OpencvFeatureTrackerWidget::get_feature_tracker()
{
  return new OpencvFeatureTracker(m_stat);
}


SubmoduleVideoReader::IAbstractVideoReader *OpencvFeatureTrackerWidget::get_video_reader()
{
  return nullptr;
}


SubmodulePoseEstimator::IAbstractPoseEstimator *OpencvFeatureTrackerWidget::get_pose_estimator()
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
