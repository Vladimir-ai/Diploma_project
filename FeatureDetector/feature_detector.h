#ifndef FEATURE_DETECTOR_H
#define FEATURE_DETECTOR_H

#include <opencv2/core/mat.hpp>
#include <vector>

using namespace cv;
using namespace std;

class abstract_feature_detector
{
public:
  virtual ~abstract_feature_detector() = default;

  virtual bool detect_features(Mat frame, vector<Point2f>& points) = 0;
};

#endif // FEATURE_DETECTOR_H
