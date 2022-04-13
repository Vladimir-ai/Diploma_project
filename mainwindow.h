#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QTimer>
#include <QComboBox>

#include "PathProcessor/path_processor.h"
#include "Logger/logger.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

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

    QPushButton *m_select_file_btn = nullptr;

    QLabel *m_image_qlabel = nullptr;

    QComboBox *m_running_type_selector;

    QString m_path_to_video_file = "/home/krevedko/Desktop/14/image_0/res.mp4";

    module_path_processor::PathProcessor *m_path_processor = nullptr;

protected:
    void setup_layout(void);
    void setup_options_vbox_layout(void);
    void setup_timer(void);

private slots:
  void handle_start_button();
  void handle_stop_button();
  void handle_continue_button();
  void handle_pause_button();

  void handle_running_mode_change(int new_value);

  void timer_timeout();
  void call_video_selection();

};
#endif // MAINWINDOW_H
