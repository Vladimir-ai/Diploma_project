#include "mainwindow.h"

#include <QtDebug>
#include <QPushButton>
#include <QLabel>
#include <QKeyEvent>
#include <QFileDialog>
#include "PathProcessor/path_processor.h"
#include "FeatureDetector/fast_feature_detector.h"
#include "VideoReader/open_cv_video_reader.h"

//make factories
//factory video driver
//factory feature detector
//factory feature tracker.

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
  setWindowTitle("Visual Odometry");
  resize(640, 480);

  setup_layout();
  setup_options_vbox_layout();
  setup_timer();
}


MainWindow::~MainWindow()
{
  delete m_central_widget;
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

  m_select_file_btn = new QPushButton("Select file");

  QVBoxLayout *feature_tracker_layout = new QVBoxLayout();
  QLabel *feature_tracker_label = new QLabel("Feature Tracker type:");
  QComboBox *feature_tracker_box = new QComboBox();
  feature_tracker_box->addItem("Not Implemented", NOT_IMPLEMENTED);
  feature_tracker_layout->addWidget(feature_tracker_label);
  feature_tracker_layout->addWidget(feature_tracker_box);
  feature_tracker_layout->setSpacing(5);


  QVBoxLayout *feature_detector_layout = new QVBoxLayout();
  QLabel *feature_detector_label = new QLabel("Feature Detector type:");
  QComboBox *feature_detector_box = new QComboBox();
  feature_detector_box->addItem("FAST", FAST_DETECTOR);
  feature_detector_layout->addWidget(feature_detector_label);
  feature_detector_layout->addWidget(feature_detector_box);
  feature_detector_layout->setSpacing(5);

  QVBoxLayout *video_reader_layout = new QVBoxLayout();
  QLabel *video_reader_label = new QLabel("Video Reader type:");
  QComboBox *video_reader_box = new QComboBox();
  video_reader_box->addItem("Video file", OPENCV_READER);
  video_reader_layout->addWidget(video_reader_label);
  video_reader_layout->addWidget(video_reader_box);
  video_reader_layout->addWidget(m_select_file_btn);
  video_reader_layout->setSpacing(5);

//  connect(video_reader_box, )

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


  m_option_box_layout->addLayout(feature_tracker_layout);

  m_option_box_layout->addLayout(feature_detector_layout);

  m_option_box_layout->addLayout(video_reader_layout);
  m_option_box_layout->addStretch(1);
  m_option_box_layout->addLayout(m_running_type_layout);

  m_options_widget->setMaximumWidth(150);

  connect(m_start_btn, &QPushButton::clicked, this, &MainWindow::handle_start_button);
  connect(m_stop_btn, &QPushButton::clicked, this, &MainWindow::handle_stop_button);
  connect(m_continue_btn, &QPushButton::clicked, this, &MainWindow::handle_continue_button);
  connect(m_pause_btn, &QPushButton::clicked, this, &MainWindow::handle_pause_button);

  connect(m_running_type_selector, &QComboBox::currentIndexChanged, this, &MainWindow::handle_running_mode_change);

  connect(m_select_file_btn, &QPushButton::clicked, this, &MainWindow::call_video_selection);

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

  abstract_feature_detector *feature_detector = new fast_feature_detector();
  abstract_video_reader *video_reader = new open_cv_video_reader(m_path_to_video_file.toStdString());

  m_path_processor = new module_path_processor::PathProcessor(nullptr, feature_detector, video_reader);

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
    QImage img_copy = QImage(img.data, img.cols, img.rows, img.step, QImage::Format_RGB888);
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


void MainWindow::call_video_selection()
{
  QString path = QFileDialog::getOpenFileName(this, "Select a video", ".", "Videos (*.mp4 *.avi)");

  if (QFileInfo::exists(path))
  {
    m_path_to_video_file = path;
  }
}




