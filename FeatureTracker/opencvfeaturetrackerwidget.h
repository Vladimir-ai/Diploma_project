#ifndef OPENCVFEATURETRACKERWIDGET_H
#define OPENCVFEATURETRACKERWIDGET_H

#include <Common/qt_gui_properties.h>



class OpencvFeatureTrackerWidget: public AbstractInfoQtFrame
{
public:
  OpencvFeatureTrackerWidget(abstract_logger *logger, func_error_handler error_handler);

  // AbstractInfoQtFrame interface
  submodule_feature_detector::abstract_feature_detector *get_feature_detector();
  submodule_feature_tracker::abstract_feature_tracker *get_feature_tracker();
  submodule_video_reader::abstract_video_reader *get_video_reader();
  submodule_pose_estimator::abstract_pose_estimator *get_pose_estimator();
  submodule_type get_type();
  string get_name();
};

#endif // OPENCVFEATURETRACKERWIDGET_H
