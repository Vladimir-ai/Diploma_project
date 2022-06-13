#include "opencv_pose_estimator.h"

#include <opencv2/calib3d.hpp>

using namespace SubmodulePoseEstimator;


OpencvPoseEstimator::OpencvPoseEstimator(Statistics *statistics, double focal, Point2d pp, int method, double prob, double threshold): IAbstractPoseEstimator(statistics)
{
  m_focal = focal;
  m_pp = pp;
  m_method = method;
  m_prob = prob;
  m_threshold = threshold;
}


void OpencvPoseEstimator::unproject_points(const vector<Point2f> &input, vector<Point2f> &output)
{
  Mat three_dim(input.size(), 3, CV_32F);
  Mat transposed;
  Mat result;

  vector<float> vec;
  for (int idx = 0; idx < 9; idx++)
  {
    vec.push_back(m_cam_matrix.at<float>(idx / 3, idx % 3));
  }

  output.clear();

  for(size_t idx = 0; idx < input.size(); idx++)
  {
   three_dim.at<float>(idx, 0) = input[idx].x;
   three_dim.at<float>(idx, 1) = input[idx].y;
   three_dim.at<float>(idx, 2) = 1.0;
  }

  transpose(three_dim, transposed);
  result = m_cam_mat_inv *transposed;

  for (int col = 0; col < result.cols; col++) {
    output.push_back(Point2f(result.at<float>(0, col), result.at<float>(1, col)));
  }
}


void OpencvPoseEstimator::find_matrix(const std::vector<Point2f> &base_points, const std::vector<Point2f> &current_points, Mat &R, Mat &t)
{
  Mat essential_mat;
  const clock_t begin_time = clock();

  R = Mat::eye(3, 3, CV_64F);
  t = Mat::zeros(3, 1, CV_64F);

  try{
    vector<Point2f> undistorted_base_pts;
    vector<Point2f> undistorted_curr_pts;
    vector<Point2f> uprojected_base_pts;
    vector<Point2f> uprojected_curr_pts;

    undistortPoints(base_points, undistorted_base_pts, m_cam_matrix, m_distortion_coef);
    undistortPoints(current_points, undistorted_curr_pts, m_cam_matrix, m_distortion_coef);

    unproject_points(undistorted_base_pts, uprojected_base_pts);
    unproject_points(undistorted_curr_pts, uprojected_curr_pts);

    Mat essential_matrix(findEssentialMat(uprojected_curr_pts, uprojected_base_pts));

    (void) recoverPose(essential_matrix, uprojected_curr_pts, uprojected_base_pts, R, t);
  }
  catch (cv::Exception &error) {
  }

  m_stat->add_statistics(POSE_ESTIMATOR, "Pose estimator last time: ", double(clock() - begin_time) / CLOCKS_PER_SEC);
  m_stat->add_statistics(POSE_ESTIMATOR, "Pose estimator average time: ", double(clock() - begin_time) / CLOCKS_PER_SEC, true);
}

