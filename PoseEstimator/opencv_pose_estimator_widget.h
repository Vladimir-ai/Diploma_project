#ifndef OPENCV_POSE_ESTIMATOR_WIDGET_H
#define OPENCV_POSE_ESTIMATOR_WIDGET_H

#include <Common/qt_gui_properties.h>

#include <QVBoxLayout>
#include <QPushButton>

class OpencvPoseEstimatorWidget : public AbstractInfoQtFrame
{
private:
  QVBoxLayout *m_layout;
  QPushButton *m_camera_matrix_push_button;

  double m_focal;
  Point2d m_pp;
  int m_method;
  double m_prob;
  double m_threshold;

  static inline std::vector<cv::Point3f> init_world_coordinates(const int rows, const int cols);

public:
  OpencvPoseEstimatorWidget(abstract_logger *logger, func_error_handler err_handler);

  // AbstractInfoQtFrame interface
  submodule_feature_detector::abstract_feature_detector *get_feature_detector();
  submodule_feature_tracker::abstract_feature_tracker *get_feature_tracker();
  submodule_video_reader::abstract_video_reader *get_video_reader();
  submodule_pose_estimator::abstract_pose_estimator *get_pose_estimator();
  submodule_type get_type();
  string get_name();

private slots:
  void call_camera_calib_data(void);
};

#endif // OPENCV_POSE_ESTIMATOR_WIDGET_H
