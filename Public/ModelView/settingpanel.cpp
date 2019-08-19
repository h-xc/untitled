#include "settingpanel.h"

#include <QTabWidget>
#include <QApplication>
#include <QLabel>
#include <QCheckBox>
#include <QRadioButton>
#include <QComboBox>
#include <QPlastiqueStyle>
#include <QScrollBar>
#include <QPainter>
#include <QMouseEvent>
#include <QDesktopWidget>
#include <QDebug>
#include <QAbstractItemDelegate>
#include <QLineEdit>
#include <QListWidgetItem>
#include <QTableView>
#include <QSpacerItem>
#include <QHeaderView>

#include "../Public/ModelView/item_delegate.h"

SettingPanel::SettingPanel(QWidget *parent) : QWidget(parent)
  , tabWidget(NULL)
  , contentsWidget(NULL)
  , widgetScrollArea(NULL)
  , scrollArea(NULL)
  , Y_endPos(0)
{

    QFile file(CSS_FILE);
    if (file.open(QFile::ReadOnly))
    {
        QString styleSheet = file.readAll();
        file.close();
        if(!styleSheet.isEmpty())
        {
            this->setStyleSheet(styleSheet);//把文件内容传参 }
        }
    }
    /*选项卡初始化*/
    tabWidget = new QTabWidget(this);
    tabWidget->setFocusPolicy(Qt::NoFocus);   //
    QWidget *tab1 = new QWidget(this);
    tabWidget->addTab(tab1, QIcon(":/qqSetUp/Image/OPTIONS"),QString("1"));
    tabWidget->setCurrentIndex(0);

    /*目录列表初始化*/
    contentsWidget = new QListWidget(this);
    contentsWidget->setFocusPolicy(Qt::NoFocus);
    connect(contentsWidget, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(slotItemClicked(QListWidgetItem*)));

    /*滚动窗口初始化*/
    scrollArea = new QScrollArea(this);
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scrollArea->setFocusPolicy(Qt::NoFocus);
    connect(scrollArea->verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(slotValueChanged(int))); // 滚动事件
    connect(tabWidget, SIGNAL(currentChanged(int)), this, SLOT(slotCurrentChanged(int)));  // 选项卡改变事件


}

SettingPanel::~SettingPanel()
{

}
void SettingPanel::setModles(QList<ModelStruct *> *Models)
{
    if(Models != NULL)
    {
        m_Models  =  Models;
        /*默认窗口开始选中第一个选项卡*/
        slotCurrentChanged(0);
    }
}

void SettingPanel::clear()
{
    contentsWidget->clear();     // 清除目录窗口
    scrollArea->takeWidget();    //
    if(widgetScrollArea != NULL){
        delete widgetScrollArea;
    }
}

void SettingPanel::resizeEvent(QResizeEvent *event)
{
    tabWidget->setGeometry(0, TAB_WIDGET_TOP, this->width(), this->height() - TAB_WIDGET_TOP);  // 设置选项卡窗口的绘制位置和大小
    contentsWidget->setGeometry(0, CONTENTS_TOP, CONTENTS_WIDTH, this->height() - CONTENTS_TOP);       // 设置目录窗口的绘制位置和大小
    scrollArea->setGeometry(SCROLL_LEFT, SCROLL_TOP, this->width() - SCROLL_LEFT, this->height() - SCROLL_TOP);  // 设置滚动窗口的绘制位置和大小
    if(contentsWidget->item(contentsWidget->count()-1) != NULL)  // 保证调节大小后，目录标签可以跳到最后一个标签
    {
        if((Y_endPos- contentsWidget->item(contentsWidget->count()-1)->data(Qt::UserRole).toInt()) < scrollArea->height())
        {
            widgetScrollArea->resize(widgetScrollArea->width(), contentsWidget->item(contentsWidget->count()-1)->data(Qt::UserRole).toInt() +  scrollArea->height());
        }
    }
    QWidget::resizeEvent(event);
}
void   SettingPanel::addContent(int ax, int ay,QString label)
{
    /*添加目录标签*/
    QLabel *contentsLabel = new QLabel(widgetScrollArea);
    contentsLabel->setText(label);
    contentsLabel->setGeometry(ax, ay,SCROLL_WIDGET_WIDTH,contentsLabel->height());
    contentsLabel->setStyleSheet("background:#8DB1E2");
    contentsLabel->show();
    // 记录目录标签的在滚动窗口的位置
    QListWidgetItem *contentsItem = new QListWidgetItem(label);
    contentsItem->setData(Qt::UserRole,ay);
    contentsWidget->addItem(contentsItem);
}

