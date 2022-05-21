#ifndef FAST_FEATURE_DETECTOR_OPTIONS_H
#define FAST_FEATURE_DETECTOR_OPTIONS_H

#include "Common/qt_gui_properties.h"

#include <QComboBox>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>

class fast_feature_detector_options : public AbstractInfoQtFrame
{
private:
  QVBoxLayout *m_layout;
  QLineEdit *m_threshold;
  QComboBox *m_type_selection;
  QPushButton *m_nonmax_supression;
public:
  fast_feature_detector_options();
  ~fast_feature_detector_options();

  // AbstractInfoQtWidget interface
  submodule_feature_detector::abstract_feature_detector *get_feature_detector() override;
  submodule_feature_tracker::abstract_feature_tracker *get_feature_tracker() override;
  submodule_video_reader::abstract_video_reader *get_video_reader() override;
  submodule_type get_type() override;
};

#endif // FAST_FEATURE_DETECTOR_OPTIONS_H
