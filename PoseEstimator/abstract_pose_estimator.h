#ifndef PATH_ESTIMATOR
#define PATH_ESTIMATOR

#include <opencv2/core/mat.hpp>

#include <Common/statistics.h>

using namespace cv;
using namespace std;

namespace SubmodulePoseEstimator
{
  class IAbstractPoseEstimator: public IStatisticsImpl
  {
  public:
    IAbstractPoseEstimator(Statistics *statistics): IStatisticsImpl(statistics){};

    virtual ~IAbstractPoseEstimator() = default;
    virtual void find_matrix(const vector<Point2f>& base_points, const vector<Point2f>& current_points, Mat &R, Mat &t) = 0;
  };
}
#endif /* PATH_ESTIMATOR */
