#ifndef OPENCVFEATURETRACKER_H
#define OPENCVFEATURETRACKER_H

#include "feature_tracker.h"



class OpencvFeatureTracker : public submodule_feature_tracker::abstract_feature_tracker
{
private:
  TermCriteria *m_termcriteria;
  Size m_winsize;
public:
  OpencvFeatureTracker(uint8_t window_width = 21, uint8_t window_height = 21,
                       TermCriteria::Type criteria_type = TermCriteria::Type(TermCriteria::Type::COUNT | TermCriteria::Type::EPS), int max_count = 30, double epsilon = 0.01);
  ~OpencvFeatureTracker();

  // abstract_feature_tracker interface
  bool track_features(std::vector<Point2f> &v_src_points, std::vector<Point2f> &v_dst_points);
  bool track_features(Mat &frame_src, Mat &frame_dst, std::vector<Point2f> &v_src_points, std::vector<Point2f> &v_dst_points);
  void reset();
  submodule_type get_type(void);
};

#endif // OPENCVFEATURETRACKER_H
