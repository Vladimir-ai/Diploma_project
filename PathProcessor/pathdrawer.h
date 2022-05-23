#ifndef PATHDRAWER_H
#define PATHDRAWER_H

#include <opencv2/core/mat.hpp>

#include <Logger/logger.h>

#include <mutex>

using namespace cv;

class PathDrawer
{
private:
  static const int mc_width = 400, mc_height = 400;
  double m_x = mc_width / 2, m_y = mc_height / 2;
  Mat m_trajectory;
  abstract_logger *m_logger;
  std::mutex m_mutex;

public:
  PathDrawer(abstract_logger *logger);

  void set_logger(abstract_logger *logger);
  void update_pos(Mat position_update);
  Mat get_img();

  static int get_width(void);
  static int get_heigth(void);
};

#endif // PATHDRAWER_H
