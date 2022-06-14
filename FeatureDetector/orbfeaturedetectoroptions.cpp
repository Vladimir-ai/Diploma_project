#include "orbfeaturedetectoroptions.h"

#include <QIntValidator>
#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>


OrbFeatureDetectorOptions::OrbFeatureDetectorOptions(Statistics *stat,
                                                     AbstractLogger *logger,
                                                     FuncErrorHandler err_handler):AbstractInfoQtFrame(stat, logger, err_handler)
{
  QLayout *layout = new QVBoxLayout(this);
  QLineEdit *num_features = new QLineEdit();
  QLineEdit *scale_factor = new QLineEdit();
  QLineEdit *num_levels = new QLineEdit();

  QLabel *num_features_qlaber = new QLabel("Num features");
  num_features->setText("500");
  QLabel *scale_factor_qlaber = new QLabel("Scale factor");
  scale_factor->setText("1.2");
  QLabel *num_levels_qlaber = new QLabel("Num levels");
  num_levels->setText("8");

  num_features->setValidator(new QIntValidator(0, 1000));
  scale_factor->setValidator(new QDoubleValidator(0, 5, 2));
  num_levels->setValidator(new QIntValidator(0, 50));

  layout->addWidget(num_features_qlaber);
  layout->addWidget(num_features);
  layout->addWidget(scale_factor_qlaber);
  layout->addWidget(scale_factor);
  layout->addWidget(num_levels_qlaber);
  layout->addWidget(num_levels);
  QPalette pal = QPalette();
  pal.setColor(QPalette::Window, Qt::lightGray);

  this->setAutoFillBackground(true);
  this->setPalette(pal);

  this->setFrameShape(Shape::Box);
  this->setLineWidth(1);
  /* Do nothing */
}

SubmoduleFeatureDetector::IAbstractFeatureDetector *OrbFeatureDetectorOptions::get_feature_detector()
{
  return nullptr;
}


SubmoduleFeatureTracker::IAbstractFeatureTracker *OrbFeatureDetectorOptions::get_feature_tracker()
{
  return nullptr;
}


SubmoduleVideoReader::IAbstractVideoReader *OrbFeatureDetectorOptions::get_video_reader()
{
  return nullptr;
}

SubmodulePoseEstimator::IAbstractPoseEstimator *OrbFeatureDetectorOptions::get_pose_estimator()
{
  return nullptr;
}


submodule_type OrbFeatureDetectorOptions::get_type()
{
  return DYNAMIC_LOADED_DETECTOR;
}

string OrbFeatureDetectorOptions::get_name()
{
  return "ORB";
}


