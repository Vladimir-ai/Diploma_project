#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QTimer>
#include <QComboBox>

#include "Common/qt_gui_properties.h"
#include "PathProcessor/path_processor.h"
#include "Logger/logger.h"
#include "PathProcessor/pathdrawerqwidget.h"


class PathDrawerQWidget;
class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

  static void error_handler(std::string error);

private:
  const int mc_timer_timeout = 1000 / 24;
  abstract_logger *m_logger = nullptr;

  QTimer *m_timer = nullptr;
  QWidget *m_central_widget = nullptr;
  QWidget *m_options_widget = nullptr;

  QHBoxLayout *m_main_hbox_layout = nullptr;
  QVBoxLayout *m_option_box_layout = nullptr;

  QHBoxLayout *m_stop_pause_layout = nullptr;
  QVBoxLayout *m_running_type_layout = nullptr;

  QPushButton *m_start_btn = nullptr;
  QPushButton *m_stop_btn = nullptr;
  QPushButton *m_pause_btn = nullptr;
  QPushButton *m_continue_btn = nullptr;

  QLabel *m_image_qlabel = nullptr;

  QComboBox *m_running_type_selector;

  module_path_processor::PathProcessor *m_path_processor = nullptr;

  AbstractInfoQtFrame *m_detector_info;
  AbstractInfoQtFrame *m_reader_info;
  AbstractInfoQtFrame *m_tracker_info;
  AbstractInfoQtFrame *m_pose_estimator_info;

  QComboBox *m_feature_detector_box;
  QComboBox *m_video_reader_box;
  QComboBox *m_feature_tracker_box;
  QComboBox *m_pose_estimator_box;

  QVBoxLayout *m_feature_detector_layout;
  QVBoxLayout *m_video_reader_layout;
  QVBoxLayout *m_feature_tracker_layout;
  QVBoxLayout *m_pose_estimator_layout;

  PathDrawerQWidget *m_drawer_widget;

  inline void init_video_reader_layout(void);
  inline void init_feature_detector_layout(void);
  inline void init_feature_tracker_layout(void);
  inline void init_pose_estimator_layout(void);

protected:
  void setup_layout(void);
  void setup_options_vbox_layout(void);
  void setup_timer(void);

private slots:
  void handle_start_button();
  void handle_stop_button();
  void handle_continue_button();
  void handle_pause_button();

  void handle_reader_type_change(int new_value);
  void handle_detector_type_change(int new_value);
  void handle_tracker_type_change(int new_value);
  void handle_pose_estimator_type_change(int new_value);

  void handle_running_mode_change(int new_value);

  void timer_timeout();

public slots:
  void closeEvent(QCloseEvent *event);
};
#endif // MAINWINDOW_H
