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


OpencvPoseEstimatorWidget::OpencvPoseEstimatorWidget(Statistics *stat, AbstractLogger *logger, FuncErrorHandler err_handler): AbstractInfoQtFrame(stat, logger, err_handler)
{
//  float cam_mat[9] = { 7.0709e+02, 0, 6.018873000000e+02, 0, 7.070912000000e+02, 1.831104000000e+02, 0, 0, 1 };
//  float dist_coefs[5] = { -3.644661e-01, 1.790019e-01, 1.148107e-03, -6.298563e-04, -5.314062e-02 };
//  float dist_coefs[5] = { 1, 1, 1, 1, 1 };
  float cam_mat[9] = { 9.591977e+02, 0, 9.529324e+02, 0, 6.944383e+02, 2.416793e+02, 0, 0, 1 };
  float dist_coefs[5] = { -3.725637e-01, 1.979803e-01, 1.799970e-04, 1.250593e-03, -6.608481e-02 };

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

  m_cam_matrix = Mat(3, 3, CV_32F, cam_mat).clone();
  m_dist_coefs = Mat(1, 5, CV_32F, dist_coefs).clone();
}


SubmoduleFeatureDetector::IAbstractFeatureDetector *OpencvPoseEstimatorWidget::get_feature_detector()
{
  return nullptr;
}


SubmoduleFeatureTracker::IAbstractFeatureTracker *OpencvPoseEstimatorWidget::get_feature_tracker()
{
  return nullptr;
}


SubmoduleVideoReader::IAbstractVideoReader *OpencvPoseEstimatorWidget::get_video_reader()
{
  return nullptr;
}


SubmodulePoseEstimator::IAbstractPoseEstimator *OpencvPoseEstimatorWidget::get_pose_estimator()
{
  SubmodulePoseEstimator::OpencvPoseEstimator *estimator = new SubmodulePoseEstimator::OpencvPoseEstimator(m_stat);
  estimator->m_cam_matrix = m_cam_matrix.clone();
  estimator->m_distortion_coef = m_dist_coefs.clone();
  estimator->m_cam_mat_inv = m_cam_matrix.clone().inv();
  return estimator;
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

    m_cam_matrix = cam_mat;
    m_dist_coefs = dist_coefs;
  }
}

