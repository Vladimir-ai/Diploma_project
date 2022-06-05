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
//  std::string m_path = "/home/krevedko/Desktop/14/image_0/res.mp4";
//  std::string m_path = "/home/krevedko/PyCharmProjects/pyslam/videos/kitti00/video.mp4";
//  std::string m_path = "/home/krevedko/Desktop/Курсач/Cam/Cropped/VID_20210405_081118.mp4";
  std::string m_path = "/home/krevedko/Desktop/Курсач/Cam/Cropped/VID_20210405_072743.mp4";

  void call_video_selection(void);

public:
  OpenCvVideoReaderInfoWidget(Statistics *stat, AbstractLogger *logger, FuncErrorHandler err_handler);
  ~OpenCvVideoReaderInfoWidget();

  // AbstractInfoQtWidget interface
  SubmoduleFeatureDetector::IAbstractFeatureDetector *get_feature_detector();
  SubmoduleFeatureTracker::IAbstractFeatureTracker *get_feature_tracker();
  SubmoduleVideoReader::IAbstractVideoReader *get_video_reader();
  SubmodulePoseEstimator::IAbstractPoseEstimator *get_pose_estimator();
  submodule_type get_type();
  string get_name();
};

#endif // OPENCV_VIDEO_READER_WIDGET_H
