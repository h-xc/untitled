#include "toolbar.h"
#include "ui_toolbar.h"

toolbar::toolbar(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::toolbar)
{
    ui->setupUi(this);
}

toolbar::~toolbar()
{
    delete ui;
}
