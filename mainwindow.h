#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QHBoxLayout>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    QWidget *m_central_widget;
    QWidget *m_options_widget;

    QHBoxLayout *m_main_hbox_layout;
    QVBoxLayout *m_option_box_layout;

    QGraphicsScene *m_scene;
    QGraphicsView *m_view;
    QPixmap *m_pixmap;
    QGraphicsPixmapItem *m_item;

    void setup_layout(void);
    void setup_options_vbox_layout(void);

private slots:
  void handleButton1();
  void handleButton2();

};
#endif // MAINWINDOW_H
