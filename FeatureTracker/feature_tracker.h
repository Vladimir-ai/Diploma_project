#ifndef FEATURE_TRACKER_H
#define FEATURE_TRACKER_H

#include "Common/common.h"
#include "Common/statistics.h"
#include <opencv2/core/mat.hpp>
#include <vector>

using namespace cv;
using namespace std;

namespace SubmoduleFeatureTracker
{
  class IAbstractFeatureTracker: public IStatisticsImpl
  {
  public:
    IAbstractFeatureTracker(Statistics *stat): IStatisticsImpl(stat){};
    virtual ~IAbstractFeatureTracker() = default;

    virtual bool track_features(vector<Point2f>& v_src_points, vector<Point2f>& v_dst_points) = 0;
    virtual bool track_features(Mat& frame_src, Mat &frame_dst,
                                vector<Point2f>& v_src_points, vector<Point2f>& v_dst_points) = 0;

    virtual void reset(void) = 0;

    virtual submodule_type get_type(void) = 0;
  };
}

#endif // FEATURE_TRACKER_H
