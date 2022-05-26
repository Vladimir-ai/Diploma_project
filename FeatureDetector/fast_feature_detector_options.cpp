#include "fast_feature_detector.h"
#include "fast_feature_detector_options.h"

#include <QDoubleValidator>
#include <QLabel>
#include <QVBoxLayout>

FastFeatureDetectorOptions::FastFeatureDetectorOptions(
    Statistics *stat, AbstractLogger *logger, FuncErrorHandler err_handler): AbstractInfoQtFrame(stat, logger, err_handler)
{
  m_layout = new QVBoxLayout(this);
  m_threshold = new QLineEdit();
  m_type_selection = new QComboBox();

  m_nonmax_supression = new QPushButton("Nonmax Supression");
  QLabel *threshold_qlabel = new QLabel("Threshold");
  m_threshold->setValidator(new QDoubleValidator(0, 1000, 2));
  m_threshold->setText("16");

  QLabel *type_label = new QLabel("N");
  m_type_selection->addItem("N = 8", FastFeatureDetector::DetectorType::TYPE_5_8);
  m_type_selection->addItem("N = 12", FastFeatureDetector::DetectorType::TYPE_7_12);
  m_type_selection->addItem("N = 16", FastFeatureDetector::DetectorType::TYPE_9_16);

  m_nonmax_supression->setCheckable(true);
  m_nonmax_supression->setChecked(true);

  m_layout->addWidget(threshold_qlabel);
  m_layout->addWidget(m_threshold);
  m_layout->addWidget(type_label);
  m_layout->addWidget(m_type_selection);
  m_layout->addWidget(m_nonmax_supression);

  QPalette pal = QPalette();
  pal.setColor(QPalette::Window, Qt::lightGray);

  this->setAutoFillBackground(true);
  this->setPalette(pal);

  this->setFrameShape(Shape::Box);
  this->setLineWidth(1);
}


FastFeatureDetectorOptions::~FastFeatureDetectorOptions()
{
  delete m_layout;
  delete m_threshold;
}


SubmoduleFeatureDetector::IAbstractFeatureDetector *FastFeatureDetectorOptions::get_feature_detector()
{
  return new OpencvFastFeatureDetector(m_stat, m_threshold->text().toDouble(),
                                   m_nonmax_supression->isChecked(),
                                   (FastFeatureDetector::DetectorType) m_type_selection->currentData().toInt());
}


SubmoduleFeatureTracker::IAbstractFeatureTracker *FastFeatureDetectorOptions::get_feature_tracker()
{
  return nullptr;
}


SubmoduleVideoReader::IAbstractVideoReader *FastFeatureDetectorOptions::get_video_reader()
{
  return nullptr;
}


SubmodulePoseEstimator::IAbstractPoseEstimator *FastFeatureDetectorOptions::get_pose_estimator()
{
  return nullptr;
}


submodule_type FastFeatureDetectorOptions::get_type()
{
  return FAST_DETECTOR;
}

string FastFeatureDetectorOptions::get_name()
{
  return "FAST";
}
