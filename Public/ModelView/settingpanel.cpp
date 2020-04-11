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
#include <QHBoxLayout>
#include <QScroller>

#include <QSpacerItem>
#include "item_delegate.h"


SettingPanel::SettingPanel(QWidget *parent) : QWidget(parent)
  , m_Models(NULL)
  , contentsWidget(NULL)
  , scrollArea(NULL)
  , scrollAreaWidgetContents(NULL)
  , gridLayout(NULL)
  , contentsItemClicked(false)
  , vSpacer(NULL)

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

                "QWidget#scrollAreaWidgetContents{background-color:#FFFFFF;}"

                "QWidget[Content=\"Content\"]{background:#8DB1E2;min-height:30px;margin-top:10px;font-weight: bold;}"
                "QWidget[Name=\"Name\"]{min-width:100px;}"
                "QWidget[Edit=\"Edit\"]{max-width:70px;}"
                ;
    }

    if(!styleSheet.isEmpty())
    {
        this->setStyleSheet(styleSheet);//把文件内容传参 }
    }

    QHBoxLayout *horizontalLayout = new QHBoxLayout(this);// 1:4横向布局类
    horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
    horizontalLayout->setSpacing(0);

    /*1、目录列表初始化*/
    contentsWidget = new QListWidget(this);
    contentsWidget->setFocusPolicy(Qt::NoFocus); // 禁止获取焦距
    contentsWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);// 禁止水平滚动条
    contentsWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);// 禁止垂直滚动条
    contentsWidget->setVerticalScrollMode(QListWidget::ScrollPerPixel);//设置为像素滚动 这样在滚动的时候就不会跳动
    // 增加滚动效果，并设置鼠标左键拖动
    QScroller::grabGesture(contentsWidget,QScroller::LeftMouseButtonGesture);
    // 响应目录鼠标点击信号，用于滚动区定值目录定位
    connect(contentsWidget, SIGNAL(itemClicked(QListWidgetItem*)),
            this, SLOT(slotItemClicked(QListWidgetItem*)));
    horizontalLayout->addWidget(contentsWidget);  // 添加到布局类

    /*2、滚动窗口初始化*/
    scrollArea = new QScrollArea(this);
    scrollAreaWidgetContents = new QWidget();  // 滚动区窗口类
    scrollArea->setFrameShape(QFrame::NoFrame); // 无窗口外框
    scrollArea->setFocusPolicy(Qt::ClickFocus); // 鼠标点击可获取焦距，用于修改定值后点击其它地方，可立即消除编辑框的焦距
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);// 禁止水平滚动条
    scrollArea->setWidgetResizable(true);// 设置滚动区自动根据小控件进行大小调整
    // 增加滚动效果，并设置鼠标左键拖动
    QScroller::grabGesture(scrollArea,QScroller::LeftMouseButtonGesture);
    horizontalLayout->addWidget(scrollArea);    // 添加到布局类

    scrollAreaWidgetContents = new QWidget();  // 创建滚动区窗口类
    scrollAreaWidgetContents->setObjectName(QStringLiteral("scrollAreaWidgetContents"));// 使用id选择器设置样式，方便样式表进行背景色设置，且不会影响小控件继承

    scrollArea->setWidget(scrollAreaWidgetContents); //将滚动区窗口类与滚动窗口类关联
    scrollAreaWidgetContents->installEventFilter(this); // 注册事件过滤器，

    gridLayout = new QGridLayout(scrollAreaWidgetContents);// 设置滚动区为网格布局
    gridLayout->setObjectName(QStringLiteral("gridLayout"));//
    // 滚动窗口滚动事件，用于同步更新目录导航的位置
    connect(scrollArea->verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(slotValueChanged(int))); // 滚动事件

    horizontalLayout->setStretch(0, 1);  // 设置拉伸系数，需要放在最后面
    horizontalLayout->setStretch(1, 4);  // 设置拉伸系数，需要放在才生效最后面
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
    if(m_Models != NULL) //
    {
        clear();     // 清空当前的界面
        scrollAreaWidgetContents = new QWidget();   // 重新创建窗口类
        scrollAreaWidgetContents->setObjectName(QStringLiteral("scrollAreaWidgetContents")); // 使用id选择器设置样式，这样不会被子控件继承
        scrollArea->setWidget(scrollAreaWidgetContents);
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

void   SettingPanel::addContent(int &GridRow,QString label)
{
    /*添加目录标签*/
    QLabel *contentsLabel = new QLabel(scrollAreaWidgetContents);
    contentsLabel->setProperty("Content","Content");  // 设置属性，用于样式表进行背景色设置
    contentsLabel->setText(label);
    gridLayout->addWidget(contentsLabel,GridRow,0,1,-1);

    // 记录目录标签的在滚动窗口的位置
    QListWidgetItem *contentsItem = new QListWidgetItem(label);
    contentsItem->setData(Qt::UserRole,GridRow);
    contentsWidget->addItem(contentsItem);
    GridRow ++;
}

void SettingPanel::initWidget()
{
    int  GridRow = 0; // 表格布局行
    ItemDelegate* Delegate;
    mydataWidgetMapper*  dataWidgetMapper;

    for (int listPos = 0; listPos < m_Models->size(); ++listPos) {
        if(m_Models->at(listPos)->Type == "QStandardItemModel")   // 标准模型类型
        {
            QStandardItemModel *model = (QStandardItemModel *)m_Models->at(listPos)->model;
            if(model == NULL)continue;

            if(!m_Models->at(listPos)->Content.isEmpty())// 如果有定义目录标签
            {
                addContent(GridRow,m_Models->at(listPos)->Content);
            }

            /*表格视图*/
            if(m_Models->at(listPos)->View == "Table")
            {
                if(m_Models->at(listPos)->Content.isEmpty())// 若没有定义目录标签
                {
                    addContent(GridRow,m_Models->at(listPos)->Name);// 表格直接以模型的名称做目录标签
                }

                QTableView * tableView = new QTableView(scrollAreaWidgetContents);
                tableView->setEditTriggers(QAbstractItemView::AllEditTriggers);// 单击直接编辑
                tableView->setVerticalScrollMode(QListWidget::ScrollPerPixel);//设置为像素滚动

                QScroller::grabGesture(tableView,QScroller::LeftMouseButtonGesture);//设置鼠标左键拖动
                tableView->installEventFilter(this); // 注册事件管理器，用于滚动区拖动的冲突处理

                Delegate = new ItemDelegate(scrollAreaWidgetContents);
                tableView->setModel(model);
                tableView->setItemDelegate(Delegate);

#if (QT_VERSION <= QT_VERSION_CHECK(5,0,0))  // 目前需要适应QT5和QT4版本，
                tableView->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
#else
                tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
#endif
                // 设置表格为5行数据的高低
                tableView->setMinimumSize(QSize(0, tableView->horizontalHeader()->height()+tableView->rowHeight(0)*5));

                // 设置表格高度为固定值
                QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Fixed);
                tableView->setSizePolicy(sizePolicy1);

                gridLayout->addWidget(tableView,GridRow,1,1,gridLayout->columnCount()-1);
                GridRow ++;
            }
            /*小控件视图*/
            else if(m_Models->at(listPos)->View == "Control")
            {
                dataWidgetMapper = new mydataWidgetMapper(scrollAreaWidgetContents);//映射类 将控件与模型关联起来
                Delegate = new ItemDelegate(scrollAreaWidgetContents); // 委托类 实现控件与模型的数据关联

                dataWidgetMapper->setItemDelegate(Delegate);
                dataWidgetMapper->setModel(model);

                for (int row = 0; row < model->rowCount(); ++row,GridRow++)
                {
                    for (int col = 0; col <model->columnCount() ; ++col)
                    {
                        QLabel *NameLabel = NULL;  // 定值名称控件
                        QWidget *ValWidget = NULL; // 定值编辑控件

                        // 模型项名称为空，不做任何动作
                        if(!model->index(row,col).data(ItemDelegate::VarNameRole).isValid())
                        {
                            continue;
                        }

                        // 模型项中包含目录标签角色则添加标签
                        if(model->index(row,col).data(ItemDelegate::ContentRole).isValid())
                        {
                            addContent(GridRow,model->index(row,col).data(ItemDelegate::ContentRole).toString());// 表格直接以模型的名称做目录标签
                        }

                        //复选框是没有编辑器的，直接放在名称的前面即可。
                        if(model->index(row,col).data(ItemDelegate::EditTypeRole).toInt() == ItemDelegate::CheckBox)
                        {
                            QCheckBox *checkBox = new QCheckBox(scrollAreaWidgetContents);
                            dataWidgetMapper->addMapping(checkBox,row,col); // 与模型对应位置绑定
                            checkBox->setText(model->index(row,col).data(ItemDelegate::NameRole).toString());
                            checkBox->adjustSize();
                            gridLayout->addWidget(checkBox,GridRow,NAME_COL(col),1,2);
                        }
                        /*定值数据的名称*/
                        else if(model->index(row,col).data(ItemDelegate::NameRole).isValid()) // CheckBox自带描述，不用添加
                        {
                            NameLabel = new QLabel(scrollAreaWidgetContents);
                            NameLabel->setProperty("Name","Name");  // 设置属性，用于样式表进行背景色设置
                            NameLabel->setText(model->index(row,col).data(ItemDelegate::NameRole).toString());
                            NameLabel->adjustSize();
                            gridLayout->addWidget(NameLabel,GridRow,NAME_COL(col),1,1);
                        }

                        /*定值数据的编辑器*/
                        if(model->index(row,col).data(ItemDelegate::EditTypeRole).toInt() == ItemDelegate::ComboBox)
                        {
                            QComboBox *combox = new QComboBox(scrollAreaWidgetContents);
                            ValWidget = combox;
                        }
                        else  if(model->index(row,col).data(ItemDelegate::EditTypeRole).toInt() == ItemDelegate::LineEdit)
                        {
                            QLineEdit *lineEdit = new MyLineEdit(scrollAreaWidgetContents);
                            ValWidget = lineEdit;
                        }
                        else
                        {
                            QLabel*  valLabel = new QLabel(scrollAreaWidgetContents);
                            ValWidget = valLabel;
                        }

                        /*编辑器与模型绑定，并设置大小和位置*/
                        if(ValWidget != NULL)
                        {
                            ValWidget->setProperty("Edit","Edit");  // 设置属性，用于样式表进行背景色设置
                            dataWidgetMapper->addMapping(ValWidget,row,col); // 与模型对应位置绑定
                            ValWidget->adjustSize();
                            gridLayout->addWidget(ValWidget,GridRow,EDIT_COL(col),1,1);
                        }
                    }
                }
                dataWidgetMapper->revert();     // 将模型数据搬到小控件中
            }
        }
    }

    // 每一组（名称加编辑框）小控件后面添加一个弹簧控件，用于伸缩
    for(int hSpacerCol = 0;hSpacerCol < gridLayout->columnCount()+1; hSpacerCol += 3)
    {
        QSpacerItem *hSpacer = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);
        gridLayout->addItem(hSpacer, 1, hSpacerCol, 1, 1);
    }
    // 末尾添加弹簧
    vSpacer = new QSpacerItem(0,0,QSizePolicy::Minimum, QSizePolicy::Expanding);
    gridLayout->addItem(vSpacer,gridLayout->rowCount(),0,1,1);
}

