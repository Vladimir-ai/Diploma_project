#include "fast_feature_detector.h"

fast_feature_detector::fast_feature_detector(int threshold,
                                             bool nonmaxSuppression,
                                             FastFeatureDetector::DetectorType type)
{
  m_detector = FastFeatureDetector::create(threshold, nonmaxSuppression, type);
}


bool fast_feature_detector::detect_features(cv::Mat frame, std::vector<Point2f> &points)
{
  vector<KeyPoint> kp;

  m_detector->detect(frame, kp);
  KeyPoint::convert(kp, points);
  return true;
}



fast_feature_detector::~fast_feature_detector()
{
}

void fast_feature_detector::reset()
{
  m_detector.reset();
}
