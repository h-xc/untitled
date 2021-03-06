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

#include "./qqSetUp/qqsetup.h"
#include "../Public/ModelView/item_delegate.h"


SettingPanel::SettingPanel(QWidget *parent) : QWidget(parent)
  , mousePress(false)
  , signFlag(false)
  , loginWidget(NULL)
  , panelWidget(NULL)
  , statusWidget(NULL)
  , sessionWidget(NULL)
  , passwordWidget(NULL)
  , qqlockWidget(NULL)
  , spaceWidget(NULL)
  , m_Models(NULL)
{
    resize(parent->size()); // 设置窗口大小
    qqSetUp*  widget= (qqSetUp*) parent;
    m_Models = &widget->m_Models;
    m_ModelsSort = &widget->m_ModelsSort;

    QFile file(".\\dracula.css");
    file.open(QFile::ReadOnly);
    if (file.isOpen())
    {
        QString styleSheet = this->styleSheet();
        styleSheet += QLatin1String(file.readAll());//读取样式表文件
        this->setStyleSheet(styleSheet);//把文件内容传参
        file.close();
    }


//    setStyleSheet("QCheckBox{font-family:arial;font-size:13px;border-radius:2px;color:#000000;}"
//                  "QCheckBox::indicator:checked{color:#FF0000}"
//                  "QTabWidget::pane{border:none;}"
//                  "QTabWidget::tab-bar{alignment:left;}"
//                  "QTabBar::tab{background:transparent;color:#000000;min-width:100px;min-height:35px;}"
//                  "QTabBar::tab:hover{background:rgb(255, 255, 255, 255);}"
//                  "QTabBar::tab:selected{border-color: white;color:green;padding-left:5px}"
//                  "QListWidget{background:rgba(240,240,240,255);color:#000000;border:0px solid gray;padding:0px;}"
//                  "QListWidget::item{width:94px;height:38px;border:0px solid gray;padding-left:24px;}"
//                  "QListWidget::item:selected:active{background:#FFFFFF;color:#19649F;border-width:-1;}"
//                  "QListWidget::item:selected{background:#FFFFFF;color:#19649F;}"
//                  "QLabel,QRadioButton{background:transparent;color:#000000;font-family:arial;font-size:13px;}"

//                  "QComboBox{border:1px solid #d7d7d7; border-radius: 3px; padding: 1px 3px 1px 3px;}"
//                  "QComboBox:editable{background:transparent;}"
//                  "QComboBox:!editable{background: #fbfbfb;color:#666666}"
//                  "QComboBox::down-arrow:on {top: 1px;left: 1px;}"
//                  "QComboBox QAbstractItemView::item{max-width:30px;min-height:20px;}"
//                  );


    rectMove = QRect(0, 0, width(), TITLE_HEIGHT);   // 设置标题栏大小

    /*选项卡初始化*/
    tabWidget = new QTabWidget(this);
    tabWidget->setFocusPolicy(Qt::NoFocus);   //
    QWidget *tab1 = new QWidget(this);
    //tab1->setStyleSheet("background:#FFFFFF");
    QWidget *tab2 = new QWidget(this);
    //tab2->setStyleSheet("background:#FFFFFF");
    QWidget *tab3 = new QWidget(this);
    //tab3->setStyleSheet("background:#FFFFFF");
    tabWidget->addTab(tab1, QIcon(":/qqSetUp/Image/OPTIONS"),tr("base setting"));
    tabWidget->addTab(tab2, QIcon(":/home.png"), tr("secure setting"));
    tabWidget->addTab(tab3, QIcon(":/qqSetUp/Image/LOCK"), tr("power setting"));
    tabWidget->setCurrentIndex(0);

    /*目录列表初始化*/
    contentsWidget = new QListWidget(this);
    contentsWidget->setFocusPolicy(Qt::NoFocus);
    connect(contentsWidget, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(slotItemClicked(QListWidgetItem*)));

    /*滚动窗口初始化*/
    scrollArea = new QScrollArea(this);
//    scrollArea->setStyleSheet("QScrollArea{background:transparent;}"
//                              "QScrollBar::vertical{background:#35A739;border:-5px solid grey;margin:-2px 0px 0px 0px;width:8px;}"
//                              "QScrollBar::horizontal{background:#35A739;border:0px solid #126691;height:10px;}"
//                              "QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {background:#D3D3D3;}"
//                              );

    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scrollArea->setFocusPolicy(Qt::NoFocus);
    connect(scrollArea->verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(slotValueChanged(int))); // 滚动事件
   // connect(scrollArea, SIGNAL(valueChanged(int)), this, SLOT(slotValueChanged(int)));
    connect(tabWidget, SIGNAL(currentChanged(int)), this, SLOT(slotCurrentChanged(int)));  // 选项卡改变事件

    slotCurrentChanged(0);

    // minButton = new EPushButton(this);
    // minButton->setPixName(":/min");
    // minButton->setToolTip(tr("minimize"));
    minButton = new QPushButton(this);   // 最小化按钮
    connect(minButton, SIGNAL(clicked()), parent, SLOT(showMinimized()));

    //closeButton = new EPushButton(this);
    // closeButton->setPixName(":/qqSetup/close");
    // closeButton->setToolTip(tr("close"));
    closeButton = new QPushButton(this);   // 关闭按钮
    closeButton->setIcon(QIcon(":/qqSetup/close"));
    connect(closeButton, SIGNAL(clicked()), parent, SLOT(close()));
}

