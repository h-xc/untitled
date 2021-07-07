#include "layoutform.h"

layoutForm::layoutForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::layoutForm)
{
    ui->setupUi(this);
}

layoutForm::~layoutForm()
{
    delete ui;
}
