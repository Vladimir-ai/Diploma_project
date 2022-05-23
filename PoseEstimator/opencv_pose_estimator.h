#ifndef OPENCV_PATH_ESTIMATOR_H
#define OPENCV_PATH_ESTIMATOR_H

#include "abstract_pose_estimator.h"
#include <opencv2/calib3d.hpp>

namespace submodule_pose_estimator
{
  class opencv_pose_estimator : public abstract_pose_estimator
  {
  private:
    double m_focal;
    Point2d m_pp;
    int m_method;
    double m_prob;
    double m_threshold;

  public:
    opencv_pose_estimator(double focal = 718.8560, Point2d pp = Point2d(607.1928, 185.2157), int method = RANSAC, double prob = 0.999, double threshold = 1.0);

    Mat find_matrix(const std::vector<Point2f> &base_points, const std::vector<Point2f> &current_points);
  };
}


#endif // OPENCV_PATH_ESTIMATOR_H
