#ifndef OPENCV_POSE_ESTIMATOR_WIDGET_H
#define OPENCV_POSE_ESTIMATOR_WIDGET_H

#include <Common/qt_gui_properties.h>


class OpencvPoseEstimatorWidget : public AbstractInfoQtFrame
{
public:
  OpencvPoseEstimatorWidget(func_error_handler err_handler);

  // AbstractInfoQtFrame interface
  submodule_feature_detector::abstract_feature_detector *get_feature_detector();
  submodule_feature_tracker::abstract_feature_tracker *get_feature_tracker();
  submodule_video_reader::abstract_video_reader *get_video_reader();
  submodule_pose_estimator::abstract_pose_estimator *get_pose_estimator();
  submodule_type get_type();
  string get_name();
};

#endif // OPENCV_POSE_ESTIMATOR_WIDGET_H
