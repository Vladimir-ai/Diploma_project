#ifndef OPENCV_POSE_ESTIMATOR_WIDGET_H
#define OPENCV_POSE_ESTIMATOR_WIDGET_H

#include <Common/qt_gui_properties.h>
#include "opencv_pose_estimator.h"

#include <QVBoxLayout>
#include <QPushButton>

class OpencvPoseEstimatorWidget : public AbstractInfoQtFrame
{
friend class SubmodulePoseEstimator::OpencvPoseEstimator;
private:
  QVBoxLayout *m_layout;
  QPushButton *m_camera_matrix_push_button;

  double m_focal;
  Point2d m_pp;
  int m_method;
  double m_prob;
  double m_threshold;

  Mat m_cam_matrix;
  Mat m_dist_coefs;

  static inline std::vector<cv::Point3f> init_world_coordinates(const int rows, const int cols);

public:
  OpencvPoseEstimatorWidget(Statistics *stat, AbstractLogger *logger, FuncErrorHandler err_handler);

  // AbstractInfoQtFrame interface
  SubmoduleFeatureDetector::IAbstractFeatureDetector *get_feature_detector();
  SubmoduleFeatureTracker::IAbstractFeatureTracker *get_feature_tracker();
  SubmoduleVideoReader::IAbstractVideoReader *get_video_reader();
  SubmodulePoseEstimator::IAbstractPoseEstimator *get_pose_estimator();
  submodule_type get_type();
  string get_name();

private slots:
  void call_camera_calib_data(void);
};

#endif // OPENCV_POSE_ESTIMATOR_WIDGET_H