SettingPanel::~SettingPanel()
{

}


void SettingPanel::resizeEvent(QResizeEvent *event)
{
    tabWidget->setGeometry(0, TAB_WIDGET_TOP, this->width(), this->height() - TAB_WIDGET_TOP);  // 设置选项卡窗口的绘制位置和大小
    contentsWidget->setGeometry(0, CONTENTS_TOP, CONTENTS_WIDTE, this->height() - CONTENTS_TOP);       // 设置目录窗口的绘制位置和大小
    scrollArea->setGeometry(SCROLL_LEFT, SCROLL_TOP, this->width() - SCROLL_LEFT, this->height() - SCROLL_TOP);  // 设置滚动窗口的绘制位置和大小
    minButton->move(width() - 27 - closeButton->width(), 6);            // 移动最小化窗口按钮的位置
    closeButton->move(width() - 27, 6);                                 // 移动关闭窗口按钮的位置
    //qDebug() << scrollArea->width();
    //move((QApplication::desktop()->width() - width())/2,  (QApplication::desktop()->height() - height())/2);
    QWidget::resizeEvent(event);
}

void SettingPanel::initTabOneWidget()
{
    /************loginWidget**************/
    int X_SpacePos;
    int Y_SpacePos;
    int Y_WidgetSpacePos = Y_WIDGET_START_POS;

    ItemDelegate* Delegate;
    mydataWidgetMapper*  dataWidgetMapper;
    for (int listPos = 0; listPos < m_ModelsSort->size(); ++listPos) {
        QStandardItemModel *model = m_Models->value(m_ModelsSort->at(listPos));
        if(model == NULL)continue;
        X_SpacePos = X_START_POS;
        Y_SpacePos = Y_START_POS;
        QWidget  *Widget = new QWidget(widgetScrollArea);
        //Widget->setStyleSheet("background:transparent");
        Widget->show();
        QLabel *contentsLabel = new QLabel(Widget);
        contentsLabel->setText(m_ModelsSort->at(listPos));
        contentsLabel->setGeometry(X_SpacePos, Y_SpacePos,500,contentsLabel->height());
        contentsLabel->setStyleSheet("background:#8DB1E2");
        contentsLabel->show();

        QListWidgetItem *contentsItem = new QListWidgetItem(m_ModelsSort->at(listPos));
        contentsItem->setData(Qt::UserRole,Y_WidgetSpacePos);
        contentsWidget->addItem(contentsItem);

        dataWidgetMapper = new mydataWidgetMapper(Widget);
        Delegate = new ItemDelegate(Widget);
        dataWidgetMapper->setItemDelegate(Delegate);
        dataWidgetMapper->setModel(model);

        Y_SpacePos += Y_SPACE;

        for (int row = 0; row < model->rowCount(); ++row) {
            X_SpacePos = X_START_POS;
            for (int col = 0; col <model->columnCount() ; ++col) {

                QLabel *NameLabel = NULL;
                if(model->index(row,col).data(NameRole).isValid()
                        && model->index(row,col).data(EditTypeRole).toInt() != CheckBox )
                {
                    NameLabel = new QLabel(Widget);
                    NameLabel->setText(model->index(row,col).data(NameRole).toString());
                    NameLabel->adjustSize();
                    NameLabel->setGeometry(X_SpacePos, Y_SpacePos,NameLabel->width()+10,NameLabel->height());

                    NameLabel->show();
                }

                if(model->index(row,col).data(EditTypeRole).toInt() == ComboBox)
                {
                    QComboBox *combox = new QComboBox(Widget);
                    dataWidgetMapper->addMapping(combox,row,col);
                    //combox->setFocusPolicy(Qt::NoFocus);
                    combox->move(NameLabel?X_SpacePos+NameLabel->width():X_SpacePos, Y_SpacePos);
                    combox->show();
                }
                else  if(model->index(row,col).data(EditTypeRole).toInt() == CheckBox)
                {
                    QCheckBox *checkBox = new QCheckBox(spaceWidget);
                    dataWidgetMapper->addMapping(checkBox,row,col);
                    //checkBox->setFocusPolicy(Qt::NoFocus);
                    checkBox->setText(model->index(row,col).data(NameRole).toString());
                    checkBox->move(X_SpacePos, Y_SpacePos);
                    checkBox->show();
                }
                else  if(model->index(row,col).data(EditTypeRole).toInt() == LineEdit)
                {
                    QLineEdit *lineEdit = new QLineEdit(Widget);
                    dataWidgetMapper->addMapping(lineEdit,row,col);
                    //lineEdit->setFocusPolicy(Qt::NoFocus);
                    lineEdit->setGeometry(NameLabel?X_SpacePos+NameLabel->width():X_SpacePos, Y_SpacePos,LINEEDIT_WIDTH,lineEdit->height());
                    lineEdit->show();
                }
                else
                {
                    QLabel*  valLabel = new QLabel(Widget);
                    dataWidgetMapper->addMapping(valLabel,row,col,"text");
                    valLabel->setGeometry(NameLabel?X_SpacePos+NameLabel->width():X_SpacePos, Y_SpacePos,LINEEDIT_WIDTH,valLabel->height());
                    //valLabel->adjustSize();
                    valLabel->show();
                }

                X_SpacePos += X_SPACE;
            }
            Y_SpacePos += Y_SPACE;
        }
        Widget->setGeometry(0,Y_WidgetSpacePos,500,Y_SpacePos);
        Y_WidgetSpacePos += Y_SpacePos;
        dataWidgetMapper->revert();
    }

    widgetScrollArea->resize(0,Y_WidgetSpacePos);
    if(Y_WidgetSpacePos > scrollArea->height())
    {
        widgetScrollArea->resize(500, Y_WidgetSpacePos );
    }
    else
    {
        widgetScrollArea->resize(500, Y_WidgetSpacePos +  scrollArea->height());
    }


}

