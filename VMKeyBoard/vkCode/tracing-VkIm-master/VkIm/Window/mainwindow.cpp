#include "mainwindow.h"
#include "ui_mainwindow.h"

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

void MainWindow::on_pushButton_clicked()
{
    this->showNormal();
}

void MainWindow::on_pushButton_2_clicked()
{
    Qt::WindowFlags flags = this->windowFlags();
    flags &= ~Qt::WindowStaysOnTopHint;
    this->setWindowFlags(flags);
    this->showFullScreen();
}
