#ifndef OPENCV_PATH_ESTIMATOR_H
#define OPENCV_PATH_ESTIMATOR_H

#include <opencv2/calib3d.hpp>
#include "abstract_pose_estimator.h"

namespace SubmodulePoseEstimator
{
  class OpencvPoseEstimator : public IAbstractPoseEstimator
  {
  private:
    double m_focal;
    Point2d m_pp;
    int m_method;
    double m_prob;
    double m_threshold;

  public:
    OpencvPoseEstimator(Statistics *statistics, double focal = 718.8560,
                        Point2d pp = Point2d(607.1928, 185.2157),
                        int method = RANSAC, double prob = 0.999,
                        double threshold = 1.0);

    void find_matrix(const std::vector<Point2f> &base_points, const std::vector<Point2f> &current_points, Mat &R, Mat &t);
  };
}


#endif // OPENCV_PATH_ESTIMATOR_H