void SettingPanel::initTabTwoWidget()
{
//    /************passwordWidget**************/
//    passwordWidget = new QWidget(widgetScrollArea);
//    passwordWidget->show();
//    QLabel *label = new QLabel(passwordWidget);
//    label->setText(tr("password:"));
//    label->setGeometry(30, 10, 50, 30);
//    label->show();
//    QLabel *first = new QLabel(passwordWidget);
//    first->setText(tr("do not remember the password:"));
//    first->setGeometry(100, 10, 300, 30);
//    first->show();
//    QPushButton *second = new QPushButton(passwordWidget);
//    second->setStyleSheet("QPushButton{border:1px solid lightgray;background:rgb(230,230,230);border-radius:3px;}"
//                          "QPushButton:hover{border-color:green;background:#bee7fd;}");
//    second->setFocusPolicy(Qt::NoFocus);
//    second->setText(tr("cancel to remember password"));
//    second->setGeometry(100, 40, 90, 25);
//    second->show();
//    QLabel *third = new QLabel(passwordWidget);
//    third->setText(tr("make sure safe,please alter password regularly"));
//    third->setGeometry(100, 70, 300, 30);
//    third->show();
//    QPushButton *forth = new QPushButton(passwordWidget);
//    forth->setStyleSheet("QPushButton{border:1px solid lightgray;background:rgb(230,230,230);border-radius:3px;}"
//                         "QPushButton:hover{border-color:green;color:#bee7fd;}");
//    forth->setFocusPolicy(Qt::NoFocus);
//    forth->setText(tr("alter password"));
//    forth->setGeometry(100, 100, 60, 25);
//    forth->show();
//    QPushButton *forths = new QPushButton(passwordWidget);
//    forths->setStyleSheet("QPushButton{border:1px solid lightgray;background:rgb(230,230,230);border-radius:3px;}"
//                          "QPushButton:hover{border-color:green;color:#bee7fd;}");
//    forths->setFocusPolicy(Qt::NoFocus);
//    forths->setText(tr("apply for protecting password"));
//    forths->setGeometry(170, 100, 90, 25);
//    forths->show();
//    QLabel *five = new QLabel(passwordWidget);
//    five->setText(tr("some servers will use special password"));
//    five->setGeometry(100, 130, 300, 30);
//    five->show();
//    QPushButton *sixth = new QPushButton(passwordWidget);
//    sixth->setStyleSheet("QPushButton{border:1px solid lightgray;background:rgb(240,240,240);border-radius:3px;}"
//                         "QPushButton:hover{border-color:green;color:#bee7fd;}");
//    sixth->setFocusPolicy(Qt::NoFocus);
//    sixth->setText(tr("manager special password"));
//    sixth->setGeometry(100, 160, 90, 25);
//    sixth->show();
//    passwordWidget->setGeometry(0, 0, 555, 190);
//    /************qqlockWidget**************/
//    qqlockWidget = new QWidget(widgetScrollArea);
//    qqlockWidget->show();
//    QLabel *qlabel = new QLabel(qqlockWidget);
//    qlabel->setText(tr("qqlock:"));
//    qlabel->setGeometry(30, 10, 50, 30);
//    qlabel->show();
//    QRadioButton *firstRadio = new QRadioButton(qqlockWidget);
//    firstRadio->setFocusPolicy(Qt::NoFocus);
//    firstRadio->setChecked(true);
//    firstRadio->setText(tr("qq password is used to unlock"));
//    firstRadio->setGeometry(100, 10, 300, 30);
//    firstRadio->show();
//    QRadioButton *secondRadio = new QRadioButton(qqlockWidget);
//    secondRadio->setFocusPolicy(Qt::NoFocus);
//    secondRadio->setText(tr("special password is used to unlock"));
//    secondRadio->setGeometry(100, 40, 300, 30);
//    secondRadio->show();
//    qqlockWidget->setGeometry(0, 190, 555, 70);
//    widgetScrollArea->resize(555, 190+70);

//    if(70 > scrollArea->height())
//    {
//        widgetScrollArea->resize(555, 190 );
//    }
//    else
//    {
//        widgetScrollArea->resize(555, 190 +  scrollArea->height());
//    }

}

