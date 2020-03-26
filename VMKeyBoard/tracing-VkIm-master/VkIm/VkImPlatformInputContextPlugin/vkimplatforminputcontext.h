#ifndef VKIMPLATFORMINPUTCONTEXT_H
#define VKIMPLATFORMINPUTCONTEXT_H

#include <qpa/qplatforminputcontext.h>

class QDBusInterface;

enum VkImQuery
{
    VKIM_REGEXP = 1,
    VKIM_END
};

class VkImPlatformInputContext : public QPlatformInputContext
{
    Q_OBJECT
public:
    VkImPlatformInputContext();
    ~VkImPlatformInputContext();

public:
    bool isValid() const Q_DECL_OVERRIDE;
    void setFocusObject(QObject *object) Q_DECL_OVERRIDE;

    void showInputPanel() Q_DECL_OVERRIDE;
    void hideInputPanel() Q_DECL_OVERRIDE;
    bool isInputPanelVisible() const Q_DECL_OVERRIDE;
    bool filterEvent(const QEvent *event) Q_DECL_OVERRIDE;

public slots:
    void keyboardCommit(QString str);

private:
    QObject *focusObject;
    QDBusInterface *dbusInterface;
};

#endif // VKIMPLATFORMINPUTCONTEXT_H
