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
    fetchOfStore();
    ui->setupUi(this);
    //ui->widget->m_Models = &m_Models;

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
        if(!Interface.attribute("Name").isEmpty())     item->setData(Interface.attribute("Name"),NameRole);
        if(!Interface.attribute("Desc").isEmpty())
        {
            item->setData(Interface.attribute("Desc"),DescRole);
            //item->setData(Interface.attribute("Desc"),Qt::ToolTipRole);
        }
        if(!Interface.attribute("ShowType").isEmpty()) item->setData(Interface.attribute("ShowType"),ShowTypeRole);
        if(!Interface.attribute("CheckType").isEmpty())item->setData(Interface.attribute("CheckType"),CheckTypeRole);
        if(!Interface.attribute("EditType").isEmpty()) item->setData(Interface.attribute("EditType"),EditTypeRole);
        if(!Interface.attribute("Regexp").isEmpty())   item->setData(Interface.attribute("Regexp"),RegexpRole);
        if(Interface.attribute("Editable") == "false")
        {
            item->flags() &= ~Qt::ItemIsEditable;
        }

        QDomElement Val =  Interface.nextSiblingElement();
        if(!Val.attribute("Unint").isEmpty())    item->setData(Val.attribute("Unint"),UnitRole);
        if(!Val.attribute("DataType").isEmpty()) item->setData(Val.attribute("DataType"),DataTypeRole);
        if(!Val.attribute("RangMin").isEmpty())  item->setData(Val.attribute("RangMin"),RangMinRole);
        if(!Val.attribute("RangMax").isEmpty())  item->setData(Val.attribute("RangMax"),RangMaxRole);
        if(!Val.attribute("Step").isEmpty())     item->setData(Val.attribute("Step"),StepRole);
        if(!Val.attribute("Decimal").isEmpty())  item->setData(Val.attribute("Decimal"),DecimalRole);

        if(!Val.text().isEmpty())
        {
            if(item->data(DataTypeRole).toString() ==  "float")
            {
                item->setData(QString().setNum(Val.text().toFloat(),'f',item->data(DecimalRole).toInt()),Qt::EditRole);
            }else
            {
                item->setData(Val.text(),Qt::EditRole);
            }
        }

        QDomElement xmlCombo =  Val.nextSiblingElement();
        QMap<QString,QVariant> mapCombo;
        while(!xmlCombo.isNull())
        {
            mapCombo.insert(xmlCombo.attribute("InterfaceName"),xmlCombo.text());
            xmlCombo = xmlCombo.nextSiblingElement();
        }
        if(!mapCombo.isEmpty())  item->setData(mapCombo,ComboMapRole);
    }
    return true;
}
qqSetUp::~qqSetUp()
{
    delete ui;
}
