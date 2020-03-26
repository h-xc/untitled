#include "inputservice.h"
#include <QDBusConnection>
#include <QDebug>

InputService::InputService(QObject *object)
{
    QDBusConnection connect = QDBusConnection::sessionBus();
    if (!connect.registerService("com.mh.input")) {
        qFatal("Unable to register at DBus");
        return;
    }
    qDebug() << "register servece" << endl;

    if (!connect.registerObject("/input/vkim", object,
        QDBusConnection::ExportAllSignals | QDBusConnection::ExportAllSlots))
    {
        qFatal("Unable to register object at DBus");
        return;
    }

    qDebug() << "register object" << endl;
}

InputService::~InputService()
{
    QDBusConnection connect = QDBusConnection::sessionBus();
    connect.unregisterObject("/input/vkim");
    qDebug() << "unregister object" << endl;

    connect.unregisterService("com.mh.input");
    qDebug() << "unregister servece" << endl;
}

