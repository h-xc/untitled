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
    this->setStyleSheet("QScrollArea{background:#FFFFFF;}"
                        "QWidget#scrollAreaWidgetContents{background-color:#FFFFFF;border-color:#FFFFFF;}");

}

LayoutTest::~LayoutTest()
{
    delete ui;
}
