#include "dialog.h"
#include <QApplication>
#include <QFile>
#include "inputservice.h"
#include <QDBusConnection>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QFile file(":/keyboard.css");

    if (file.open(QFile::ReadOnly))
    {
        QString css = QLatin1String(file.readAll());
        a.setStyleSheet(css);
    }
    file.close();

    Dialog keyboard;
    InputService inputService(&keyboard);


    return a.exec();
}
