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


void OpencvPoseEstimator::find_matrix(const std::vector<Point2f> &base_points, const std::vector<Point2f> &current_points, Mat &R, Mat &t)
{
  Mat essential_mat;
  R = Mat::eye(3, 3, CV_64F);
  t = Mat::zeros(3, 1, CV_64F);

  try{
    Mat essential_matrix(findEssentialMat(current_points, base_points, m_focal, m_pp, m_method, m_prob, m_threshold));
    (void) recoverPose(essential_matrix, current_points, base_points, R, t, m_focal, m_pp);
  }
  catch (cv::Exception &error) {
  }

}