void SettingPanel::initTabThreeWidget()
{
//    /************loginWidget**************/
//    spaceWidget = new QWidget(widgetScrollArea);
//    spaceWidget->show();
//    QLabel *label = new QLabel(spaceWidget);
//    label->setText(tr("spacepermission:"));
//    label->setGeometry(30, 10, 60, 30);
//    label->show();
//    QCheckBox *first = new QCheckBox(spaceWidget);
//    first->setFocusPolicy(Qt::NoFocus);
//    first->setChecked(true);
//    first->setText(tr("newly log and photos display on qq"));
//    first->setGeometry(100, 10, 300, 30);
//    first->show();
//    QCheckBox *second = new QCheckBox(spaceWidget);
//    second->setFocusPolicy(Qt::NoFocus);
//    second->setText(tr("update sign when updating chat"));
//    second->setGeometry(100, 40, 300, 30);
//    second->show();
//    QCheckBox *third = new QCheckBox(spaceWidget);
//    third->setFocusPolicy(Qt::NoFocus);
//    third->setChecked(true);
//    third->setText(tr("notice you when you log is republished"));
//    third->setGeometry(100, 70, 300, 30);
//    third->show();
//    QCheckBox *forth = new QCheckBox(spaceWidget);
//    forth->setFocusPolicy(Qt::NoFocus);
//    forth->setChecked(true);
//    forth->setText(tr("notice you when you have new tips"));
//    forth->setGeometry(100, 100, 300, 30);
//    forth->show();
//    QCheckBox *five = new QCheckBox(spaceWidget);
//    five->setFocusPolicy(Qt::NoFocus);
//    five->setChecked(true);
//    five->setText(tr("display your newly application"));
//    five->setGeometry(100, 130, 300, 30);
//    five->show();
//    spaceWidget->setGeometry(0, 0, 555, 160);
//    widgetScrollArea->resize(555, 160);

//    if(0 > scrollArea->height())
//    {
//        widgetScrollArea->resize(555, 0 );
//    }
//    else
//    {
//        widgetScrollArea->resize(555, 0 +  scrollArea->height());
//    }
}

