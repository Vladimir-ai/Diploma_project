#ifndef FEATURE_TRACKER_H
#define FEATURE_TRACKER_H

#include <opencv2/core/mat.hpp>
#include <vector>

using namespace cv;
using namespace std;

class abstract_feature_tracker
{
public:
  virtual ~abstract_feature_tracker() = default;

  virtual bool track_features(vector<Point2f>& v_src_points, vector<Point2f>& v_dst_points) = 0;
  virtual bool track_features(Mat& frame_src, Mat &frame_dst,
                              vector<Point2f>& v_src_points, vector<Point2f>& v_dst_points) = 0;
};

#endif // FEATURE_TRACKER_H