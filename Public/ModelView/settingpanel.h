#ifndef SETTINGPANEL_H
#define SETTINGPANEL_H

#include <QWidget>
//#include "epushbutton.h"
#include <QListWidgetItem>
#include <QScrollArea>
#include <QPushButton>

class SettingPanel : public QWidget
{
    Q_OBJECT
public:
    SettingPanel(QWidget *parent = 0);
    ~SettingPanel();

protected:
     void paintEvent(QPaintEvent *event);
     void mousePressEvent(QMouseEvent *event);
     void mouseReleaseEvent(QMouseEvent *event);
     void mouseMoveEvent(QMouseEvent *event);
     void resizeEvent(QResizeEvent *event);

private:
     void initTabOneWidget();
     void initTabTwoWidget();
     void initTabThreeWidget();

private slots:
     void slotCurrentChanged(int index);
     void slotItemClicked(QListWidgetItem *item);
     void slotValueChanged(int value);

private:
    QPoint movePoint;
    bool mousePress;
//    EPushButton *minButton;
//    EPushButton *closeButton;
    QPushButton *minButton;
    QPushButton *closeButton;
    QRect rectMove;
    QTabWidget *tabWidget;
    QListWidget *contentsWidget;
    QWidget *widgetScrollArea;
    QScrollArea *scrollArea;
    QWidget *loginWidget;
    QWidget *panelWidget;
    QWidget *statusWidget;
    QWidget *sessionWidget;

    QWidget *passwordWidget;
    QWidget *qqlockWidget;

    QWidget *spaceWidget;
    bool signFlag;
};

#endif // SETTINGPANEL_H
