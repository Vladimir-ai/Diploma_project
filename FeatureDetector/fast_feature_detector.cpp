#include "fast_feature_detector.h"


OpencvFastFeatureDetector::OpencvFastFeatureDetector(Statistics *stat,
                                                     int threshold,
                                                     bool nonmaxSuppression,
                                                     FastFeatureDetector::DetectorType type): IAbstractFeatureDetector(stat)
{
  m_detector = FastFeatureDetector::create(threshold, nonmaxSuppression, type);
}

bool OpencvFastFeatureDetector::detect_features(Mat frame, std::vector<Point2f> &points)
{
  vector<KeyPoint> kp;
  const clock_t begin_time = clock();

  m_detector->detect(frame, kp);
  KeyPoint::convert(kp, points);

  m_stat->add_statistics(FEATURE_DETECTOR, "FAST Detector current time: ", (double) (clock() - begin_time) / CLOCKS_PER_SEC);
  m_stat->add_statistics(FEATURE_DETECTOR, "FAST Detector average time: ", (double) (clock() - begin_time) / CLOCKS_PER_SEC, true);

  return true;
}

OpencvFastFeatureDetector::~OpencvFastFeatureDetector()
{

}

void OpencvFastFeatureDetector::reset()
{
  m_detector.reset();
}

