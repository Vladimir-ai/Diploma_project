#include "mainwindow.h"

#include <QDebug>
#include <QPushButton>
#include <QComboBox>
#include "PathProcessor/path_processor.h"

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
  m_scene = new QGraphicsScene(m_central_widget);
  m_view = new QGraphicsView(m_scene);
  m_pixmap = new QPixmap("/home/krevedko/VSU/Diploma_project/download.jpeg");
  // Reduce size to QWidgetSize
  m_item = new QGraphicsPixmapItem(*m_pixmap);

  m_scene->addItem(m_item);
  m_main_hbox_layout->addWidget(m_view);

  m_central_widget->setLayout(m_main_hbox_layout);
}


void MainWindow::setup_options_vbox_layout()
{
  m_options_widget = new QWidget();
  m_option_box_layout = new QVBoxLayout(m_options_widget);

  QComboBox *feature_tracker_box = new QComboBox();
  feature_tracker_box->addItem("Not Implemented", NOT_IMPLEMENTED);

  QComboBox *feature_detector_box = new QComboBox();
  feature_detector_box->addItem("FAST", FAST_DETECTOR);

  QComboBox *video_reader_box = new QComboBox();
  video_reader_box->addItem("Video file", OPENCV_READER);

  QPushButton *btn1 = new QPushButton("Start");

  m_option_box_layout->addWidget(feature_tracker_box);
  m_option_box_layout->addWidget(feature_detector_box);
  m_option_box_layout->addWidget(video_reader_box);
  m_option_box_layout->addWidget(btn1);

  m_options_widget->setMaximumWidth(150);

  connect(btn1, &QPushButton::clicked, this, &MainWindow::handleButton1);

  m_options_widget->setLayout(m_option_box_layout);
  m_options_widget->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred));

  m_main_hbox_layout->addWidget(m_options_widget);
}


void MainWindow::handleButton1()
{
  m_item->setPixmap(QPixmap("/home/krevedko/VSU/Diploma_project/photo-1453728013993-6d66e9c9123a.jpeg"));
  m_view->fitInView(m_scene->itemsBoundingRect(), Qt::KeepAspectRatio);
}


void MainWindow::handleButton2()
{
  m_item->setPixmap(QPixmap("/home/krevedko/VSU/Diploma_project/download.jpeg"));
}


