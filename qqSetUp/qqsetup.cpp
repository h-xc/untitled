#include "qqsetup.h"
#include "ui_qqsetup.h"

#include <QDomDocument>
#include <QFile>
#include <QDebug>

#include "..\\Public\\ModelView\\item_delegate.h"

#define  XML_FILE_NAME     ".\\FaultFixVal.xml"

qqSetUp::qqSetUp(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::qqSetUp)
{
    ui->setupUi(this);
    fetchOfStore();

    ui->KISetModelView->setModel(m_Models.value("KISetModel"));
    ui->KISetModelView->setItemDelegate(new ItemDelegate(this));
    ui->KOSetModelView->setModel(m_Models.value("KOSetModel"));
    ui->KOSetModelView->setItemDelegate(new ItemDelegate(this));
    ui->faultSetModelView->setModel(m_Models.value("faultSetModel"));
    ui->faultSetModelView->setItemDelegate(new ItemDelegate(this));

    ui->lineSetModelView->setModel(m_Models.value("lineSetModel"));
    ui->lineSetModelView->setItemDelegate(new ItemDelegate(this));

}
bool qqSetUp::fetchOfStore()
{
    QDomDocument doc;
    QFile file(XML_FILE_NAME);
    if( !file.open(QFile::ReadOnly | QFile::Text))
    {
        qDebug() << QObject::tr("error::ParserXML->OpenXmlFile->file.open->%s\n") << XML_FILE_NAME;

        return false;
    }
    if( !doc.setContent(&file))
    {
        qDebug() << QObject::tr("error::ParserXML->OpenXmlFile->doc.setContent\n") << XML_FILE_NAME;
        file.close();
        return false;
    }

    file.close();

    QStandardItem *item ;
    QStandardItemModel* WidgetModel;

    QDomElement DataBase = doc.documentElement(); //返回根节点
    for(QDomElement Data = DataBase.firstChildElement(); !Data.isNull(); Data = Data.nextSiblingElement())  //如果节点不空
    {
        if(m_Models.find(Data.attribute("ModelName")) == m_Models.end())  // 判断的当前模型是否已经创建,若未创建，则创建并填入到
        {
            m_Models.insert(Data.attribute("ModelName"),new QStandardItemModel(this));
        }

        WidgetModel = (QStandardItemModel*)(m_Models.value(Data.attribute("ModelName")));
        if(WidgetModel == 0)return false;


        if(WidgetModel->rowCount() <= Data.attribute("ItemRow").toInt())
        {
            WidgetModel->setRowCount(Data.attribute("ItemRow").toInt()+1);
        }
        if(WidgetModel->columnCount() <= Data.attribute("ItemCol").toInt())
        {
            WidgetModel->setColumnCount(Data.attribute("ItemCol").toInt()+1);
        }
        //qDebug() << Data.attribute("ItemRow") + "," + Data.attribute("ItemCol");

        item = new QStandardItem(Data.attribute("ItemRow") + "," + Data.attribute("ItemCol"));
        WidgetModel->setItem(Data.attribute("ItemRow").toInt(),Data.attribute("ItemCol").toInt(),item);


        item->setData(Data.attribute("DataName"),VarNameRole);
        QDomElement Interface = Data.firstChildElement(); //获得第一个子节点

        item->setData(Interface.attribute("Name"),NameRole);
        item->setData(Interface.attribute("Desc"),DescRole);
        item->setData(Interface.attribute("ShowType"),ShowTypeRole);
        item->setData(Interface.attribute("CheckType"),CheckTypeRole);
        item->setData(Interface.attribute("EditType"),EditTypeRole);
        item->setData(Interface.attribute("Regexp"),RegexpRole);
        if(Interface.attribute("Editable") == "false")
        {
            item->flags() &= ~Qt::ItemIsEditable;
        }

        QDomElement Val =  Interface.nextSiblingElement();
        item->setData(Val.attribute("Unint"),UnitRole);
        item->setData(Val.attribute("DataType"),DataTypeRole);
        item->setData(Val.attribute("RangMin"),RangMinRole);
        item->setData(Val.attribute("RangMax"),RangMaxRole);
        item->setData(Val.attribute("Step"),StepRole);
        item->setData(Val.attribute("Decimal"),DecimalRole);
        qDebug() << Val.text();
        item->setData(Val.text(),Qt::EditRole);
    }
    return true;
}
qqSetUp::~qqSetUp()
{
    delete ui;
}
