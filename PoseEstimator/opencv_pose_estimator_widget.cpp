#include "opencv_pose_estimator.h"
#include "opencv_pose_estimator_widget.h"

#include <QFileDialog>
#include <filesystem>
#include <string>


inline std::vector<Point3f> OpencvPoseEstimatorWidget::init_world_coordinates(const int rows, const int cols)
{
  std::vector<cv::Point3f> objp;
  for(int column = 0; column < cols; column++)
  {
    for(int row = 0; row < rows; row++)
      objp.push_back(cv::Point3f(row, column,0));
  }

  return objp;
}


OpencvPoseEstimatorWidget::OpencvPoseEstimatorWidget(abstract_logger *logger, func_error_handler err_handler): AbstractInfoQtFrame(logger, err_handler)
{
  m_layout = new QVBoxLayout(this);
  m_camera_matrix_push_button = new QPushButton("Cam calibration data");

  m_layout->addWidget(m_camera_matrix_push_button);

  QPalette pal = QPalette();
  pal.setColor(QPalette::Window, Qt::lightGray);

  connect(m_camera_matrix_push_button, &QPushButton::clicked,
          this, &OpencvPoseEstimatorWidget::call_camera_calib_data);

  this->setAutoFillBackground(true);
  this->setPalette(pal);

  this->setFrameShape(Shape::Box);
  this->setLineWidth(1);
}


submodule_feature_detector::abstract_feature_detector *OpencvPoseEstimatorWidget::get_feature_detector()
{
  return nullptr;
}


submodule_feature_tracker::abstract_feature_tracker *OpencvPoseEstimatorWidget::get_feature_tracker()
{
  return nullptr;
}


submodule_video_reader::abstract_video_reader *OpencvPoseEstimatorWidget::get_video_reader()
{
  return nullptr;
}


submodule_pose_estimator::abstract_pose_estimator *OpencvPoseEstimatorWidget::get_pose_estimator()
{
  return new submodule_pose_estimator::opencv_pose_estimator();
}


submodule_type OpencvPoseEstimatorWidget::get_type()
{
  return OPENCV_POSE_ESTIMATOR;
}


string OpencvPoseEstimatorWidget::get_name()
{
  return "Opencv Pose Estimator";
}


void OpencvPoseEstimatorWidget::call_camera_calib_data()
{
  QString path = QFileDialog::getExistingDirectory(this, "Select calibration data", ".");
  const int rows = 6, cols = 9;
  Mat img;
  bool success;
  std::vector<cv::Point2f> corner_pts;
  std::vector<std::vector<cv::Point3f>> objpoints;
  std::vector<std::vector<cv::Point2f>> imgpoints;
  TermCriteria criteria(TermCriteria::MAX_ITER | TermCriteria::EPS, 30, 0.001);
  std::vector<cv::Point3f> world_pts = init_world_coordinates(rows, cols);

  std::string log_string;


  if (QFileInfo::exists(path))
  {
    Mat cam_mat, dist_coefs, rotate, translate;

    for (const auto & entry : std::filesystem::directory_iterator(path.toStdString()))
    {
      if ((img = imread(entry.path(), IMREAD_GRAYSCALE)).data != NULL)
      {
        success = findChessboardCorners(img, Size(rows, cols), corner_pts,
                                        CALIB_CB_ADAPTIVE_THRESH | CALIB_CB_FAST_CHECK | CALIB_CB_NORMALIZE_IMAGE);
        if (success)
        {
          // refining pixel coordinates for given 2d points.
          cornerSubPix(img, corner_pts, cv::Size(11,11), cv::Size(-1,-1),criteria);
          // Displaying the detected corner points on the checker board
          drawChessboardCorners(img, cv::Size(rows, cols), corner_pts, success);
          objpoints.push_back(world_pts);
          imgpoints.push_back(corner_pts);
        }

        imshow("Image", img);
      }
    }
    destroyAllWindows();

    calibrateCamera(objpoints, imgpoints,
                    cv::Size(img.rows, img.cols),
                    cam_mat, dist_coefs,
                    rotate, translate);

    log_string = "rotate_matrix: ";
    log_string << rotate;
    LOG_INFO(m_logger, log_string);

    log_string = "Translate_matrix: ";
    log_string << translate;
    LOG_INFO(m_logger, log_string);

    log_string = "Distortion coefs: ";
    log_string << dist_coefs;
    LOG_INFO(m_logger, log_string);

    log_string = "Camera matrix: ";
    log_string << cam_mat;
    LOG_INFO(m_logger, log_string);
  }
}

