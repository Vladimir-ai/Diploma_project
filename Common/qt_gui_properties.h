#ifndef QT_GUI_PROPERTIES_H
#define QT_GUI_PROPERTIES_H

#include <QFrame>

#include "PoseEstimator/abstract_pose_estimator.h"

#include "FeatureDetector/feature_detector.h"
#include "FeatureTracker/feature_tracker.h"
#include "VideoReader/video_reader.h"


class AbstractInfoQtFrame : public QFrame
{
  Q_OBJECT
protected:
  FuncErrorHandler m_error_handler;
  AbstractLogger *m_logger;
  Statistics *m_stat;

public:
  AbstractInfoQtFrame(Statistics *stat, AbstractLogger *logger, FuncErrorHandler err_handler)
  {
    m_error_handler = err_handler;
    m_logger = logger;
    m_stat = stat;
  };

  virtual SubmoduleFeatureDetector::IAbstractFeatureDetector *get_feature_detector() = 0;
  virtual SubmoduleFeatureTracker::IAbstractFeatureTracker *get_feature_tracker() = 0;
  virtual SubmoduleVideoReader::IAbstractVideoReader *get_video_reader() = 0;
  virtual SubmodulePoseEstimator::IAbstractPoseEstimator *get_pose_estimator() = 0;
  virtual submodule_type get_type(void) = 0;
  virtual std::string get_name(void) = 0;
};


#endif // QT_GUI_PROPERTIES_H
