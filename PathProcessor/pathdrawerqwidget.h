#ifndef PATHDRAWERQWIDGET_H
#define PATHDRAWERQWIDGET_H

#include "Common/statistics.h"
#include "pathdrawer.h"

#include <QLabel>
#include <QTextStream>
#include <QTimer>
#include <QWidget>

class PathDrawerQWidget : public QWidget
{
private:
  const int mc_timer_timeout = 1000 / 24;

  QTimer *m_timer;
  PathDrawer *m_path_drawer = nullptr;
  Statistics *m_stat = nullptr;

  QLabel *m_stat_qlabel;
  QLabel *m_image_qlabel;

  void timer_timeout();
  void get_stat_as_str(QTextStream &str_stream);

public:
  PathDrawerQWidget(QWidget *parent = nullptr);
  PathDrawerQWidget(PathDrawer *path_drawer, Statistics *stat);
  ~PathDrawerQWidget(void);

  void set_path_drawer(PathDrawer *path_drawer);
  PathDrawer *get_path_drawer(void);

  void set_stat(Statistics *stat);

private slots:
  void handle_save_img_btn();

public slots:
  void show();
  void hide();
};

#endif // PATHDRAWERQWIDGET_H
