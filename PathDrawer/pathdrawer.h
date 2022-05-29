#ifndef PATHDRAWER_H
#define PATHDRAWER_H

#include <opencv2/core/mat.hpp>

#include "Logger/logger.h"

#include <mutex>

using namespace cv;

class PathDrawer
{
private:
  static const int mc_width = 800, mc_height = 800;
  static constexpr double scale = 0.1;

  double m_x = 0, m_y = 0;

  Mat m_trajectory;
  AbstractLogger *m_logger;
  std::mutex m_mutex;
  Mat curr_t;
  Mat curr_R;

public:
  PathDrawer(AbstractLogger *logger);

  void set_logger(AbstractLogger *logger);
  void update_pos(Mat R, Mat t);
  Mat get_img(void);
  void reset(void);

  static int get_width(void);
  static int get_heigth(void);
};

#endif // PATHDRAWER_H
