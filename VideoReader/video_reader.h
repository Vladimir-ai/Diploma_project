#ifndef VIDEO_READER_H
#define VIDEO_READER_H

#include <opencv2/opencv.hpp>
#include <Eigen/Dense>

using namespace cv;

Mat read_next_frame(void);

class abstract_video_reader
{
protected:
  uint64_t m_frame_num;
  Eigen::MatrixXi m_previous_frame;
  Eigen::MatrixXi m_current_frame;

public:
  virtual ~abstract_video_reader() = default;

  virtual uint64_t get_current_frame(void) = 0;
  virtual Eigen::MatrixXi read_current_frame(void) = 0;
  virtual Eigen::MatrixXi read_previous_frame(void) = 0;
  virtual Eigen::MatrixXi read_frame_by_num(void) = 0;
};


#endif // VIDEO_READER_H
