#include "vkimplatforminputcontext.h"
#include <QDebug>
#include <QWidget>
#include <QtDBus>
#include <QKeyEvent>
#include <QGuiApplication>


VkImPlatformInputContext::VkImPlatformInputContext() :
    focusObject(NULL)
{
    dbusInterface = new QDBusInterface("com.mh.input", "/input/vkim", "local.keyboard.Dialog",
                                       QDBusConnection::sessionBus(), this);

    connect(dbusInterface, SIGNAL(commit(QString)), SLOT(keyboardCommit(QString)));
}

VkImPlatformInputContext::~VkImPlatformInputContext()
{
    if (dbusInterface != NULL)
    {
        qDebug("delete interface");
        delete dbusInterface;
        dbusInterface = NULL;
    }
}

bool VkImPlatformInputContext::isValid() const
{
    return true;
}


void VkImPlatformInputContext::setFocusObject(QObject *object)
{
    focusObject = object;
    //qDebug() << "set focus object" << endl;
}

void VkImPlatformInputContext::showInputPanel()
{
    qDebug() << "show keyboard" << endl;
    if (dbusInterface != NULL)
    {
        QWidget *w = qobject_cast<QWidget*>(focusObject);
        QPoint pt = w->pos();
        QRect rect = w->rect();
        pt = w->mapToGlobal(QPoint(0, 0));
        int x = pt.x();
        int y = pt.y();
        dbusInterface->call("showKeyboard", pt, rect);
    }
    else
    {
        qDebug() << "interface is null" << endl;
    }
}

void VkImPlatformInputContext::hideInputPanel()
{
    qDebug() << "hide keyboard" << endl;
    if (dbusInterface != NULL)
    {
        dbusInterface->call("hideKeyboard");
    }
}

bool VkImPlatformInputContext::isInputPanelVisible() const
{
    if (dbusInterface == NULL)
    {
        return false;
    }

    if (!QDBusConnection::sessionBus().isConnected())
    {
        return false;
    }

    const QDBusReply<bool> reply = dbusInterface->call("isVisible");

    if (reply.isValid())
    {
        return reply.value();
    }
    return false;
}

bool VkImPlatformInputContext::filterEvent(const QEvent *event)
{
    const QKeyEvent *keyEvent = (const QKeyEvent *)event;
    int key = keyEvent->key();
    // should pass only the key presses
    if (keyEvent->type() != QEvent::KeyPress) {
        return false;
    }

    if (key == Qt::Key_Left || key == Qt::Key_Right ||
            key == Qt::Key_Up || key == Qt::Key_Down ||
            key == Qt::Key_Tab)
    {
        return false;
    }

    qDebug() << "filter Event" << endl;
    if (!isInputPanelVisible())
    {
        showInputPanel();
    }

    if (dbusInterface != NULL)
    {
        dbusInterface->call("pressKey", keyEvent->key());
    }

    return true;
}

void VkImPlatformInputContext::keyboardCommit(QString str)
{
    if (focusObject == NULL)
    {
        return;
    }

    qDebug() << "keyboard commit" << endl;

    QInputMethodEvent event;
    event.setCommitString(str);

    QGuiApplication::sendEvent(focusObject, &event);
}
