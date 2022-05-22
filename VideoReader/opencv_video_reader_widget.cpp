#include "opencv_video_reader_widget.h"
#include "open_cv_video_reader.h"

#include <QFileDialog>


OpenCvVideoReaderInfoWidget::OpenCvVideoReaderInfoWidget(func_error_handler err_handler): AbstractInfoQtFrame(err_handler)
{
  m_layout = new QVBoxLayout(this);
  m_path_button = new QPushButton("Select file");

  m_layout->addWidget(m_path_button);

  QPalette pal = QPalette();
  pal.setColor(QPalette::Window, Qt::lightGray);

  connect(m_path_button, &QPushButton::clicked, this, &OpenCvVideoReaderInfoWidget::call_video_selection);

  this->setAutoFillBackground(true);
  this->setPalette(pal);

  this->setFrameShape(Shape::Box);
  this->setLineWidth(1);
}


OpenCvVideoReaderInfoWidget::~OpenCvVideoReaderInfoWidget()
{
  if (m_path_button)
  {
    delete m_path_button;
  }

  if (m_layout)
  {
    delete m_layout;
  }
}


submodule_feature_detector::abstract_feature_detector *OpenCvVideoReaderInfoWidget::get_feature_detector()
{
  return nullptr;
}


submodule_feature_tracker::abstract_feature_tracker *OpenCvVideoReaderInfoWidget::get_feature_tracker()
{
  return nullptr;
}


submodule_video_reader::abstract_video_reader *OpenCvVideoReaderInfoWidget::get_video_reader()
{
  abstract_video_reader *result = nullptr;

  if (!m_path.empty())
  {
    result = new open_cv_video_reader(m_path);
  }
  else
  {
    m_error_handler("Path to video file isn't specified");
  }

  return result;
}


submodule_pose_estimator::abstract_pose_estimator *OpenCvVideoReaderInfoWidget::get_pose_estimator()
{
  return nullptr;
}


submodule_type OpenCvVideoReaderInfoWidget::get_type()
{
  return submodule_type::OPENCV_READER;
}


string OpenCvVideoReaderInfoWidget::get_name()
{
  return "Video file";
}


void OpenCvVideoReaderInfoWidget::call_video_selection()
{
  QString path = QFileDialog::getOpenFileName(this, "Select a video", ".", "Videos (*.mp4 *.avi)");

  if (QFileInfo::exists(path))
  {
    m_path = path.toStdString();
  }

}





