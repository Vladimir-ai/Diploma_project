#ifndef PATHDRAWERQWIDGET_H
#define PATHDRAWERQWIDGET_H

#include "pathdrawer.h"

#include <QLabel>
#include <QTimer>
#include <QWidget>

class PathDrawerQWidget : public QWidget
{
private:
  const int mc_timer_timeout = 1000 / 24;

  QTimer *m_timer;
  PathDrawer *m_path_drawer = nullptr;
  QLabel *m_image_qlabel;

  void timer_timeout();
public:
  PathDrawerQWidget(QWidget *parent = nullptr);
  PathDrawerQWidget(PathDrawer *path_drawer);
  ~PathDrawerQWidget(void);

  void set_path_drawer(PathDrawer *path_drawer);
  PathDrawer *get_path_drawer(void);

private slots:
  void handle_save_img_btn();

public slots:
  void show();
  void hide();
};

#endif // PATHDRAWERQWIDGET_H
