#ifndef PATH_ESTIMATOR
#define PATH_ESTIMATOR

#include <opencv2/core/mat.hpp>

using namespace cv;
using namespace std;

namespace submodule_pose_estimator
{
  class abstract_pose_estimator
  {
    public:
    virtual ~abstract_pose_estimator() = default;
    virtual Mat find_matrix(const vector<Point2f>& base_points, const vector<Point2f>& current_points) = 0;
  };
}
#endif /* PATH_ESTIMATOR */
