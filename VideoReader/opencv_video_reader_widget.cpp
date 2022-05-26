#include "opencv_video_reader_widget.h"
#include "open_cv_video_reader.h"

#include <QFileDialog>


OpenCvVideoReaderInfoWidget::OpenCvVideoReaderInfoWidget(
    Statistics *stat, AbstractLogger *logger, FuncErrorHandler err_handler): AbstractInfoQtFrame(stat, logger, err_handler)
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


SubmoduleFeatureDetector::IAbstractFeatureDetector *OpenCvVideoReaderInfoWidget::get_feature_detector()
{
  return nullptr;
}


SubmoduleFeatureTracker::IAbstractFeatureTracker *OpenCvVideoReaderInfoWidget::get_feature_tracker()
{
  return nullptr;
}


SubmoduleVideoReader::IAbstractVideoReader *OpenCvVideoReaderInfoWidget::get_video_reader()
{
  IAbstractVideoReader *result = nullptr;

  if (!m_path.empty())
  {
    result = new OpencvVideoReader(m_path);
  }
  else
  {
    m_error_handler("Path to video file isn't specified");
  }

  return result;
}


SubmodulePoseEstimator::IAbstractPoseEstimator *OpenCvVideoReaderInfoWidget::get_pose_estimator()
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





