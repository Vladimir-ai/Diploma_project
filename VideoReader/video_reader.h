#ifndef VIDEO_READER_H
#define VIDEO_READER_H

#include <opencv2/opencv.hpp>
#include <Eigen/Dense>
#include <Logger/logger.h>

using namespace cv;

Mat read_next_frame(void);

namespace submodule_video_reader
{
  class abstract_video_reader
  {
  protected:
    uint64_t m_frame_num;
    Mat m_previous_frame;
    Mat m_current_frame;

    abstract_logger *m_logger;

  public:
    virtual ~abstract_video_reader() = default;

    virtual void set_logger(abstract_logger *logger) = 0;

    virtual uint64_t get_current_frame_num(void) = 0;

    virtual Mat get_current_frame(void) = 0;
    virtual Mat get_previous_frame(void) = 0;

    virtual Mat read_frame_by_num(uint64_t num) = 0;
    virtual Mat read_next_frame(void) = 0;

    virtual bool is_finished(void) = 0;
    virtual bool is_started(void) = 0;

    virtual void stop(void) = 0;
    virtual void reset(void) = 0;
  };
}

#endif // VIDEO_READER_H