void SettingPanel::slotItemClicked(QListWidgetItem *item)
{
    contentsItemClicked = true;
    int pos = gridLayout->cellRect(item->data(Qt::UserRole).toInt(),0).top(); // 获取目录在滚动区的位置

    // 使用滚动效果，定位到目录标签
    QScroller *scroller = QScroller::scroller(scrollArea);
    scroller->scrollTo(QPointF(scrollArea->horizontalScrollBar()->sliderPosition(), pos));
}

void SettingPanel::slotValueChanged(int value)
{
    int pos = 0;
    QScroller *scroller = QScroller::scroller(scrollArea);
    if(contentsItemClicked )// 若是目录标签进行的位置更行，则不更行目录窗口
    {
        if(scroller->state() == QScroller::Scrolling)// 滚动效果为自滚动，即不是用鼠标滚动的
        {
            return;
        }
        contentsItemClicked = false; // 鼠标进行滚动区的滚动，则清除目录选择标记
    }
    // 更行目录窗口
    for (int itemPos = 0; itemPos < contentsWidget->count(); ++itemPos)
    {
        pos = gridLayout->cellRect(
                    contentsWidget->item(itemPos)->data(Qt::UserRole).toInt(),0).top();
        if(value >= pos)
        {
            contentsWidget->setCurrentRow(itemPos);
        }
    }
}

