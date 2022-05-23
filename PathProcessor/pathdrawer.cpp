#include "pathdrawer.h"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"


PathDrawer::PathDrawer(abstract_logger *logger)
{
  m_logger = logger;
  m_trajectory = Mat(mc_width, mc_height, CV_8UC3);
}


void PathDrawer::set_logger(abstract_logger *logger)
{
  m_logger = logger;
}


void PathDrawer::update_pos(Mat position_update)
{
  std::stringstream ss;
  ss.precision(3);
  m_x += (int) position_update.at<double>(0);
  m_y += (int) position_update.at<double>(1);

  m_mutex.lock();

  rectangle(m_trajectory, cv::Point(0, 30), cv::Point(mc_width, 0), CV_RGB(0, 0, 0), FILLED);
  circle(m_trajectory, cv::Point((int) m_x, (int) m_y), 1, CV_RGB(255, 0, 0), 3);

  ss << "x = " << m_x - mc_width / 2 << ", y = " << m_y - mc_height / 2;

  putText(m_trajectory, ss.str(), cv::Point(10, 20), FONT_HERSHEY_COMPLEX, 0.5, cv::Scalar::all(255));

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


int PathDrawer::get_width()
{
  return mc_width;
}


int PathDrawer::get_heigth()
{
  return mc_height;
}



