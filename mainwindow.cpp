#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "./qqSetUp/qqsetup.h"

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
   qqSetUp *widget = new qqSetUp();
   widget->setAttribute(Qt::WA_DeleteOnClose);
   widget->show();
}
