#include "layouttest.h"
#include "ui_layouttest.h"

#include <QScrollArea>
#include <QVBoxLayout>
#include <QWidget>
#include <QLabel>
LayoutTest::LayoutTest(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LayoutTest)
{
    ui->setupUi(this);
   QScrollArea  *myScrollArea = new QScrollArea(this);
   myScrollArea->resize(this->width(),this->height());
   QWidget      *myWidget = new QWidget(myScrollArea);
   myScrollArea->setWidget(myWidget);
   myWidget->resize(1000,1000);
   myWidget->setLayout(new QVBoxLayout(myWidget));
   myWidget->layout()->addWidget(new QLabel(myWidget));
}

LayoutTest::~LayoutTest()
{
    delete ui;
}
