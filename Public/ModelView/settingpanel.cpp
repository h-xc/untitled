#include "settingpanel.h"

#include <QTabWidget>
#include <QApplication>
#include <QLabel>
#include <QCheckBox>
#include <QRadioButton>
#include <QComboBox>
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

#include <QScroller>

#include "item_delegate.h"


SettingPanel::SettingPanel(QWidget *parent) : QWidget(parent)
  , contentsWidget(NULL)
  , widgetScrollArea(NULL)
  , scrollArea(NULL)
  , Y_endPos(0)
  , ItemClickedFlag(false)
  , m_Models(NULL)
{
    QFile file(CSS_FILE);
    QString styleSheet;
    if (file.open(QFile::ReadOnly))  // 加载样式表
    {
        styleSheet = file.readAll();
        file.close();

    }
    else
    {
        styleSheet =
                "QListWidget{width:150px;background:rgba(240,240,240,255);border:0px solid gray;padding:0px;}"
                "QListWidget::item{width:100px;height:35px;border:0px solid gray;padding-left:20px;}"
                "QListWidget::item:selected{background:#FFFFFF;}"

                "QScrollArea{background:#FFFFFF;}"
                "QWidget#widgetScrollArea{background-color:#FFFFFF;border-color:#FFFFFF;}";
    }

    if(!styleSheet.isEmpty())
    {
        this->setStyleSheet(styleSheet);//把文件内容传参 }
    }
    /*目录列表初始化*/
    contentsWidget = new QListWidget(this);
    contentsWidget->setFocusPolicy(Qt::NoFocus);
    contentsWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    contentsWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    contentsWidget->setVerticalScrollMode(QListWidget::ScrollPerPixel);//设置为像素滚动
    QScroller::grabGesture(contentsWidget,QScroller::LeftMouseButtonGesture);//设置鼠标左键拖动

    connect(contentsWidget, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(slotItemClicked(QListWidgetItem*)));

    /*滚动窗口初始化*/
    scrollArea = new QScrollArea(this);
    widgetScrollArea = new QWidget();
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scrollArea->setFocusPolicy(Qt::ClickFocus);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    QScroller::grabGesture(scrollArea,QScroller::LeftMouseButtonGesture);//设置鼠标左键拖动


    widgetScrollArea->setObjectName(QStringLiteral("widgetScrollArea"));
    scrollArea->setWidget(widgetScrollArea);

     // 滚动窗口滚动事件
    connect(scrollArea->verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(slotValueChanged(int))); // 滚动事件
}

SettingPanel::~SettingPanel()
{

}

void SettingPanel::setModles(QList<ModelStruct *> *Models)
{
    if(Models == NULL)
    {
        qDebug() << "SettingPanel::setModles(QList<ModelStruct *> *Models)  Models is NULL";
        return;
    }
    if(m_Models != NULL)
    {
        clear();     // 清空当前的界面
        widgetScrollArea = new QWidget();   // 重新创建窗口类
        widgetScrollArea->setObjectName(QStringLiteral("widgetScrollArea"));
        scrollArea->setWidget(widgetScrollArea);
    }

    m_Models  =  Models;
    initWidget();   // 初始化当前的界面
    if(contentsWidget->count() != 0)// 默认选中第一个目录标签
    {
        contentsWidget->item(0)->setSelected(true);
    }
}

void SettingPanel::clear()
{
    contentsWidget->clear();     // 清除目录窗口
    delete scrollArea->takeWidget();    //
}

