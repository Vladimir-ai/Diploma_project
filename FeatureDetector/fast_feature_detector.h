#ifndef FAST_FEATURE_DETECTOR_H
#define FAST_FEATURE_DETECTOR_H

#include "feature_detector.h"
#include <opencv2/features2d.hpp>

using namespace submodule_feature_detector;

class fast_feature_detector : abstract_feature_detector
{
private:
  Ptr<FastFeatureDetector> m_detector;

public:
  fast_feature_detector(int threshold=10,
                        bool nonmaxSuppression=true,
                        FastFeatureDetector::DetectorType type=FastFeatureDetector::TYPE_9_16);

  // abstract_feature_detector interface
public:
  bool detect_features(Eigen::MatrixXi frame, std::vector<Point2f> &points);
  ~fast_feature_detector();
};

#endif // FAST_FEATURE_DETECTOR_H