#ifndef FAST_FEATURE_DETECTOR_OPTIONS_H
#define FAST_FEATURE_DETECTOR_OPTIONS_H

#include "Common/qt_gui_properties.h"

#include <QComboBox>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>

class FastFeatureDetectorOptions : public AbstractInfoQtFrame
{
private:
  QVBoxLayout *m_layout;
  QLineEdit *m_threshold;
  QComboBox *m_type_selection;
  QPushButton *m_nonmax_supression;

public:
  FastFeatureDetectorOptions(Statistics *stat, AbstractLogger *logger, FuncErrorHandler err_handler);
  ~FastFeatureDetectorOptions();

  // AbstractInfoQtWidget interface
  SubmoduleFeatureDetector::IAbstractFeatureDetector *get_feature_detector() override;
  SubmoduleFeatureTracker::IAbstractFeatureTracker *get_feature_tracker() override;
  SubmoduleVideoReader::IAbstractVideoReader *get_video_reader() override;
  SubmodulePoseEstimator::IAbstractPoseEstimator *get_pose_estimator() override;
  submodule_type get_type() override;
  string get_name() override;
};

#endif // FAST_FEATURE_DETECTOR_OPTIONS_H
