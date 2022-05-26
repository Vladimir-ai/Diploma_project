#include "opencvfeaturetracker.h"
#include <opencv2/opencv.hpp>


void OpencvFeatureTracker::filter_values_out(std::vector<Point2f> &src_pts, std::vector<Point2f> &dst_pts, std::vector<uint8_t> &status)
{
  vector<uint8_t>::reverse_iterator iter;
  const auto status_size = status.size();

  for (iter = status.rbegin(); iter < status.rend(); iter++)
  {
    if (!*iter)
    {
      const auto offset = (iter - status.rbegin());
      src_pts.erase(src_pts.begin() + (status_size - offset - 1));
      dst_pts.erase(dst_pts.begin() + (status_size - offset - 1));
    }
  }
}


OpencvFeatureTracker::OpencvFeatureTracker(Statistics *stat, uint8_t window_width,
                                           uint8_t window_height,
                                           TermCriteria::Type criteria_type,
                                           int max_count,
                                           double epsilon): SubmoduleFeatureTracker::IAbstractFeatureTracker(stat)
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
  vector<uchar> status;
  vector<float> error;
  clock_t begin_time = clock();

  cv::calcOpticalFlowPyrLK(frame_src, frame_dst, v_src_points, v_dst_points, status, error, m_winsize, 3, *m_termcriteria);  

  m_stat->add_statistics(FEATURE_TRACKER, "Lucas-Kanade tracker current time: ", (double) (clock() - begin_time) / CLOCKS_PER_SEC);
  m_stat->add_statistics(FEATURE_TRACKER, "Lucas-Kanade tracker average time: ", (double) (clock() - begin_time) / CLOCKS_PER_SEC, true);

  filter_values_out(v_src_points, v_dst_points, status);

  return true;
}



void OpencvFeatureTracker::reset()
{
  /* Do nothing */
}

submodule_type OpencvFeatureTracker::get_type()
{
  return FEATURE_TRACKER_WITH_FRAME;
}