void SettingPanel::initTabWidget()
{
    int X_SpacePos;
    int Y_SpacePos;

    ItemDelegate* Delegate;
    mydataWidgetMapper*  dataWidgetMapper;
    X_SpacePos = X_START_POS;
    Y_SpacePos = Y_START_POS;

    for (int listPos = 0; listPos < m_Models->size(); ++listPos) {
        X_SpacePos = X_START_POS;
        if(m_Models->at(listPos)->Type == "QStandardItemModel")
        {
            QStandardItemModel *model = (QStandardItemModel *)m_Models->at(listPos)->model;
            if(model == NULL)continue;

            if(!m_Models->at(listPos)->Content.isEmpty())// 如果有定义目录标签
            {
                addContent(X_SpacePos,Y_SpacePos,m_Models->at(listPos)->Content);
                Y_SpacePos += Y_SPACE;  // 下一行小控件显示偏移
            }

            if(m_Models->at(listPos)->View == "Table")  // 表格视图
            {
                if(m_Models->at(listPos)->Content.isEmpty())// 若没有定义目录标签
                {
                    addContent(X_SpacePos,Y_SpacePos,m_Models->at(listPos)->Name);// 表格直接以模型的名称做目录标签
                    Y_SpacePos += Y_SPACE;  // 下一行小控件显示偏移
                }

                QTableView * tableView = new QTableView(widgetScrollArea);
                Delegate = new ItemDelegate(widgetScrollArea);
                tableView->setModel(model);
                tableView->setItemDelegate(Delegate);
                tableView->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
                int scrollBarWidth = 50;
                tableView->setGeometry(X_SpacePos,Y_SpacePos,SCROLL_WIDGET_WIDTH - scrollBarWidth,tableView->rowHeight(0)*5);

                //tableView->adjustSize();
                Y_SpacePos += tableView->rowHeight(0)*5 + scrollBarWidth - Y_SPACE;
            }
            else if(m_Models->at(listPos)->View == "Control")   // 小控件视图
            {
                dataWidgetMapper = new mydataWidgetMapper(widgetScrollArea);
                Delegate = new ItemDelegate(widgetScrollArea);
                dataWidgetMapper->setItemDelegate(Delegate);
                dataWidgetMapper->setModel(model);

                for (int row = 0; row < model->rowCount(); ++row) {
                    X_SpacePos = X_START_POS;
                    for (int col = 0; col <model->columnCount() ; ++col) {
                        QLabel *NameLabel = NULL;
                        QWidget *ValWidget = NULL;
                        if(model->index(row,col).data(ContentRole).isValid())
                        {
                            addContent(X_SpacePos,Y_SpacePos,model->index(row,col).data(ContentRole).toString());// 表格直接以模型的名称做目录标签
                            Y_SpacePos += Y_SPACE;  // 下一行小控件显示偏移
                        }

                        /*定值数据的名称*/
                        if(model->index(row,col).data(NameRole).isValid()
                                && model->index(row,col).data(EditTypeRole).toInt() != CheckBox ) // CheckBox自带描述，不用添加
                        {
                            NameLabel = new QLabel(widgetScrollArea);
                            NameLabel->setText(model->index(row,col).data(NameRole).toString());
                            NameLabel->adjustSize();
                            NameLabel->setGeometry(X_SpacePos, Y_SpacePos,NameLabel->width()+10,NameLabel->height());
                            NameLabel->show();
                        }
                        /*定值数据的编辑器*/
                        if(model->index(row,col).data(EditTypeRole).toInt() == ComboBox)
                        {
                            QComboBox *combox = new QComboBox(widgetScrollArea);
                            ValWidget = combox;
                        }
                        else  if(model->index(row,col).data(EditTypeRole).toInt() == CheckBox)
                        {
                            QCheckBox *checkBox = new QCheckBox(widgetScrollArea);
                            ValWidget = checkBox;
                            checkBox->setText(model->index(row,col).data(NameRole).toString());

                        }
                        else  if(model->index(row,col).data(EditTypeRole).toInt() == LineEdit)
                        {
                            QLineEdit *lineEdit = new QLineEdit(widgetScrollArea);
                            ValWidget = lineEdit;
                        }
                        else
                        {
                            QLabel*  valLabel = new QLabel(widgetScrollArea);
                            ValWidget = valLabel;
                        }
                        /*编辑器与模型绑定，并设置大小和位置*/
                        if(ValWidget != NULL)
                        {
                            dataWidgetMapper->addMapping(ValWidget,row,col); // 与模型对应位置绑定
                            ValWidget->adjustSize();
                            ValWidget->setGeometry(X_SpacePos + NAME_LABEL_WIDTH, Y_SpacePos,EDIT_WIDTH,ValWidget->height());
                            ValWidget->show();
                            if(NameLabel)
                            {
                               NameLabel->move(NameLabel->x(),NameLabel->y() + ValWidget->height()/2 - NameLabel->height()/2);
                            }

                        }
                        X_SpacePos += X_SPACE;
                    }
                    Y_SpacePos += Y_SPACE;
                }
                dataWidgetMapper->revert();
            }
        }
    }
    Y_endPos = Y_SpacePos;
    widgetScrollArea->resize(SCROLL_WIDGET_WIDTH,Y_SpacePos);
}

void SettingPanel::slotCurrentChanged(int index)
{
    clear();
    widgetScrollArea = new QWidget(this);
    scrollArea->setWidget(widgetScrollArea);
    if(index == 0)
    {
        initTabWidget();
    }
    if(contentsWidget->count() != 0)
    {
        contentsWidget->item(0)->setSelected(true);
    }
}

void SettingPanel::slotItemClicked(QListWidgetItem *item)
{
    scrollArea->verticalScrollBar()->setSliderPosition(item->data(Qt::UserRole).toInt());
}

void SettingPanel::slotValueChanged(int value)
{
    for (int itemPos = 0; itemPos < contentsWidget->count(); ++itemPos) {
        if((value + Y_START_POS) >= contentsWidget->item(itemPos)->data(Qt::UserRole).toInt())
        {
            contentsWidget->item(itemPos)->setSelected(true);
        }
        else
        {
            contentsWidget->item(itemPos)->setSelected(false);
        }
    }
}


