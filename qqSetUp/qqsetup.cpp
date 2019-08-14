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
    ui->widget->setModles(&m_Models);
    ui->KISetModelView->setModel(m_Models.at(0)->model);
    ui->KISetModelView->setItemDelegate(new ItemDelegate(this));
    ui->KOSetModelView->setModel(m_Models.at(1)->model);
    ui->KOSetModelView->setItemDelegate(new ItemDelegate(this));
    ui->faultSetModelView->setModel(m_Models.at(2)->model);
    ui->faultSetModelView->setItemDelegate(new ItemDelegate(this));
    ui->lineSetModelView->setModel(m_Models.at(3)->model);
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
    QStandardItemModel* widgetModel;
    m_Models.clear();


    QDomElement DataBase = doc.documentElement(); //返回根节点
    for(QDomElement Model = DataBase.firstChildElement(); !Model.isNull(); Model = Model.nextSiblingElement())  //如果节点不空
    {
        if(Model.nodeName() != "Model") continue;
        ModelStruct *modelStruct = new ModelStruct;
        modelStruct->Name = Model.attribute("Name");
        modelStruct->View = Model.attribute("View");
        modelStruct->Type = Model.attribute("Type");
        modelStruct->Content = Model.attribute("Content");
        widgetModel = new QStandardItemModel(this);
        modelStruct->model = widgetModel;
        m_Models.append(modelStruct);

        for(QDomElement Data = Model.firstChildElement(); !Data.isNull(); Data = Data.nextSiblingElement())  //如果节点不空
        {
            if(Data.nodeName() != "Data")
            {
                if(Data.nodeName() == "Header")  // 显示列表头
                {
                    int H_sectionPos = 0;
                    int V_sectionPos = 0;

                    for (QDomElement Header = Data.firstChildElement(); !Header.isNull(); Header = Header.nextSiblingElement())
                    {
                        if(Header.nodeName() == "Horizontal")
                        {
                            widgetModel->setColumnCount(H_sectionPos+1);
                            widgetModel->setHeaderData(H_sectionPos,Qt::Horizontal,Header.text());
                            H_sectionPos ++;
                        }
                        else  if(Header.nodeName() == "Vertical")
                        {
                            widgetModel->setRowCount(V_sectionPos+1);
                            widgetModel->setHeaderData(V_sectionPos,Qt::Vertical,Header.text());
                            V_sectionPos ++;
                        }
                    }
                }
                continue;
            }

            int ItemRow = Data.attribute("ItemRow").toInt();
            int ItemCol = Data.attribute("ItemCol").toInt();

            if(widgetModel->item(ItemRow,ItemCol) != NULL)  // 有重复的数据在模型中进行了映射
            {
                if(widgetModel->item(ItemRow,ItemCol)->data(VarNameRole).isValid())
                {
                    qDebug() << Data.attribute("ItemRow") << "," << Data.attribute("ItemCol")
                             << ":" <<widgetModel->item(ItemRow,ItemCol)->data(VarNameRole).toString()
                             << " repetition item";
                    continue;
                }
            }

            if(widgetModel->rowCount() <= ItemRow)  // 使用扩充行的方式，xml文件，可以防止因为元素顺序问题造成的死机问题
            {
                widgetModel->setRowCount(ItemRow+1);
            }
            if(widgetModel->columnCount() <= ItemCol)
            {
                widgetModel->setColumnCount(ItemCol+1);
            }

            item = new QStandardItem;
            widgetModel->setItem(Data.attribute("ItemRow").toInt(),Data.attribute("ItemCol").toInt(),item);

            item->setData(Data.attribute("DataName"),VarNameRole);

            QDomElement Interface = Data.firstChildElement();     //获得第一个子节点
            if(!Interface.attribute("Name").isEmpty())     item->setData(Interface.attribute("Name"),NameRole);
            if(!Interface.attribute("Desc").isEmpty())
            {
                item->setData(Interface.attribute("Desc"),DescRole);
                item->setData(Interface.attribute("Desc"),Qt::ToolTipRole);  // 不起作用？
            }
            if(!Interface.attribute("ShowType").isEmpty()) item->setData(Interface.attribute("ShowType"),ShowTypeRole);
            if(!Interface.attribute("CheckType").isEmpty())item->setData(Interface.attribute("CheckType"),CheckTypeRole);
            if(!Interface.attribute("EditType").isEmpty()) item->setData(Interface.attribute("EditType"),EditTypeRole);
            if(!Interface.attribute("Regexp").isEmpty())   item->setData(Interface.attribute("Regexp"),RegexpRole);
            if(!Interface.attribute("Content").isEmpty())  item->setData(Interface.attribute("Content"),ContentRole);
            if(Interface.attribute("Editable") == "false")
            {
               Qt::ItemFlags flags =  item->flags();
               flags &= ~Qt::ItemIsEditable;
               item->setFlags(flags);
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
                if(xmlCombo.nodeName() != "Combo") continue;
                mapCombo.insert(xmlCombo.attribute("InterfaceName"),xmlCombo.text());
                xmlCombo = xmlCombo.nextSiblingElement();
            }
            if(!mapCombo.isEmpty())  item->setData(mapCombo,ComboMapRole);
        }
    }
    return true;
}
qqSetUp::~qqSetUp()
{
    delete ui;
}
