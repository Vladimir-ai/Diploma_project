#ifndef ORBFEATUREDETECTOROPTIONS_H
#define ORBFEATUREDETECTOROPTIONS_H

#include <Common/qt_gui_properties.h>


class OrbFeatureDetectorOptions : public AbstractInfoQtFrame
{
private:
  int m_nfeatures=500;
  float m_scaleFactor=1.2f;
  int m_nlevels=8;
  int m_edgeThreshold=31;
  int m_firstLevel=0;
  int m_patchSize=31;
  int m_fastThreshold=20;

public:
  OrbFeatureDetectorOptions(Statistics *stat, AbstractLogger *logger, FuncErrorHandler err_handler);

  // AbstractInfoQtFrame interface
public:
  SubmoduleFeatureDetector::IAbstractFeatureDetector *get_feature_detector();
  SubmoduleFeatureTracker::IAbstractFeatureTracker *get_feature_tracker();
  SubmoduleVideoReader::IAbstractVideoReader *get_video_reader();
  SubmodulePoseEstimator::IAbstractPoseEstimator *get_pose_estimator();
  submodule_type get_type();
  string get_name();
};

#endif // ORBFEATUREDETECTOROPTIONS_H