bool SettingPanel::eventFilter(QObject *obj, QEvent *eve)
{
    if(obj->metaObject()->className() == QTableView::staticMetaObject.className())
    {
        // 主要是表格与滚动区滚动效果的冲突处理。不然没法滚动表格
        if(eve->type() == QEvent::Enter)
        {
            QScroller::ungrabGesture(scrollArea);//设置鼠标左键拖动
        }
        else if(eve->type() == QEvent::Leave)
        {
            QScroller::grabGesture(scrollArea,QScroller::LeftMouseButtonGesture);//设置鼠标左键拖动
        }
    }
    else if(obj == scrollAreaWidgetContents)
    {
        //程序在第一次resizeEvent事件中， gridLayout是没有刷新的，所以目录标签的位置都是0，所以截取滚动区改变大小的时间，再次人为刷新。
        if(eve->type() == QEvent::Resize)
        {
             resizeEvent(NULL);
        }
    }
    return QWidget::eventFilter(obj,eve);
}

void SettingPanel::resizeEvent(QResizeEvent* size)
{
    QWidget::resizeEvent(size);

    if(!contentsWidget->count()>0)return;

    // 获取滚动最后一个标签的在滚动区的位置
    int contentEndPos = gridLayout->cellRect(
                contentsWidget->item(contentsWidget->count()-1)->data(Qt::UserRole).toInt(),0).top();

    if(contentEndPos == 0)return;
    scrollAreaWidgetContents->setMinimumHeight(0);  // 滚动区窗口先自适应

    // 填充空白到滚动区窗口最尾端，让导航目录可以跳转到滚动区对应的定值标签上
    if(gridLayout->cellRect(gridLayout->rowCount()-1,0).top() - contentEndPos +10 < scrollArea->height())
    {
        scrollAreaWidgetContents->setMinimumHeight(contentEndPos+scrollArea->height());
    }


}

