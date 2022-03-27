#include "open_cv_video_reader.h"
#include <opencv2/videoio/videoio_c.h>
#include <opencv2/core/eigen.hpp>


// TODO: add checks to path in the outer part
open_cv_video_reader::open_cv_video_reader(std::string path_to_video)
{
  m_cap = new VideoCapture(path_to_video);
  m_frame_count = m_cap->get(CV_CAP_PROP_FRAME_COUNT);
}


open_cv_video_reader::~open_cv_video_reader()
{
  delete m_cap;
}


uint64_t open_cv_video_reader::get_current_frame_num()
{
  return m_cap->get(CV_CAP_PROP_POS_FRAMES);
}


Eigen::MatrixXi open_cv_video_reader::get_current_frame()
{
  return m_current_frame;
}


Eigen::MatrixXi open_cv_video_reader::get_previous_frame()
{
  return m_previous_frame;
}


Eigen::MatrixXi open_cv_video_reader::read_frame_by_num(uint64_t num)
{
  Mat tmp_img;
  Eigen::MatrixXi result;

  m_frame_num = m_cap->get(CV_CAP_PROP_POS_FRAMES);
  m_cap->set(CV_CAP_PROP_POS_FRAMES, num);

  *m_cap >> tmp_img;

  cv2eigen(tmp_img, result);

  m_cap->set(CV_CAP_PROP_POS_FRAMES, m_frame_num);
  return result;
}


Eigen::MatrixXi open_cv_video_reader::read_next_frame()
{
  Mat tmp_img;
  m_previous_frame = m_current_frame;

  *m_cap >> tmp_img;
  cv2eigen(tmp_img, m_current_frame);

  return m_current_frame;
}


bool open_cv_video_reader::is_finished()
{
  return m_frame_count < m_cap->get(CV_CAP_PROP_POS_FRAMES);
}


bool open_cv_video_reader::is_started()
{
  return m_cap->isOpened();
}

