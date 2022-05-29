#include "mainwindow.h"

#include <QtDebug>
#include <QPushButton>
#include <QLabel>
#include <QKeyEvent>
#include <QFileDialog>
#include <QMessageBox>
#include "FeatureDetector/fast_feature_detector_options.h"
#include "FeatureTracker/opencvfeaturetracker.h"
#include "FeatureTracker/opencvfeaturetrackerwidget.h"
#include "PathProcessor/path_processor.h"
#include "FeatureDetector/fast_feature_detector.h"
#include "PoseEstimator/opencv_pose_estimator_widget.h"
#include "VideoReader/open_cv_video_reader.h"
#include "Logger/boostlogger.h"
#include "VideoReader/opencv_video_reader_widget.h"

//make factories
//factory video driver
//factory feature detector
//factory feature tracker.

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
  setWindowTitle("Visual Odometry");
  resize(640, 480);
  m_logger = new BoostLogger(QDir::currentPath().toStdString() + "/text_log.log");

  setup_layout();
  setup_options_vbox_layout();
  setup_timer();

  m_drawer_widget = new PathDrawerQWidget();
  m_drawer_widget->set_stat(&m_statistics);

  m_central_widget->setFixedSize(1341, 682);
}


MainWindow::~MainWindow()
{
  m_drawer_widget->close();
  delete m_drawer_widget;
  delete m_central_widget;
}

void MainWindow::error_handler(string error)
{
  QMessageBox messageBox;
  messageBox.critical(0, "Error", QString::fromStdString(error));
  messageBox.show();
}


inline void MainWindow::init_video_reader_layout()
{
  m_video_reader_layout = new QVBoxLayout();
  QLabel *video_reader_label = new QLabel("Video Reader type:");
  m_video_reader_box = new QComboBox();

  m_reader_info = new OpenCvVideoReaderInfoWidget(&m_statistics, m_logger, error_handler);
  m_video_reader_box->addItem(QString::fromStdString(m_reader_info->get_name()), QVariant::fromValue(m_reader_info));
  m_video_reader_layout->addWidget(video_reader_label);
  m_video_reader_layout->addWidget(m_video_reader_box);
  m_video_reader_layout->addWidget(m_reader_info);
  m_video_reader_layout->setSpacing(5);
}


inline void MainWindow::init_feature_detector_layout()
{
  m_feature_detector_layout = new QVBoxLayout();
  QLabel *feature_detector_label = new QLabel("Feature Detector type:");
  m_feature_detector_box = new QComboBox();

  m_detector_info = new FastFeatureDetectorOptions(&m_statistics, m_logger, error_handler);

  m_feature_detector_box->addItem(QString::fromStdString(m_detector_info->get_name()), QVariant::fromValue(m_detector_info));
  m_feature_detector_layout->addWidget(feature_detector_label);
  m_feature_detector_layout->addWidget(m_feature_detector_box);
  m_feature_detector_layout->addWidget(m_detector_info);
  m_feature_detector_layout->setSpacing(5);
}


void MainWindow::init_feature_tracker_layout()
{
  m_feature_tracker_layout = new QVBoxLayout();
  QLabel *feature_tracker_label = new QLabel("Feature Tracker type:");
  m_feature_tracker_box = new QComboBox();

  m_tracker_info = new OpencvFeatureTrackerWidget(&m_statistics, m_logger, error_handler);

  m_feature_tracker_box->addItem(QString::fromStdString(m_tracker_info->get_name()), QVariant::fromValue(m_tracker_info));
  m_feature_tracker_layout->addWidget(feature_tracker_label);
  m_feature_tracker_layout->addWidget(m_feature_tracker_box);
  m_feature_tracker_layout->setSpacing(5);
}


void MainWindow::init_pose_estimator_layout()
{
  m_pose_estimator_layout = new QVBoxLayout();
  QLabel *pose_estimator_label = new QLabel("Pose estimator type:");
  m_pose_estimator_box = new QComboBox();

  m_pose_estimator_info = new OpencvPoseEstimatorWidget(&m_statistics, m_logger, error_handler);

  m_pose_estimator_box->addItem(QString::fromStdString(m_pose_estimator_info->get_name()), QVariant::fromValue(m_pose_estimator_info));

  m_pose_estimator_layout->addWidget(pose_estimator_label);
  m_pose_estimator_layout->addWidget(m_pose_estimator_box);
  m_pose_estimator_layout->addWidget(m_pose_estimator_info);
  m_pose_estimator_layout->setSpacing(5);
}