void SettingPanel::resizeEvent(QResizeEvent *event)
{
    contentsWidget->setGeometry(0, CONTENTS_TOP, CONTENTS_WIDTH, this->height() - CONTENTS_TOP);       // 设置目录窗口的绘制位置和大小
    scrollArea->setGeometry(SCROLL_LEFT, SCROLL_TOP, this->width() - SCROLL_LEFT, this->height() - SCROLL_TOP);  // 设置滚动窗口的绘制位置和大小
    if(contentsWidget->item(contentsWidget->count()-1) != NULL)  // 保证调节大小后，目录标签可以跳到最后一个标签
    {
        if((Y_endPos - contentsWidget->item(contentsWidget->count()-1)->data(Qt::UserRole).toInt()) < scrollArea->height())
        {
            widgetScrollArea->resize(SCROLL_WIDGET_WIDTH, contentsWidget->item(contentsWidget->count()-1)->data(Qt::UserRole).toInt() +  scrollArea->height() - Y_START_POS);
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

void SettingPanel::initWidget()
{
    int X_SpacePos = X_START_POS;
    int Y_SpacePos = Y_START_POS;

    ItemDelegate* Delegate;
    mydataWidgetMapper*  dataWidgetMapper;

    for (int listPos = 0; listPos < m_Models->size(); ++listPos) {
        X_SpacePos = X_START_POS;
        if(m_Models->at(listPos)->Type == "QStandardItemModel")   // 标准模型类型
        {
            QStandardItemModel *model = (QStandardItemModel *)m_Models->at(listPos)->model;
            if(model == NULL)continue;

            if(!m_Models->at(listPos)->Content.isEmpty())// 如果有定义目录标签
            {
                addContent(X_SpacePos,Y_SpacePos,m_Models->at(listPos)->Content);
                Y_SpacePos += Y_SPACE;  // 下一行小控件显示偏移
            }
            /*表格视图*/
            if(m_Models->at(listPos)->View == "Table")
            {
                if(m_Models->at(listPos)->Content.isEmpty())// 若没有定义目录标签
                {
                    addContent(X_SpacePos,Y_SpacePos,m_Models->at(listPos)->Name);// 表格直接以模型的名称做目录标签
                    Y_SpacePos += Y_SPACE;  // 下一行小控件显示偏移
                }

                QTableView * tableView = new QTableView(widgetScrollArea);

                tableView->setVerticalScrollMode(QListWidget::ScrollPerPixel);//设置为像素滚动
                QScroller::grabGesture(tableView,QScroller::LeftMouseButtonGesture);//设置鼠标左键拖动
                tableView->installEventFilter(this);

                Delegate = new ItemDelegate(widgetScrollArea);
                tableView->setModel(model);
                tableView->setItemDelegate(Delegate);
#if (QT_VERSION <= QT_VERSION_CHECK(5,0,0))  // 目前需要适应QT5和QT4版本，
                tableView->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
#else
                tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
#endif
                int scrollBarWidth = 50;
                tableView->setGeometry(X_SpacePos,Y_SpacePos,SCROLL_WIDGET_WIDTH - scrollBarWidth,tableView->rowHeight(0)*5);

                //tableView->adjustSize();
                Y_SpacePos += tableView->rowHeight(0)*5 + scrollBarWidth - Y_SPACE;
            }
            /*小控件视图*/
            else if(m_Models->at(listPos)->View == "Control")
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
                        if(!model->index(row,col).data(ItemDelegate::VarNameRole).isValid())
                        {
                            X_SpacePos += X_SPACE;  // 列偏移
                            continue;
                        }
                        if(model->index(row,col).data(ItemDelegate::ContentRole).isValid())
                        {
                            addContent(X_SpacePos,Y_SpacePos,model->index(row,col).data(ItemDelegate::ContentRole).toString());// 表格直接以模型的名称做目录标签
                            Y_SpacePos += Y_SPACE;  // 下一行小控件显示偏移
                        }
                        //复选框是没有编辑器的，直接放在名称的前面即可。
                        if(model->index(row,col).data(ItemDelegate::EditTypeRole).toInt() == ItemDelegate::CheckBox)
                        {
                            QCheckBox *checkBox = new QCheckBox(widgetScrollArea);
                            dataWidgetMapper->addMapping(checkBox,row,col); // 与模型对应位置绑定
                            checkBox->setText(model->index(row,col).data(ItemDelegate::NameRole).toString());
                            checkBox->adjustSize();
                            checkBox->setGeometry(X_SpacePos, Y_SpacePos,checkBox->width()+10,checkBox->height());
                            checkBox->show();
                        }
                        /*定值数据的名称*/
                        else if(model->index(row,col).data(ItemDelegate::NameRole).isValid()) // CheckBox自带描述，不用添加
                        {
                            NameLabel = new QLabel(widgetScrollArea);
                            NameLabel->setText(model->index(row,col).data(ItemDelegate::NameRole).toString());
                            NameLabel->adjustSize();
                            NameLabel->setGeometry(X_SpacePos, Y_SpacePos,NameLabel->width()+10,NameLabel->height());
                            NameLabel->show();
                        }
                        /*定值数据的编辑器*/
                        if(model->index(row,col).data(ItemDelegate::EditTypeRole).toInt() == ItemDelegate::ComboBox)
                        {
                            QComboBox *combox = new QComboBox(widgetScrollArea);
                            ValWidget = combox;
                        }
                        else  if(model->index(row,col).data(ItemDelegate::EditTypeRole).toInt() == ItemDelegate::LineEdit)
                        {
                            QLineEdit *lineEdit = new MyLineEdit(widgetScrollArea);
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
                            int ax = NAME_LABEL_WIDTH;// 默认偏移NAME_LABEL_WIDTH
                            dataWidgetMapper->addMapping(ValWidget,row,col); // 与模型对应位置绑定
                            ValWidget->adjustSize();
                            if(NameLabel)
                            {
                                // 控件中间对齐
                                NameLabel->move(NameLabel->x(),NameLabel->y() + ValWidget->height()/2 - NameLabel->height()/2);
                                ax = ax<NameLabel->width()?NameLabel->width():ax;
                            }
                            ValWidget->setGeometry(ax + X_SpacePos, Y_SpacePos,EDIT_WIDTH,ValWidget->height());
                            ValWidget->show();

                        }
                        X_SpacePos += X_SPACE;  // 列偏移
                    }
                    Y_SpacePos += Y_SPACE;      // 行偏移
                }
                dataWidgetMapper->revert();     // 将数据搬到小控件中
            }//end by else if(m_Models->at(listPos)->View == "Control")
        }//ebd by if(m_Models->at(listPos)->Type == "QStandardItemModel")
    }//end by for (int listPos = 0; listPos < m_Models->size(); ++listPos)
    Y_endPos = Y_SpacePos;  // 记录Y坐标当前偏移的坐标，使目录标签能定位到滚动窗口后面的标签
    widgetScrollArea->resize(scrollArea->width(), Y_endPos);
}

