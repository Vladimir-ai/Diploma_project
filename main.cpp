#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    std::string lib_path = "";
    QApplication a(argc, argv);

    /* ignore other values */
    if (argc > 2 && std::string(argv[1]) == "-l")
    {
      lib_path = argv[2];
    }

    MainWindow w(nullptr, lib_path);
    w.show();
    return a.exec();
}
