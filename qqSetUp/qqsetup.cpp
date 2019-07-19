#include "qqsetup.h"
#include "ui_qqsetup.h"

qqSetUp::qqSetUp(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::qqSetUp)
{
    ui->setupUi(this);
}

qqSetUp::~qqSetUp()
{
    delete ui;
}
