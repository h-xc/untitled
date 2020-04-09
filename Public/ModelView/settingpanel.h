#ifndef SETTINGPANEL_H
#define SETTINGPANEL_H

#include <QWidget>
//#include "epushbutton.h"
#include <QListWidgetItem>
#include <QScrollArea>
#include <QPushButton>
#include <QStandardItemModel>
#include "mydatawidgetmapper.h"
#include "datastore.h"
#include "mylineedit.h"
#define CSS_FILE  "./SettingPanel.css"

#define CONTENTS_TOP    0      // 目录窗口头位置（LEFT为0）
#define CONTENTS_WIDTH  150    // 目录窗口宽度

#define SCROLL_TOP      CONTENTS_TOP + 1      // 滚动窗口头位置 比目录窗口低一个像素
#define SCROLL_LEFT     CONTENTS_WIDTH        // 滚动窗口左边开始位置，
#define SCROLL_WIDGET_WIDTH    500            // 滚动窗口宽度

#define Y_START_POS     10                    // 定值控件y开始位置
#define Y_SPACE         40                    // 定值控件y偏移位置

#define X_START_POS     30
#define X_SPACE         250


#define NAME_LABEL_WIDTH 100                 // 定值名称宽度
#define EDIT_WIDTH  70                       // 编辑器宽度



class SettingPanel : public QWidget
{
    Q_OBJECT
public:
    SettingPanel(QWidget *parent = 0);
    ~SettingPanel();
    void setModles(QList<ModelStruct *> *Models);
    void clear();

protected:
    void  resizeEvent(QResizeEvent *event);
    void  addContent(int ax, int ay,QString label);
    const  QList<ModelStruct *>   *m_Models;


private:
    void initWidget();


private slots:
    void slotItemClicked(QListWidgetItem *item);
    void slotValueChanged(int value);

private:
    QListWidget *contentsWidget;
    QWidget     *widgetScrollArea;
    QScrollArea *scrollArea;
    int    Y_endPos;
    bool   ItemClickedFlag;   //标记是否点击了目录导航 false 无操作  true 正在操作

    bool eventFilter(QObject *, QEvent *);  //事件过滤器
    //实现鼠标拖动功能
    QObject         *m_Sender;
    bool            m_bMousePressed;
    QPoint          m_PressPosition;

};

#endif // SETTINGPANEL_H
