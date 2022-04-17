#ifndef FAST_FEATURE_DETECTOR_H
#define FAST_FEATURE_DETECTOR_H

#include "feature_detector.h"
#include <opencv2/features2d.hpp>

using namespace submodule_feature_detector;

class fast_feature_detector : public abstract_feature_detector
{
private:
  Ptr<FastFeatureDetector> m_detector;

public:
  fast_feature_detector(int threshold=100,
                        bool nonmaxSuppression=false,
                        FastFeatureDetector::DetectorType type=FastFeatureDetector::TYPE_9_16);

  bool detect_features(cv::Mat frame, std::vector<Point2f> &points);
  ~fast_feature_detector();

  void reset();
};

#endif // FAST_FEATURE_DETECTOR_H
