#include "opencvfeaturetracker.h"
#include <opencv2/opencv.hpp>


OpencvFeatureTracker::OpencvFeatureTracker(uint8_t window_width, uint8_t window_height,
                                           TermCriteria::Type criteria_type, int max_count, double epsilon)
{
  m_termcriteria = new TermCriteria(criteria_type, max_count, epsilon);
  m_winsize = Size(window_width, window_height);
}

OpencvFeatureTracker::~OpencvFeatureTracker()
{
  delete m_termcriteria;
}


bool OpencvFeatureTracker::track_features(std::vector<Point2f> &v_src_points, std::vector<Point2f> &v_dst_points)
{
  (void) v_src_points;
  (void) v_dst_points;
  return false;
}


bool OpencvFeatureTracker::track_features(Mat &frame_src, Mat &frame_dst, std::vector<Point2f> &v_src_points, std::vector<Point2f> &v_dst_points)
{
  vector<uchar> arr;
  vector<float> error;
  cv::calcOpticalFlowPyrLK(frame_src, frame_dst, v_src_points, v_dst_points, arr, error, m_winsize, 3, *m_termcriteria);
  return true;
}



void OpencvFeatureTracker::reset()
{
  /* Do nothing */
}

submodule_type OpencvFeatureTracker::get_type()
{
  return OPENCV_FEATURE_TRACKER;
}
