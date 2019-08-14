#ifndef SETTINGPANEL_H
#define SETTINGPANEL_H

#include <QWidget>
//#include "epushbutton.h"
#include <QListWidgetItem>
#include <QScrollArea>
#include <QPushButton>
#include <QStandardItemModel>
#include "mydatawidgetmapper.h"

#define CSS_FILE  ""

#define TAB_WIDGET_TOP  0

#define CONTENTS_TOP    (TAB_WIDGET_TOP + 36)
#define CONTENTS_WIDTH  130

#define SCROLL_TOP      CONTENTS_TOP + 1
#define SCROLL_LEFT     CONTENTS_WIDTH + 1
#define SCROLL_WIDGET_WIDTH    500

#define Y_START_POS     10
#define Y_SPACE         40

#define X_START_POS     30
#define X_SPACE         200

#define EDIT_WIDTH  70

struct ModelStruct{
    QString Name;
    QString View;
    QString Type;
    QString Content;  // 标签记录
    QAbstractItemModel* model;
};

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
    int    Y_endPos;

private:
    void initTabWidget();


private slots:
    void slotCurrentChanged(int index);
    void slotItemClicked(QListWidgetItem *item);
    void slotValueChanged(int value);

private:
    QTabWidget  *tabWidget;
    QListWidget *contentsWidget;
    QWidget     *widgetScrollArea;
    QScrollArea *scrollArea;
};

#endif // SETTINGPANEL_H
