#include "opencv_pose_estimator.h"

#include <opencv2/calib3d.hpp>

using namespace submodule_pose_estimator;


opencv_pose_estimator::opencv_pose_estimator(double focal, Point2d pp, int method, double prob, double threshold)
{
  m_focal = focal;
  m_pp = pp;
  m_method = method;
  m_prob = prob;
  m_threshold = threshold;
}


Mat opencv_pose_estimator::find_matrix(const std::vector<Point2f> &base_points, const std::vector<Point2f> &current_points)
{
  Mat rotate_mat, translation_mat;

  Mat essential_matrix = findEssentialMat(base_points, current_points, m_focal, m_pp, m_method, m_prob, m_threshold);
  (void) recoverPose(essential_matrix, base_points, current_points, rotate_mat, translation_mat);
  return rotate_mat * translation_mat;
}

