#ifndef QT_GUI_PROPERTIES_H
#define QT_GUI_PROPERTIES_H

#include <QFrame>

#include "FeatureDetector/feature_detector.h"
#include "FeatureTracker/feature_tracker.h"
#include "VideoReader/video_reader.h"


class AbstractInfoQtFrame : public QFrame
{
  Q_OBJECT

public:
  virtual submodule_feature_detector::abstract_feature_detector *get_feature_detector(void) = 0;
  virtual submodule_feature_tracker::abstract_feature_tracker *get_feature_tracker(void) = 0;
  virtual submodule_video_reader::abstract_video_reader *get_video_reader(void) = 0;
  virtual submodule_type get_type(void) = 0;
};


#endif // QT_GUI_PROPERTIES_H
