#ifndef FAST_FEATURE_DETECTOR_H
#define FAST_FEATURE_DETECTOR_H

#include <opencv2/features2d.hpp>
#include "feature_detector.h"

using namespace SubmoduleFeatureDetector;

class OpencvFastFeatureDetector : public IAbstractFeatureDetector
{
private:
  Ptr<FastFeatureDetector> m_detector;

public:
  OpencvFastFeatureDetector(Statistics *stat = nullptr, int threshold=10,
                        bool nonmaxSuppression=false,
                        FastFeatureDetector::DetectorType type=FastFeatureDetector::TYPE_9_16);

  bool detect_features(cv::Mat frame, std::vector<Point2f> &points);
  ~OpencvFastFeatureDetector();

  void reset();
};

#endif // FAST_FEATURE_DETECTOR_H
