#ifndef OPENCV_VIDEO_READER_WIDGET_H
#define OPENCV_VIDEO_READER_WIDGET_H

#include "Common/qt_gui_properties.h"

#include <QPushButton>
#include <QVBoxLayout>


class OpenCvVideoReaderInfoWidget : public AbstractInfoQtFrame
{
private:
  QVBoxLayout *m_layout;
  QPushButton *m_path_button;
  std::string m_path;

  void call_video_selection(void);

public:
  OpenCvVideoReaderInfoWidget(func_error_handler err_handler);
  ~OpenCvVideoReaderInfoWidget();

  // AbstractInfoQtWidget interface
  submodule_feature_detector::abstract_feature_detector *get_feature_detector();
  submodule_feature_tracker::abstract_feature_tracker *get_feature_tracker();
  submodule_video_reader::abstract_video_reader *get_video_reader();
  submodule_pose_estimator::abstract_pose_estimator *get_pose_estimator();
  submodule_type get_type();
  string get_name();
};

#endif // OPENCV_VIDEO_READER_WIDGET_H
