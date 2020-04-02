#ifndef VKIMPLATFORMINPUTCONTEXTPLUGIN_H
#define VKIMPLATFORMINPUTCONTEXTPLUGIN_H

#include <qpa/qplatforminputcontextplugin_p.h>
#include "vkimplatforminputcontext.h"

class VkImPlatformInputContextPlugin : public QPlatformInputContextPlugin
{
    Q_OBJECT
#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID QPlatformInputContextFactoryInterface_iid FILE "vkim.json")
#endif // QT_VERSION >= 0x050000

public:
    virtual VkImPlatformInputContext *create(const QString &key, const QStringList &paramList);
};

#endif // VKIMPLATFORMINPUTCONTEXTPLUGIN_H
