#include "pathdrawerqwidget.h"

#include <QFileDialog>
#include <QPushButton>
#include <QVBoxLayout>
#include <filesystem>


PathDrawerQWidget::PathDrawerQWidget(QWidget *parent): QWidget(parent)
{
  this->resize(PathDrawer::get_width(), PathDrawer::get_heigth());

  QVBoxLayout *vbox_layout = new QVBoxLayout(this);
  QHBoxLayout *btns_layout = new QHBoxLayout(this);
  m_image_qlabel = new QLabel();

  QPushButton *hide_btn = new QPushButton("Hide");
  QPushButton *save_btn = new QPushButton("Save");

  btns_layout->addWidget(save_btn);
  btns_layout->addWidget(hide_btn);

  vbox_layout->addWidget(m_image_qlabel);
  vbox_layout->addLayout(btns_layout);

  this->setLayout(vbox_layout);
  this->setWindowTitle("Map");

  m_timer = new QTimer(this);
  m_timer->setTimerType(Qt::TimerType::CoarseTimer);
  m_timer->setSingleShot(false);

  connect(m_timer, &QTimer::timeout, this, &PathDrawerQWidget::timer_timeout);
  connect(hide_btn, &QPushButton::clicked, this, &PathDrawerQWidget::hide);
  connect(save_btn, &QPushButton::clicked, this, &PathDrawerQWidget::handle_save_img_btn);
}


PathDrawerQWidget::PathDrawerQWidget(PathDrawer *path_drawer): PathDrawerQWidget()
{
  m_path_drawer = path_drawer;
}


PathDrawerQWidget::~PathDrawerQWidget()
{
  delete m_timer;
  delete m_image_qlabel;
}


void PathDrawerQWidget::set_path_drawer(PathDrawer *path_drawer)
{
  m_path_drawer = path_drawer;
}


PathDrawer *PathDrawerQWidget::get_path_drawer()
{
  return m_path_drawer;
}


void PathDrawerQWidget::handle_save_img_btn()
{
  QString file_name = QFileDialog::getSaveFileName(this,
          "Save", ".",
          "Png Images(*.png);;JPEG Images(*.jpg)");

  std::string extension = std::filesystem::path(file_name.toStdString()).extension();
  extension.erase(0, 1);

  m_image_qlabel->pixmap().save(file_name, extension.c_str());
}


void PathDrawerQWidget::show()
{
  m_timer->start(mc_timer_timeout);
  QWidget::show();
}


void PathDrawerQWidget::hide()
{
  m_timer->stop();
  QWidget::hide();
}


void PathDrawerQWidget::timer_timeout()
{
  if (m_path_drawer)
  {
    Mat img = m_path_drawer->get_img();
    if (img.data)
    {
      QImage img_copy = QImage(img.data, img.cols, img.rows, img.step, QImage::Format_BGR888);
      QPixmap pixmap = QPixmap::fromImage(img_copy);
      m_image_qlabel->setPixmap(pixmap.scaled(m_image_qlabel->width(), m_image_qlabel->height(), Qt::AspectRatioMode::KeepAspectRatio));
    }
  }
}

