#include "pathdrawerqwidget.h"

#include <QFileDialog>
#include <QPushButton>
#include <QTextStream>
#include <QVBoxLayout>
#include <filesystem>


PathDrawerQWidget::PathDrawerQWidget(QWidget *parent): QWidget(parent)
{
  this->resize(PathDrawer::get_width(), PathDrawer::get_heigth());

  QHBoxLayout *btns_layout = new QHBoxLayout();
  QVBoxLayout *vbox_layout = new QVBoxLayout(this);
  m_image_qlabel = new QLabel();
  m_stat_qlabel = new QLabel();

  m_stat_qlabel->setFixedHeight(200);

  QPushButton *hide_btn = new QPushButton("Hide");
  QPushButton *save_btn = new QPushButton("Save");

  btns_layout->addWidget(save_btn);
  btns_layout->addWidget(hide_btn);

  vbox_layout->addWidget(m_image_qlabel);
  vbox_layout->addWidget(m_stat_qlabel);

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


PathDrawerQWidget::PathDrawerQWidget(PathDrawer *path_drawer, Statistics *stat): PathDrawerQWidget()
{
  m_path_drawer = path_drawer;
  m_stat = stat;
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


void PathDrawerQWidget::set_stat(Statistics *stat)
{
  m_stat = stat;
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

  if (m_stat)
  {
    QString str;
    QTextStream str_stream(&str);
    double total = accumulate(m_stat->m_general_stat.values.begin(), m_stat->m_general_stat.values.end(), decltype(m_stat->m_general_stat.values)::value_type(0));

    str_stream << "Total time: " << total << Qt::endl;
    str_stream << "Average frame time: " << total / m_stat->m_general_stat.values.size() << Qt::endl;

    if (m_stat->m_general_stat.values.size())
    {
      str_stream << "Min frame time: " << *min_element(m_stat->m_general_stat.values.begin(), m_stat->m_general_stat.values.end()) << Qt::endl;
      str_stream << "Max frame time: " << *max_element(m_stat->m_general_stat.values.begin(), m_stat->m_general_stat.values.end()) << Qt::endl;
    }

    get_stat_as_str(str_stream);
    m_stat_qlabel->setText(str);
  }
}


void PathDrawerQWidget::get_stat_as_str(QTextStream &str_stream)
{
  if (!m_stat)
  {
    return;
  }

  for (const auto& map_unit : m_stat->m_stat)
  {
    for (const auto& kv_pair: map_unit)
    {
      str_stream << kv_pair.first.c_str();

      if (kv_pair.second.compute_average)
      {
        if (kv_pair.second.compute_average_impl)
        {
          str_stream << kv_pair.second.compute_average_impl(kv_pair.second.values) << Qt::endl;
        }
        else if (kv_pair.second.values.size())
        {
          str_stream << accumulate(kv_pair.second.values.begin(),
                                   kv_pair.second.values.end(),
                                   decltype(kv_pair.second.values)::value_type(0)) / kv_pair.second.values.size();
          str_stream << Qt::endl;
        }
      }
      else
      {
        str_stream << " last: " << kv_pair.second.values.back() << Qt::endl;
      }
    }
  }
}



