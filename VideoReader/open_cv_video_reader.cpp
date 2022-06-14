#include "open_cv_video_reader.h"
#include <opencv2/videoio/videoio_c.h>
#include <opencv2/core/eigen.hpp>
#include "Common/common.h"


OpencvVideoReader::OpencvVideoReader(std::string path_to_video)
{
  m_cap = new VideoCapture(path_to_video);
  m_frame_count = m_cap->get(CV_CAP_PROP_FRAME_COUNT);
  m_frame_num = 0;
}


OpencvVideoReader::~OpencvVideoReader()
{
  if (m_cap->isOpened())
  {
    m_cap->release();
  }

  delete m_cap;
}


uint64_t OpencvVideoReader::get_current_frame_num()
{
  return m_cap->get(CV_CAP_PROP_POS_FRAMES);
}


void OpencvVideoReader::set_logger(AbstractLogger *logger)
{
  m_logger = logger;
}


Mat OpencvVideoReader::get_current_frame()
{
  return m_current_frame;
}


Mat OpencvVideoReader::get_previous_frame()
{
  return m_previous_frame;
}


Mat OpencvVideoReader::read_frame_by_num(uint64_t num)
{
  Mat result;

  m_frame_num = m_cap->get(CV_CAP_PROP_POS_FRAMES);
  m_cap->set(CV_CAP_PROP_POS_FRAMES, num);

  *m_cap >> result;

  m_cap->set(CV_CAP_PROP_POS_FRAMES, m_frame_num);
  return result;
}


Mat OpencvVideoReader::read_next_frame()
{
  Mat tmp_img;
  m_previous_frame = m_current_frame;
  if (!is_finished())
  {
    *m_cap >> tmp_img;
    m_frame_num++;
  }

  return tmp_img;
}


bool OpencvVideoReader::is_finished()
{
  return (m_frame_count - 1) < m_frame_num;
}


bool OpencvVideoReader::is_started()
{
  return m_cap->isOpened();
}


void OpencvVideoReader::reset()
{
  ASSERT(m_cap);
  m_cap->set(CAP_PROP_POS_FRAMES, 0);
}


void OpencvVideoReader::stop()
{
  ASSERT(m_cap);
  m_cap->release();
}
