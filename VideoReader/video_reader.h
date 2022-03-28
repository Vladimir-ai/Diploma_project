#ifndef VIDEO_READER_H
#define VIDEO_READER_H

#include <opencv2/opencv.hpp>
#include <Eigen/Dense>

using namespace cv;

Mat read_next_frame(void);

namespace submodule_video_reader
{
  class abstract_video_reader
  {
  protected:
    uint64_t m_frame_num;
    Eigen::MatrixXi m_previous_frame;
    Eigen::MatrixXi m_current_frame;

  public:
    virtual ~abstract_video_reader() = default;

    virtual uint64_t get_current_frame_num(void) = 0;

    virtual Eigen::MatrixXi get_current_frame(void) = 0;
    virtual Eigen::MatrixXi get_previous_frame(void) = 0;

    virtual Eigen::MatrixXi read_frame_by_num(uint64_t num) = 0;
    virtual Eigen::MatrixXi read_next_frame(void) = 0;

    virtual bool is_finished(void) = 0;
    virtual bool is_started(void) = 0;
    virtual void reset(void) = 0;
  };
}

#endif // VIDEO_READER_H
