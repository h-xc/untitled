#ifndef PLATFORMINPUTCONTEXTPLUGIN_H
#define PLATFORMINPUTCONTEXTPLUGIN_H

#include "virtualkeyboard_global.h"
#include <qpa/qplatforminputcontextplugin_p.h>

class VIRTUALKEYBOARDSHARED_EXPORT PlatformInputContextPlugin: public QPlatformInputContextPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID QPlatformInputContextFactoryInterface_iid FILE "Qt5Input.json")

public:
    QPlatformInputContext *create(const QString& key, const QStringList &paramList);
};

#endif // PLATFORMINPUTCONTEXTPLUGIN_H