void MainWindow::setup_layout()
{
  m_central_widget = new QWidget();
  this->setCentralWidget(m_central_widget);

  m_main_hbox_layout = new QHBoxLayout(this);
  m_image_qlabel  = new QLabel(m_central_widget);
  m_image_qlabel->setScaledContents(true);
  m_image_qlabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  m_image_qlabel->setPixmap(QPixmap::fromImage(QImage("/home/krevedko/VSU/Diploma_project/download.jpeg")));

  m_main_hbox_layout->addWidget(m_image_qlabel);
  m_central_widget->setLayout(m_main_hbox_layout);
}


void MainWindow::setup_options_vbox_layout()
{
  m_options_widget = new QWidget();
  m_option_box_layout = new QVBoxLayout(m_options_widget);

  init_video_reader_layout();
  init_feature_detector_layout();
  init_feature_tracker_layout();
  init_pose_estimator_layout();

  m_start_btn = new QPushButton("Start");
  m_stop_btn = new QPushButton("Stop");
  m_continue_btn = new QPushButton("Continue");
  m_pause_btn = new QPushButton("Pause");

  m_running_type_layout = new QVBoxLayout();
  QLabel *running_type_qlabel = new QLabel("Running type:");
  m_running_type_selector = new QComboBox();
  m_running_type_selector->addItem("Continious run", CONTINUOUSLY);
  m_running_type_selector ->addItem("Stepped run", STEP_BY_STEP);

  m_running_type_layout->addWidget(running_type_qlabel);
  m_running_type_layout->addWidget(m_running_type_selector);
  m_running_type_layout->addWidget(m_start_btn);
  m_running_type_layout->setSpacing(5);

  m_stop_pause_layout = new QHBoxLayout();
  m_stop_pause_layout->addWidget(m_stop_btn);
  m_stop_pause_layout->addWidget(m_continue_btn);


  m_option_box_layout->addLayout(m_feature_detector_layout);
  m_option_box_layout->addLayout(m_feature_tracker_layout);
  m_option_box_layout->addLayout(m_pose_estimator_layout);

  m_option_box_layout->addLayout(m_video_reader_layout);
  m_option_box_layout->addStretch(1);
  m_option_box_layout->addLayout(m_running_type_layout);

  m_options_widget->setMaximumWidth(150);

  connect(m_start_btn, &QPushButton::clicked, this, &MainWindow::handle_start_button);
  connect(m_stop_btn, &QPushButton::clicked, this, &MainWindow::handle_stop_button);
  connect(m_continue_btn, &QPushButton::clicked, this, &MainWindow::handle_continue_button);
  connect(m_pause_btn, &QPushButton::clicked, this, &MainWindow::handle_pause_button);

  connect(m_feature_detector_box, &QComboBox::currentIndexChanged, this, &MainWindow::handle_detector_type_change);
  connect(m_running_type_selector, &QComboBox::currentIndexChanged, this, &MainWindow::handle_running_mode_change);

  m_options_widget->setLayout(m_option_box_layout);

  m_main_hbox_layout->addWidget(m_options_widget);
}


void MainWindow::setup_timer()
{
  m_timer = new QTimer(this);
  m_timer->setTimerType(Qt::TimerType::CoarseTimer);
  m_timer->setSingleShot(false);

  connect(m_timer, &QTimer::timeout, this, &MainWindow::timer_timeout);
}


void MainWindow::handle_start_button()
{
  m_running_type_selector->setEnabled(false);

  if (m_path_processor)
  {
    delete m_path_processor;
  }

  IAbstractFeatureDetector *feature_detector = m_detector_info->get_feature_detector();
  IAbstractVideoReader *video_reader = m_reader_info->get_video_reader();
  IAbstractFeatureTracker *feature_tracker = m_tracker_info->get_feature_tracker();
  IAbstractPoseEstimator *pose_estimator = m_pose_estimator_info->get_pose_estimator();
  PathDrawer *path_drawer = new PathDrawer(m_logger);

  if (m_drawer_widget->get_path_drawer())
  {
    delete m_drawer_widget->get_path_drawer();
  }
  m_drawer_widget->set_path_drawer(path_drawer);

  // TODO: add pose estimator to path processor

  m_path_processor = new ModulePathProcessor::PathProcessor(feature_tracker,
                                                            feature_detector,
                                                            video_reader,
                                                            pose_estimator,
                                                            path_drawer,
                                                            &m_statistics);

  m_path_processor->set_logger(m_logger);

  if (m_running_type_selector->currentIndex() == 0)
  {
    m_pause_btn->show();
    m_stop_pause_layout->replaceWidget(m_continue_btn, m_pause_btn);
    m_path_processor->start(CONTINUOUSLY);
  }
  else if (m_running_type_selector->currentIndex() == 1)
  {
    m_path_processor->start(STEP_BY_STEP);
  }

  m_running_type_layout->removeWidget(m_start_btn);
  m_start_btn->hide();

  m_running_type_layout->addLayout(m_stop_pause_layout);

  m_stop_btn->show();
  m_drawer_widget->show();

  m_timer->start(mc_timer_timeout);
}


