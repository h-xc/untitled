#include "vkimplatforminputcontextplugin.h"
#include <QDebug>


VkImPlatformInputContext* VkImPlatformInputContextPlugin::create(const QString &key, const QStringList &paramList)
{
    if (key == QLatin1String("vkim")) {
        qDebug() << "vkim input context plugin created" << endl;
        return new VkImPlatformInputContext;
    }

    return NULL;
}

