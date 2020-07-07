#ifndef SETTINGPANEL_H
#define SETTINGPANEL_H

#include <QWidget>
//#include "epushbutton.h"
#include <QListWidgetItem>
#include <QScrollArea>
#include <QPushButton>
#include <QStandardItemModel>
#include <QGridLayout>
#include "mydatawidgetmapper.h"
#include "datastore.h"
#include "mylineedit.h"
#define CSS_FILE  "./SettingPanel.css"

#define  NAME_COL(col)   (col*3+1)
#define  EDIT_COL(col)   (col*3+2)

class SettingPanel : public QWidget
{
    Q_OBJECT
public:
    SettingPanel(QWidget *parent = 0);
    ~SettingPanel();
    void setModles(QList<ModelStruct *> *Models);
    void clear();

    void initTableView();

protected:
    void  addContent(int &GridRow,QString label);
    const  QList<ModelStruct *>   *m_Models;

private:
    void initWidget();

private slots:
    void slotItemClicked(QListWidgetItem *item);
    void slotValueChanged(int value);

private:
    QListWidget *contentsWidget;
    QScrollArea *scrollArea;

    QWidget     *scrollAreaWidgetContents;
    QGridLayout *gridLayout;

    bool   contentsItemClicked;   //标记是否点击了目录导航 false 无操作  true 正在操作

    bool eventFilter(QObject *, QEvent *);  //事件过滤器，当滑动表格时，先屏蔽窗口的滑动事件
    void resizeEvent(QResizeEvent* size);
};

#endif // SETTINGPANEL_H
