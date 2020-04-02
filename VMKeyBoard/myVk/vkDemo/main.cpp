#include "mainwindow.h"
#include <QApplication>
#include <QDebug>
int main(int argc, char *argv[])
{
    //qputenv("QT_PLUGIN_PATH", "./plugins");
    //qputenv(QByteArray("QT_IM_MODULE"), QByteArray("Qt5Input"));

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
