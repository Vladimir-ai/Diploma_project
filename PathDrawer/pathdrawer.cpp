#include "pathdrawer.h"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"


PathDrawer::PathDrawer(AbstractLogger *logger)
{
  m_logger = logger;
  m_trajectory = Mat(mc_width, mc_height, CV_8UC3);

  curr_t = Mat::zeros(3, 1, CV_64F);
  curr_R = Mat::eye(3, 3, CV_64F);
}


void PathDrawer::set_logger(AbstractLogger *logger)
{
  m_logger = logger;
}


void PathDrawer::update_pos(Mat R, Mat t)
{
  std::stringstream text_str;
  text_str.precision(3);


//  m_x += position_update.at<double>(0);
//  m_y += position_update.at<double>(2);

  curr_t += scale * (curr_R * t);
  curr_R = R * curr_R;

//  if (curr_t.at<double>(0) >= 0.1 && curr_t.at<double>(1) >= 0.1)
//  {
    m_x += curr_t.at<double>(0);
    m_y += curr_t.at<double>(1);
//  }

  m_mutex.lock();

  rectangle(m_trajectory, cv::Point(0, 0), cv::Point(mc_width, 40), CV_RGB(0, 0, 0), FILLED);
  circle(m_trajectory, cv::Point((int) (m_x) + mc_width / 2, (int) (m_y) + mc_height / 2), 1, CV_RGB(255, 0, 0), 3, LINE_AA);

  text_str << "x = " << m_x << ", y = " << m_y;

  putText(m_trajectory, text_str.str(), cv::Point(20, 30), FONT_HERSHEY_COMPLEX, 1, cv::Scalar::all(255), 1, LINE_AA);

  m_mutex.unlock();
}


Mat PathDrawer::get_img()
{
  Mat trajectory_copy;

  m_mutex.lock();
  trajectory_copy = m_trajectory.clone();
  m_mutex.unlock();

  return trajectory_copy;
}


void PathDrawer::reset()
{
  rectangle(m_trajectory, cv::Point(0, 0), cv::Point(mc_width, mc_height), CV_RGB(0, 0, 0), FILLED);
}


int PathDrawer::get_width()
{
  return mc_width;
}


int PathDrawer::get_heigth()
{
  return mc_height;
}



