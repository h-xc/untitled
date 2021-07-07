#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "./qqSetUp/qqsetup.h"
#include "./LayoutTest/layouttest.h"
#include "./sysSet/sysset.h"
#include "./WinCMD/winCmd.h"
#include "./Socket/socket.h"
#include "./LayoutForm/layoutform.h"
#include "./StruOffse/struoffse.h"

#include <QQuickView>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_qqSetUp_clicked()
{
    QWidget *widget = new qqSetUp;
    widget->setAttribute(Qt::WA_DeleteOnClose);
    widget->show();
}

void MainWindow::on_LayoutTest_clicked()
{
    QWidget *widget = new LayoutTest;
    widget->setAttribute(Qt::WA_DeleteOnClose);
    widget->show();
}

void MainWindow::on_sysSet_clicked()
{
    QWidget *widget = new sysSet;
    widget->setAttribute(Qt::WA_DeleteOnClose);
    widget->show();
}

void MainWindow::on_QMLSet_clicked()
{
    QQuickView *viewer =new QQuickView();
    viewer->setResizeMode(QQuickView::SizeRootObjectToView);
    viewer->setSource(QUrl("qrc:/qmlTest.qml"));
    viewer->show();
}

void MainWindow::on_WinCMD_clicked()
{
    QWidget *widget = new WinCMD;
    widget->setAttribute(Qt::WA_DeleteOnClose);
    widget->show();
}

void MainWindow::on_Socket_clicked()
{
    QWidget *widget = new Socket;
    widget->setAttribute(Qt::WA_DeleteOnClose);
    widget->show();
}

void MainWindow::on_layoutForm_clicked()
{
    QWidget *widget = new layoutForm;
    widget->setAttribute(Qt::WA_DeleteOnClose);
    widget->show();
}

void MainWindow::on_pushButton_clicked()
{
    struOffse stru;
}