void SettingPanel::slotItemClicked(QListWidgetItem *item)
{
    // 缓慢滚动至目标
    int oldPos = scrollArea->verticalScrollBar()->sliderPosition();
    int nowPos = item->data(Qt::UserRole).toInt();
    int step = (nowPos - oldPos)/10;
    ItemClickedFlag = true;
    if(step > 0)
    {
        for(;oldPos < nowPos;oldPos += step)
        {
            scrollArea->verticalScrollBar()->setSliderPosition(oldPos);
            scrollArea->repaint();
        }
    }
    else
    {
        for(;oldPos > nowPos;oldPos += step)
        {
            scrollArea->verticalScrollBar()->setSliderPosition(oldPos);
            scrollArea->repaint();
        }
    }
    scrollArea->verticalScrollBar()->setSliderPosition(item->data(Qt::UserRole).toInt());
    ItemClickedFlag = false;

}

void SettingPanel::slotValueChanged(int value)
{
    if(ItemClickedFlag == false)
    {
        for (int itemPos = 0; itemPos < contentsWidget->count(); ++itemPos) {
            if((value + Y_START_POS) >= contentsWidget->item(itemPos)->data(Qt::UserRole).toInt())
            {
                //contentsWidget->item(itemPos)->setSelected(true);
                contentsWidget->setCurrentRow(itemPos);
            }
        }
    }
}

bool SettingPanel::eventFilter(QObject *obj, QEvent *eve)
{
    if(obj->metaObject()->className() == QTableView::staticMetaObject.className())
    {

        if(eve->type() == QEvent::Enter)
        {
            QScroller::ungrabGesture(scrollArea);//设置鼠标左键拖动
        }
        else if(eve->type() == QEvent::Leave)
        {
            QScroller::grabGesture(scrollArea,QScroller::LeftMouseButtonGesture);//设置鼠标左键拖动
        }
    }
   return QWidget::eventFilter(obj,eve);
}