void SettingPanel::slotCurrentChanged(int index)
{
    contentsWidget->clear();     // 清除目录窗口
    scrollArea->takeWidget();    //

    loginWidget = NULL;
    panelWidget = NULL;
    statusWidget = NULL;
    sessionWidget = NULL;
    passwordWidget = NULL;
    qqlockWidget = NULL;
    spaceWidget = NULL;

    widgetScrollArea = new QWidget(this);
    //widgetScrollArea->setStyleSheet("background:transparent;");

    scrollArea->setWidget(widgetScrollArea);
    if (index == 0) {
//        contentsWidget->addItem(tr("login"));
//        contentsWidget->addItem(tr("mainpanel"));
//        contentsWidget->addItem(tr("status"));
//        contentsWidget->addItem(tr("session"));
//        QMap<QString, QStandardItemModel *>::const_iterator i = m_Models->constBegin();
//        while (i != m_Models->constEnd()) {
//            contentsWidget->addItem(i.key());
//            i++;
//        }
        initTabOneWidget();
    }
    else if (index == 1){
        contentsWidget->addItem(tr("password"));
        contentsWidget->addItem(tr("qqlock"));
        initTabTwoWidget();
    }
    else if (index == 2) {
        contentsWidget->addItem(tr("spacepermission"));
        initTabThreeWidget();
    }
    if (contentsWidget->count() > 0) {
        contentsWidget->setCurrentRow(0);
    }
}

void SettingPanel::slotItemClicked(QListWidgetItem *item)
{
    signFlag = true ;
    QString itemText = item->text();
    QPoint widgetPos;
    if (tabWidget->currentIndex() == 0) {
        scrollArea->verticalScrollBar()->setSliderPosition(item->data(Qt::UserRole).toInt());
    }
    else if (tabWidget->currentIndex() == 1) {
        if (itemText == tr("password")) {
            widgetPos = passwordWidget->pos();
            scrollArea->verticalScrollBar()->setSliderPosition(widgetPos.y());
        }
        else if (itemText == tr("qqlock")) {
            widgetPos = qqlockWidget->pos();
            scrollArea->verticalScrollBar()->setValue(widgetPos.y());
        }
    }
}

void SettingPanel::slotValueChanged(int value)
{
    if (tabWidget->currentIndex() == 0) {
        for (int itemPos = 0; itemPos < contentsWidget->count(); ++itemPos) {
            if(value >= contentsWidget->item(itemPos)->data(Qt::UserRole).toInt())
            {
                contentsWidget->item(itemPos)->setSelected(true);
            }
        }
    }
    else if (tabWidget->currentIndex() == 1) {
        QListWidgetItem *passwordItem = contentsWidget->item(0);
        QListWidgetItem *qqlockItem = contentsWidget->item(1);
        if (!signFlag) {
            if (passwordWidget && qqlockWidget) {
                if (!passwordWidget->visibleRegion().isEmpty()) {
                    passwordItem->setSelected(true);
                    return;
                }
                else {
                    passwordItem->setSelected(false);
                }
                if (!qqlockWidget->visibleRegion().isEmpty()) {
                    qqlockItem->setSelected(true);
                    return;
                }
                else {
                    qqlockItem->setSelected(false);
                }
            }
        }
    }
    signFlag = false;
}

void SettingPanel::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor("#F0F0F0")); // 将整个背景渲染成灰白色
    painter.drawRect(this->rect());

    painter.setBrush(QColor("#069dd5")); //  标题背景色为蓝色
    painter.drawRect(rectMove);/*画标题栏*/

    painter.setPen(QColor("#D7D7D7"));     // 目录栏背景色
    painter.drawLine(0, 70, this->width(), 70);
    QWidget::paintEvent(event);
}

void SettingPanel::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton && rectMove.contains(event->pos())) {
        mousePress = true;
    }
    movePoint = event->globalPos() - pos();
    QWidget::mousePressEvent(event);
}

void SettingPanel::mouseReleaseEvent(QMouseEvent *event)
{
    mousePress = false;
    QWidget::mouseReleaseEvent(event);
}

void SettingPanel::mouseMoveEvent(QMouseEvent *event)
{
    if(mousePress) {
        QPoint movePos = event->globalPos();
        move(movePos - movePoint);
    }
    QWidget::mouseMoveEvent(event);
}
