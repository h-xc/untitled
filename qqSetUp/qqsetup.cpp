#include "qqsetup.h"
#include "ui_qqsetup.h"

#include <QDomDocument>
#include <QFile>
#include <QDebug>
#include <QMap>
#include <QToolTip>
#include "..\\Public\\ModelView\\item_delegate.h"

#define  XML_FILE_NAME     ".\\FaultFixVal.xml"

qqSetUp::qqSetUp(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::qqSetUp)
{
    ui->setupUi(this);

    dataStore =new xmlDataStore(parent);
    dataStore->xmlFileName =XML_FILE_NAME;
    dataStore->fetchOfStore();

    ui->widget->setModles(&dataStore->m_Models);
//    ui->faultSetModelView->setModel(dataStore->m_Models[0]->model);
//    ui->lineSetModelView->setModel(dataStore->m_Models[1]->model);
//    ui->KISetModelView->setModel(dataStore->m_Models[2]->model);
//    ui->KOSetModelView->setModel(dataStore->m_Models[3]->model);
}

qqSetUp::~qqSetUp()
{
    delete ui;
}
