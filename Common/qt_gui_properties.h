#ifndef QT_GUI_PROPERTIES_H
#define QT_GUI_PROPERTIES_H

#include <QFrame>

#include <PoseEstimator/abstract_pose_estimator.h>

#include "FeatureDetector/feature_detector.h"
#include "FeatureTracker/feature_tracker.h"
#include "VideoReader/video_reader.h"


class AbstractInfoQtFrame : public QFrame
{
  Q_OBJECT
protected:
  func_error_handler m_error_handler;

public:
  AbstractInfoQtFrame(func_error_handler err_handler)
  {
    m_error_handler = err_handler;
  };

  virtual submodule_feature_detector::abstract_feature_detector *get_feature_detector(void) = 0;
  virtual submodule_feature_tracker::abstract_feature_tracker *get_feature_tracker(void) = 0;
  virtual submodule_video_reader::abstract_video_reader *get_video_reader(void) = 0;
  virtual submodule_pose_estimator::abstract_pose_estimator *get_pose_estimator(void) = 0;
  virtual submodule_type get_type(void) = 0;
  virtual std::string get_name(void) = 0;
};


#endif // QT_GUI_PROPERTIES_H
