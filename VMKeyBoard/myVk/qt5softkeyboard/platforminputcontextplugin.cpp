#include "platforminputcontextplugin.h"
#include "inputcontext.h"

QPlatformInputContext *PlatformInputContextPlugin::create(const QString &key, const QStringList &paramList)
{
    Q_UNUSED(paramList);

    if (key.compare(key, QStringLiteral("Qt5Input"), Qt::CaseInsensitive) == 0)
    {
        return new InputContext;
    }
    return NULL;
}
