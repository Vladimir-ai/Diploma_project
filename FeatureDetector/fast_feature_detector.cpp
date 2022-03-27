#include "fast_feature_detector.h"
#include <opencv2/core/eigen.hpp>

fast_feature_detector::fast_feature_detector(int threshold,
                                             bool nonmaxSuppression,
                                             FastFeatureDetector::DetectorType type)
{
  m_detector = FastFeatureDetector::create(threshold, nonmaxSuppression, type);
}


bool fast_feature_detector::detect_features(Eigen::MatrixXi frame, std::vector<Point2f> &points)
{
  vector<KeyPoint> kp;
  Mat img;
  eigen2cv(frame, img);

  m_detector->detect(img, kp);
  return false;
}



fast_feature_detector::~fast_feature_detector()
{
  delete m_detector;
}
