#ifndef FEATURE_DETECTOR_H
#define FEATURE_DETECTOR_H

#include "Common/statistics.h"
#include <opencv2/core/mat.hpp>
#include <vector>

using namespace cv;
using namespace std;

namespace SubmoduleFeatureDetector
{
  class IAbstractFeatureDetector: public IStatisticsImpl
  {
  public:
    IAbstractFeatureDetector(Statistics *stat): IStatisticsImpl(stat){};
    virtual ~IAbstractFeatureDetector() = default;

    virtual bool detect_features(cv::Mat frame, vector<Point2f>& points) = 0;

    virtual void reset() = 0;
  };
}

#endif // FEATURE_DETECTOR_H
