#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QQuickView>
#include "./qqSetUp/qqsetup.h"
#include "./LayoutTest/layouttest.h"
#include "./sysSet/sysset.h"
#include "./WinCMD/winCmd.h"
#include "./Socket/socket.h"
#include "./LayoutForm/layoutform.h"
#include "./StruOffse/struoffse.h"
#include "./SetTime/SetTime.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_widgetCount(0),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_GLayout = qobject_cast<QGridLayout *>(ui->centralWidget->layout());
    QWidget *pWidget;

    pWidget = new qqSetUp();
    pWidget->setWindowTitle(tr("QQ设置面板"));
    addWidget(pWidget);

    pWidget = new LayoutTest();
    pWidget->setWindowTitle(tr("UI布局测试"));
    addWidget(pWidget);

    pWidget = new sysSet();
    pWidget->setWindowTitle(tr("系统设置"));
    addWidget(pWidget);

    //
    QPushButton *qPushButton = new QPushButton(tr("QML测试"),this);
    connect(qPushButton,SIGNAL(clicked(bool)),this,SLOT(on_QMLSet_clicked()));
    m_GLayout->addWidget(qPushButton,m_widgetCount/COLNUM_COUNT,m_widgetCount%COLNUM_COUNT);
    m_widgetCount++;

    pWidget = new WinCMD();
    pWidget->setWindowTitle(tr("WinCMD"));
    addWidget(pWidget);

    pWidget = new Socket();
    pWidget->setWindowTitle(tr("Socket"));
    addWidget(pWidget);

    pWidget = new layoutForm();
    pWidget->setWindowTitle(tr("布局间隔"));
    addWidget(pWidget);

    qPushButton = new QPushButton(tr("代码测试"),this);
    connect(qPushButton,SIGNAL(clicked(bool)),this,SLOT(on_codeTest_clicked()));
    m_GLayout->addWidget(qPushButton,m_widgetCount/COLNUM_COUNT,m_widgetCount%COLNUM_COUNT);
    m_widgetCount++;


    pWidget = new SelDateTimeWidget();
    pWidget->setWindowTitle(tr("时间设置"));
    addWidget(pWidget);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::addWidget(QWidget *pWidget)
{
    QPushButton *qPushButton = new QPushButton(pWidget->windowTitle(),this);
    connect(qPushButton,SIGNAL(clicked(bool)),this,SLOT(on_pushButton_clicked()));
    qPushButton->setProperty("showWidget",QVariant::fromValue<void*>(pWidget));
    m_GLayout->addWidget(qPushButton,m_widgetCount/COLNUM_COUNT,m_widgetCount%COLNUM_COUNT);
    m_widgetCount++;
}

void MainWindow::on_pushButton_clicked()
{
    QPushButton *qPushButton = qobject_cast<QPushButton *>(sender());
    QWidget *widget = (QWidget *)qPushButton->property("showWidget").value<void*>();
    //widget->setAttribute(Qt::WA_DeleteOnClose);
    widget->show();
}

void MainWindow::on_QMLSet_clicked()
{
    QQuickView *viewer =new QQuickView();
    viewer->setResizeMode(QQuickView::SizeRootObjectToView);
    viewer->setSource(QUrl("qrc:/qmlTest.qml"));
    viewer->show();
}

void MainWindow::on_codeTest_clicked()
{
    struOffse stru;
}
