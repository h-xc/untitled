#ifndef SETTINGPANEL_H
#define SETTINGPANEL_H

#include <QWidget>
//#include "epushbutton.h"
#include <QListWidgetItem>
#include <QScrollArea>
#include <QPushButton>
#include ".\Public\ModelView\mydatawidgetmapper.h"

#define TITLE_HEIGHT    35

#define TAB_WIDGET_TOP  TITLE_HEIGHT

#define CONTENTS_TOP    (TAB_WIDGET_TOP + 36)
#define CONTENTS_WIDTE  130

#define SCROLL_TOP      CONTENTS_TOP + 1
#define SCROLL_LEFT     CONTENTS_WIDTE + 1



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
    bool signFlag;   // 标记是使用目录窗口更新滚动窗口
};

#endif // SETTINGPANEL_H
