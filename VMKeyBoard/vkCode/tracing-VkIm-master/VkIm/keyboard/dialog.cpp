#include "dialog.h"
#include <QPushButton>
#include <QGridLayout>
#include <QLineEdit>
#include <QSignalMapper>
#include <QDesktopWidget>
#include <QDebug>
#include <QKeyEvent>
#include <QGuiApplication>
#include <QApplication>
#include <QRegExpValidator>
#include <QDBusVariant>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent)
{
    setWindowFlags(Qt::WindowDoesNotAcceptFocus | Qt::FramelessWindowHint |
                   Qt::WindowStaysOnTopHint | Qt::Tool | Qt::X11BypassWindowManagerHint);
    initWidgets();
}

Dialog::~Dialog()
{

}

void Dialog::initWidgets()
{
    QGridLayout *gridLayout = new QGridLayout(this);
    QSignalMapper *signalMapper = new QSignalMapper(this);

    for (int i = 0; i < 10; i++)
    {
        QString text;
        text.sprintf("%d", i);
        numBt[i] = new QPushButton(this);
        numBt[i]->setText(text);

        connect(numBt[i], SIGNAL(clicked()), signalMapper, SLOT(map()));
        signalMapper->setMapping(numBt[i], i);

    }

    dotBt = new QPushButton(this);
    dotBt->setText(".");


    minusBt = new QPushButton(this);
    minusBt->setText("-");

    backspaceBt = new QPushButton(this);
    backspaceBt->setObjectName("Backspace");
    backspaceBt->setText("Bkspace");

    enterBt = new QPushButton(this);
    enterBt->setObjectName("Enter");
    enterBt->setText("Enter");

    inputEdit = new QLineEdit(this);

    connect(dotBt, SIGNAL(clicked()), signalMapper, SLOT(map()));
    connect(minusBt, SIGNAL(clicked()), signalMapper, SLOT(map()));
    connect(backspaceBt, SIGNAL(clicked()), signalMapper, SLOT(map()));
    connect(enterBt, SIGNAL(clicked()), signalMapper, SLOT(map()));

    signalMapper->setMapping(dotBt, 10);
    signalMapper->setMapping(minusBt, 11);
    signalMapper->setMapping(backspaceBt, 12);
    signalMapper->setMapping(enterBt, 13);

    connect(signalMapper, SIGNAL(mapped(int)), this, SLOT(clickButton(int)));

    gridLayout->setSpacing(0);
    gridLayout->setSizeConstraint(QLayout::SetFixedSize);

    gridLayout->addWidget(inputEdit, 0, 0, 1, 4, Qt::AlignCenter);
    gridLayout->addWidget(backspaceBt, 1, 3, 1, 1, Qt::AlignCenter);
    gridLayout->addWidget(enterBt, 2, 3, 4, 1, Qt::AlignCenter);

    gridLayout->addWidget(numBt[7], 1, 0, 1, 1, Qt::AlignCenter);
    gridLayout->addWidget(numBt[8], 1, 1, 1, 1, Qt::AlignCenter);
    gridLayout->addWidget(numBt[9], 1, 2, 1, 1, Qt::AlignCenter);

    gridLayout->addWidget(numBt[4], 2, 0, 1, 1, Qt::AlignCenter);
    gridLayout->addWidget(numBt[5], 2, 1, 1, 1, Qt::AlignCenter);
    gridLayout->addWidget(numBt[6], 2, 2, 1, 1, Qt::AlignCenter);

    gridLayout->addWidget(numBt[1], 3, 0, 1, 1, Qt::AlignCenter);
    gridLayout->addWidget(numBt[2], 3, 1, 1, 1, Qt::AlignCenter);
    gridLayout->addWidget(numBt[3], 3, 2, 1, 1, Qt::AlignCenter);

    gridLayout->addWidget(numBt[0], 4, 0, 1, 1, Qt::AlignCenter);
    gridLayout->addWidget(dotBt,    4, 1, 1, 1, Qt::AlignCenter);
    gridLayout->addWidget(minusBt,  4, 2, 1, 1, Qt::AlignCenter);

}

void Dialog::clickButton(int index)
{
    QKeyEvent *keyPressEvent = NULL;

    if (index >=0 && index < 10)
    {
        /* 0-9 */
        QString text;
        text.sprintf("%d", index);
        keyPressEvent = new QKeyEvent(QEvent::KeyPress, Qt::Key_0 + index,
                                      Qt::NoModifier, text);
    }
    else if (index == 10)
    {
        /* dot */
        keyPressEvent = new QKeyEvent(QEvent::KeyPress, Qt::Key_Period,
                                      Qt::NoModifier, ".");
    }
    else if (index == 11)
    {
        /* minus */
        keyPressEvent = new QKeyEvent(QEvent::KeyPress, Qt::Key_Minus,
                                      Qt::NoModifier, "-");
    }
    else if (index == 12)
    {
        /* backspace */
        keyPressEvent = new QKeyEvent(QEvent::KeyPress, Qt::Key_Backspace,
                                      Qt::NoModifier);
    }
    else if (index == 13)
    {
        /* enter */
        QString str = inputEdit->text();
        emit commit(str);
        inputEdit->setText("");
        hide();
    }

    if (keyPressEvent != NULL)
    {
        QGuiApplication::postEvent(inputEdit, keyPressEvent);
    }
}

void Dialog::pressKey(int key)
{
    QKeyEvent *keyPressEvent = NULL;
    QString text;

    qDebug() << "press key " << key << endl;

    if (key >= Qt::Key_0 && key <= Qt::Key_9)
    {
        /* 0-9 */
        text.sprintf("%d", key - Qt::Key_0);
    }
    else if (key == Qt::Key_Period)
    {
        /* dot */
        text = QString(".");
    }
    else if (key == Qt::Key_Minus)
    {
        /* minus */
        text = QString("-");
    }
    else if (key == Qt::Key_Return)
    {
        /* enter */
        qDebug() << "enter pressed" << endl;
        QString str = inputEdit->text();
        emit commit(str);
        inputEdit->setText("");
        hide();
        return;
    }
    else if (key != Qt::Key_Backspace)
    {
        return;
    }

    keyPressEvent = new QKeyEvent(QEvent::KeyPress, key,
                                  Qt::NoModifier, text);
    if (keyPressEvent != NULL)
    {
        QGuiApplication::postEvent(inputEdit, keyPressEvent);
    }

}

void Dialog::showKeyboard(QPoint pt, QRect focusWidget)
{
    QDesktopWidget* desktopWidget = QApplication::desktop();
    QRect kbRect = QWidget::rect();
    QRect screenRect = desktopWidget->screenGeometry();

    qDebug() << "dialog show" << endl;
    qDebug() << pt.x() << " " << pt.y() << endl;

    pt.setY(pt.y() + focusWidget.height());

    QWidget::move(pt);
    QWidget::show();
}

void Dialog::hideKeyboard()
{
    qDebug() << "dialog hide" << endl;
    QWidget::hide();
}

bool Dialog::isVisible() const
{
    return QWidget::isVisible();
}

