#ifndef FEATURE_DETECTOR_H
#define FEATURE_DETECTOR_H

#include <opencv2/core/mat.hpp>
#include <vector>
#include <Eigen/Dense>

using namespace cv;
using namespace std;

namespace submodule_feature_detector
{
  class abstract_feature_detector
  {
  public:
    virtual ~abstract_feature_detector() = default;

    virtual bool detect_features(Eigen::MatrixXi frame, vector<Point2f>& points) = 0;
  };
}

#endif // FEATURE_DETECTOR_H
