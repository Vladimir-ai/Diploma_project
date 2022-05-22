#include "opencv_pose_estimator.h"
#include "opencv_pose_estimator_widget.h"


OpencvPoseEstimatorWidget::OpencvPoseEstimatorWidget(func_error_handler err_handler): AbstractInfoQtFrame(err_handler)
{
}


submodule_feature_detector::abstract_feature_detector *OpencvPoseEstimatorWidget::get_feature_detector()
{
  return nullptr;
}


submodule_feature_tracker::abstract_feature_tracker *OpencvPoseEstimatorWidget::get_feature_tracker()
{
  return nullptr;
}


submodule_video_reader::abstract_video_reader *OpencvPoseEstimatorWidget::get_video_reader()
{
  return nullptr;
}


submodule_pose_estimator::abstract_pose_estimator *OpencvPoseEstimatorWidget::get_pose_estimator()
{
  return new submodule_pose_estimator::opencv_pose_estimator();
}


submodule_type OpencvPoseEstimatorWidget::get_type()
{
  return OPENCV_POSE_ESTIMATOR;
}


string OpencvPoseEstimatorWidget::get_name()
{
  return "Opencv Pose Estimator";
}