void MainWindow::handle_stop_button()
{
  m_timer->stop();
  m_path_processor->stop();

  m_continue_btn->hide();
  m_pause_btn->hide();
  m_stop_btn->hide();
  m_start_btn->show();

  m_running_type_layout->removeItem(m_stop_pause_layout);
  m_running_type_layout->addWidget(m_start_btn);

  m_running_type_selector->setEnabled(true);
}


void MainWindow::handle_continue_button()
{
  if (!m_timer->isActive())
  {
    m_timer->start();
  }

  m_path_processor->continue_running();

  m_stop_pause_layout->replaceWidget(m_pause_btn, m_continue_btn);

  m_pause_btn->hide();
  m_continue_btn->show();
  m_running_type_selector->setEnabled(false);
}


void MainWindow::handle_pause_button()
{
  m_timer->stop();
  m_path_processor->pause();

  m_stop_pause_layout->replaceWidget(m_continue_btn, m_pause_btn);

  m_pause_btn->hide();
  m_continue_btn->show();
}


void MainWindow::handle_reader_type_change(int new_value)
{
  if (new_value != -1)
  {
    m_video_reader_layout->replaceWidget(m_reader_info, static_cast<AbstractInfoQtFrame *>(m_video_reader_box->itemData(new_value).data()));
    m_detector_info = static_cast<AbstractInfoQtFrame *>(m_video_reader_box->itemData(new_value).data());
  }
}


void MainWindow::handle_detector_type_change(int new_value)
{
  if (new_value != -1)
  {
    m_feature_detector_layout->replaceWidget(m_detector_info, static_cast<AbstractInfoQtFrame *>(m_feature_detector_box->itemData(new_value).data()));
    m_detector_info = static_cast<AbstractInfoQtFrame *>(m_feature_detector_box->itemData(new_value).data());
  }
}


void MainWindow::handle_tracker_type_change(int new_value)
{
  if (new_value != -1)
  {
    m_feature_detector_layout->replaceWidget(m_tracker_info, static_cast<AbstractInfoQtFrame *>(m_feature_tracker_box->itemData(new_value).data()));
    m_tracker_info = static_cast<AbstractInfoQtFrame *>(m_feature_tracker_box->itemData(new_value).data());
  }
}


void MainWindow::handle_pose_estimator_type_change(int new_value)
{
  if (new_value != -1)
  {
    m_pose_estimator_layout->replaceWidget(m_pose_estimator_info, static_cast<AbstractInfoQtFrame *>(m_pose_estimator_box->itemData(new_value).data()));
    m_pose_estimator_info = static_cast<AbstractInfoQtFrame *>(m_pose_estimator_box->itemData(new_value).data());
  }
}


void MainWindow::handle_running_mode_change(int new_value)
{
  switch (new_value)
  {
    case 0:
      m_continue_btn->setText("Continue");
      break;

    case 1:
      m_continue_btn->setText("Make step");
      break;

    default:
      break;
  }
}


void MainWindow::timer_timeout()
{
  if (m_path_processor && !m_path_processor->is_stopped())
  {
    Mat img = m_path_processor->get_curr_frame();
    QImage img_copy = QImage(img.data, img.cols, img.rows, img.step, QImage::Format_BGR888);
    img_copy.bits();
    QPixmap pixmap = QPixmap::fromImage(img_copy);

    if (!pixmap.isNull())
    {
      m_image_qlabel->setPixmap(pixmap.scaled(m_image_qlabel->width(), m_image_qlabel->height(), Qt::AspectRatioMode::KeepAspectRatio));
    }
  }
  else if (m_path_processor && m_path_processor->is_stopped())
  {
    m_image_qlabel->setPixmap(QPixmap());
    m_timer->stop();

    handle_stop_button();
  }

  qDebug() << "Timer timeout: m_path_processor " << m_path_processor << " is started " << (m_path_processor != nullptr ? m_path_processor->is_started() : 0);
}


void MainWindow::closeEvent(QCloseEvent *event)
{
  m_drawer_widget->close();
  QMainWindow::closeEvent(event);
}





