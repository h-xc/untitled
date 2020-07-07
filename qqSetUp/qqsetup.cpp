#include "qqsetup.h"
#include "ui_qqsetup.h"

#include <QDomDocument>
#include <QFile>
#include <QDebug>
#include <QMap>
#include <QToolTip>
#include <QObject>
#include <QMetaEnum>
#include "..\\Public\\ModelView\\item_delegate.h"

#define  XML_FILE_NAME     ".\\FaultFixVal.xml"


qqSetUp::qqSetUp(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::qqSetUp)
{
    ui->setupUi(this);

    dataStore =new xmlDataStore(parent);
    connect(this,SIGNAL(valChanged()),this,SLOT(valSlot()));
    dataStore->xmlFileName =XML_FILE_NAME;
    dataStore->fetchOfStore();
    ui->widget->setModles(&dataStore->m_Models);
    setProperty("val1",8);
    qDebug() <<property("val1");
    //mstruct.val1 = 5;

}

qqSetUp::~qqSetUp()
{
    delete ui;
}
