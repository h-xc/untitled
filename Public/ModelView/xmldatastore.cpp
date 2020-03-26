#include "xmldatastore.h"

xmlDataStore::xmlDataStore(QObject *parent):DataStore(parent)
{

}

bool xmlDataStore::createStore()//
{
    return true;
}
bool xmlDataStore::saveToStore()
{
    QDomDocument doc;
    QDomProcessingInstruction xmlInstruction = doc.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"");
    doc.appendChild(xmlInstruction);  // 开始文档（XML 声明）

    // 根元素 <Blogs>
    QDomElement DataBase = doc.createElement(ELEMENT_DATABASE);
    DataBase.setAttribute(ATTR_VERSION, ATTR_VERSION_1_0);  // 储存载体版本
    doc.appendChild(DataBase);


    // 遍历models进行保存
    for (int pos = 0; pos < m_Models.size(); ++pos)
    {
        QDomElement Model = doc.createElement(ELEMENT_MODEL);   // 储存单元描述体
        Model.setAttribute("Name",  m_Models[pos]->Name);
        Model.setAttribute("View",  m_Models[pos]->View);
        Model.setAttribute("Type",  m_Models[pos]->Type);
        Model.setAttribute("ContentRole",  m_Models[pos]->Content);
        // 对表格的列标题和行标题需要进行保存，待完成
        DataBase.appendChild(Model);
        for(int row = 0;row < m_Models[pos]->model->rowCount();row++)
        {
            for(int col = 0;col < m_Models[pos]->model->columnCount();col++)
            {
                QModelIndex ModelIndex = m_Models[pos]->model->index(row,col);
                if(ModelIndex.isValid() && !ModelIndex.data(ItemDelegate::VarNameRole).toString().isEmpty())
                {
                    addXmlData(doc,Model,&ModelIndex);
                }
            }
        }
    }

    // 保存 XML 文件
    QString strFile(xmlFileName);
    QFile file(strFile);
    // 只写模式打开文件
    if (file.open(QFile::WriteOnly | QFile::Text))
    {
        QTextStream out(&file);
        doc.save(out, QDomNode::EncodingFromDocument);
        file.close();
    }
    return true;
}

void xmlDataStore::addXmlData(QDomDocument &doc,QDomElement &root,QModelIndex *item)
{
    QMetaEnum EditTypeEnym      = QMetaEnum::fromType<ItemDelegate::EditType>();    // 枚举值与字符转换
    QMetaEnum ItemUserRoleEnym  = QMetaEnum::fromType<ItemDelegate::ItemUserRole>();
    QMetaEnum CheckTypeEnym     = QMetaEnum::fromType<ItemDelegate::CheckType>();
    QMetaEnum ShowTypeEnym      = QMetaEnum::fromType<ItemDelegate::ShowType>();

    QDomElement Data = doc.createElement(ELEMENT_DATA);   // 储存单元描述体
    root.appendChild(Data);
    Data.setAttribute(ItemUserRoleEnym.valueToKey(ItemDelegate::VarNameRole),  item->data(ItemDelegate::VarNameRole).toString());              // 储存单元名称(英文)
    //Data.setAttribute("ModelType", ModelType);      // 使用的模型类型
    //Data.setAttribute("ModelName", ModelName);      // 使用的模型名称
    Data.setAttribute(ATTR_ITEMROW,   QString("%1").arg(item->row()));     // 处在模型的行位置 0开始
    Data.setAttribute(ATTR_ITEMCOL,   QString("%1").arg(item->column()));  // 处在模型的列位置 0开始

    QDomElement Interface = doc.createElement(ELEMENT_INTERFACE);             // 记录在界面中的显示方式和输入检测
    Data.appendChild(Interface);
    Interface.setAttribute(ItemUserRoleEnym.valueToKey(ItemDelegate::NameRole),      item->data(ItemDelegate::NameRole).toString());      // 显示名称(中文)
    Interface.setAttribute(ItemUserRoleEnym.valueToKey(ItemDelegate::DescRole),      item->data(ItemDelegate::DescRole).toString());      // 描述
    Interface.setAttribute(ItemUserRoleEnym.valueToKey(ItemDelegate::ShowTypeRole),  ShowTypeEnym.valueToKey( item->data(ItemDelegate::ShowTypeRole).toInt()));  // 显示格式
    Interface.setAttribute(ItemUserRoleEnym.valueToKey(ItemDelegate::CheckTypeRole), CheckTypeEnym.valueToKey(item->data(ItemDelegate::CheckTypeRole).toInt())); // 合法性检测
    Interface.setAttribute(ItemUserRoleEnym.valueToKey(ItemDelegate::EditTypeRole),  EditTypeEnym.valueToKey(item->data(ItemDelegate::EditTypeRole).toInt()));  // 编辑器类型
    Interface.setAttribute(ItemUserRoleEnym.valueToKey(ItemDelegate::RegexpRole),    item->data(ItemDelegate::RegexpRole).toString());    // 正则检测
    Interface.setAttribute(ItemUserRoleEnym.valueToKey(ItemDelegate::ContentRole),   item->data(ItemDelegate::ContentRole).toString());    // 标签
    Interface.setAttribute(ATTR_EDITABLE,  (item->flags()&Qt::ItemIsEditable)?"true":"false");  // 是否可编辑

    QDomElement Val = doc.createElement(ELEMENT_VAL);      // 变量值描述体
    Data.appendChild(Val);
    Val.setAttribute(ItemUserRoleEnym.valueToKey(ItemDelegate::UnitRole),  item->data(ItemDelegate::UnitRole).toString());     // 值单位
    Val.setAttribute(ItemUserRoleEnym.valueToKey(ItemDelegate::DataTypeRole),QMetaType::typeName(item->data(ItemDelegate::DataTypeRole).toInt()));  // 值类型
    Val.setAttribute(ItemUserRoleEnym.valueToKey(ItemDelegate::RangMinRole),item->data(ItemDelegate::RangMinRole).toString());  // 值上限
    Val.setAttribute(ItemUserRoleEnym.valueToKey(ItemDelegate::RangMaxRole),item->data(ItemDelegate::RangMaxRole).toString());  // 值下限
    Val.setAttribute(ItemUserRoleEnym.valueToKey(ItemDelegate::StepRole),   item->data(ItemDelegate::StepRole).toString());     // 步进量
    Val.setAttribute(ItemUserRoleEnym.valueToKey(ItemDelegate::DecimalRole),item->data(ItemDelegate::DecimalRole).toString());  // 小时点数
    QDomText ValText = doc.createTextNode(item->data(Qt::EditRole).toString());    // 创建值储存
    Val.appendChild(ValText);       // 值
    Val.setAttribute(ItemUserRoleEnym.valueToKey(ItemDelegate::DefaultValRole),item->data(ItemDelegate::DefaultValRole).toString());  //默认值


    /*值选项*/
    if(item->data(ItemDelegate::ComboMapRole).isValid())
    {
        QMap<QString,QVariant> ComboMap = item->data(ItemDelegate::ComboMapRole).toMap();
        QMap<QString, QVariant>::const_iterator k = ComboMap.constBegin();
        while (k != ComboMap.constEnd())
        {
            QDomElement Combo = doc.createElement(ELEMENT_COMBO);
            Combo.setAttribute(ATTR_INTERFACENAME,k.key());    // 在界面上的显示
            Combo.appendChild(doc.createTextNode(k.value().toString()));
            Data.appendChild(Combo);
            k++;
        }
    }
}

bool xmlDataStore::fetchOfStore()
{
    QMetaEnum EditTypeEnym      = QMetaEnum::fromType<ItemDelegate::EditType>();    // 枚举值与字符转换
    QMetaEnum ItemUserRoleEnym  = QMetaEnum::fromType<ItemDelegate::ItemUserRole>();
    QMetaEnum CheckTypeEnym     = QMetaEnum::fromType<ItemDelegate::CheckType>();
    QMetaEnum ShowTypeEnym      = QMetaEnum::fromType<ItemDelegate::ShowType>();
    QString   strTmp;// 辅助字符，用于临时储存字符

    QDomDocument doc;
    QFile file(xmlFileName);

    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        qDebug() << QObject::tr("xmlDataStore::fetchOfStore  file.open->%s") << xmlFileName << file.errorString();
        return false;
    }
    if(!doc.setContent(&file))
    {
        qDebug() << QObject::tr("xmlDataStore::fetchOfStore  doc.setContent") << xmlFileName;
        file.close();
        return false;
    }
    file.close();

    QStandardItem *item ;
    QStandardItemModel* widgetModel;
    m_Models.clear();

    QDomElement DataBase = doc.documentElement(); //返回根节点
    if(ATTR_VERSION_1_0 != DataBase.attribute(ATTR_VERSION))return false;// 判断当前的XML版本
    for(QDomElement Model = DataBase.firstChildElement(); !Model.isNull(); Model = Model.nextSiblingElement())  //如果节点不空
    {
        if(Model.nodeName() != ELEMENT_MODEL) continue;
        ModelStruct *modelStruct = new ModelStruct;  // 创建模型描述结构体
        modelStruct->Name = Model.attribute("Name");
        modelStruct->View = Model.attribute("View");
        modelStruct->Type = Model.attribute("Type");
        modelStruct->Content = Model.attribute("ContentRole");
        widgetModel = new QStandardItemModel(this);

        modelStruct->model = widgetModel;
          m_Models.append(modelStruct);

        for(QDomElement Data = Model.firstChildElement(); !Data.isNull(); Data = Data.nextSiblingElement())  //如果节点不空
        {
            if(Data.nodeName() != ELEMENT_DATA)
            {
                if(Data.nodeName() == ELEMENT_HEADER)  // 定义表格的列和行表头
                {
                    int H_sectionPos = 0;
                    int V_sectionPos = 0;

                    for (QDomElement Header = Data.firstChildElement(); !Header.isNull(); Header = Header.nextSiblingElement())
                    {
                        if(Header.nodeName() == ELEMENT_HORIZONTAL)
                        {
                            widgetModel->setColumnCount(H_sectionPos+1);
                            widgetModel->setHeaderData(H_sectionPos,Qt::Horizontal,Header.text());
                            H_sectionPos ++;
                        }
                        else  if(Header.nodeName() == ELEMENT_VERTICAL)
                        {
                            widgetModel->setRowCount(V_sectionPos+1);
                            widgetModel->setHeaderData(V_sectionPos,Qt::Vertical,Header.text());
                            V_sectionPos ++;
                        }
                    }
                }
                continue;
            }
            //首先检测xml中VarNameRole变量名称是否为空和是否重复
            int ItemRow = Data.attribute(ATTR_ITEMROW).toInt();
            int ItemCol = Data.attribute(ATTR_ITEMCOL).toInt();
            QString XmlVarName = Data.attribute(ItemUserRoleEnym.valueToKey(ItemDelegate::VarNameRole));
            if(XmlVarName.isEmpty())  // xml中变量名为空
            {
                qDebug() << "fetchOfStore:" << ItemRow << "," << ItemCol << ":"
                         << XmlVarName << ".isEmpty";
                 continue;
            }
            if(widgetModel->item(ItemRow,ItemCol) != NULL)  // 有重复的数据在模型中进行了映射
            {
                if(widgetModel->item(ItemRow,ItemCol)->data(ItemDelegate::VarNameRole).isValid())
                {
                    qDebug() << "fetchOfStore:" << ItemRow << "," << ItemCol
                             << ":" <<widgetModel->item(ItemRow,ItemCol)->data(ItemDelegate::VarNameRole).toString()
                             << "/" << XmlVarName << " repetition item";
                    continue;
                }
            }

             // 根据xml文件的最大行和列进行 行列的扩充
            if(widgetModel->rowCount() <= ItemRow)
            {
                widgetModel->setRowCount(ItemRow+1);
            }
            if(widgetModel->columnCount() <= ItemCol)
            {
                widgetModel->setColumnCount(ItemCol+1);
            }


            item = new QStandardItem(); // 创建模型项储存体对象
            widgetModel->setItem(ItemRow,ItemCol,item); // 将项添加到模型中对应的行列中
            modelStruct->itemMap.insert(XmlVarName,item);
            item->setData(XmlVarName,ItemDelegate::VarNameRole);  // 设置项角色（属性）

            QDomElement Interface = Data.firstChildElement(ELEMENT_INTERFACE);     //xml操作获得第一个子节点元素
            if(Interface.isNull())continue;

            strTmp.clear();strTmp = Interface.attribute(ItemUserRoleEnym.valueToKey(ItemDelegate::NameRole));
            if(!strTmp.isEmpty())
                item->setData(strTmp,ItemDelegate::NameRole);

            strTmp.clear();strTmp = Interface.attribute(ItemUserRoleEnym.valueToKey(ItemDelegate::DescRole));
            if(!strTmp.isEmpty())
            {
                item->setData(strTmp,ItemDelegate::DescRole);
                item->setData(strTmp,Qt::ToolTipRole);  // 用于泡泡窗口提示，但视图那边好像不起作用？
            }

            strTmp.clear();strTmp = Interface.attribute(ItemUserRoleEnym.valueToKey(ItemDelegate::ShowTypeRole));
            if(!strTmp.isEmpty()) item->setData(ShowTypeEnym.keyToValue(strTmp.toLatin1().data()),ItemDelegate::ShowTypeRole);

            strTmp.clear();strTmp = Interface.attribute(ItemUserRoleEnym.valueToKey(ItemDelegate::CheckTypeRole));
            if(!strTmp.isEmpty())item->setData(CheckTypeEnym.keyToValue(strTmp.toLatin1().data()),ItemDelegate::CheckTypeRole);

            strTmp.clear();strTmp = Interface.attribute(ItemUserRoleEnym.valueToKey(ItemDelegate::EditTypeRole));
            if(!strTmp.isEmpty())item->setData(EditTypeEnym.keyToValue(strTmp.toLatin1().data()),ItemDelegate::EditTypeRole);

            strTmp.clear();strTmp = Interface.attribute(ItemUserRoleEnym.valueToKey(ItemDelegate::RegexpRole));
            if(!strTmp.isEmpty())item->setData(strTmp,ItemDelegate::RegexpRole);

            strTmp.clear();strTmp = Interface.attribute(ItemUserRoleEnym.valueToKey(ItemDelegate::ContentRole));
            if(!strTmp.isEmpty())item->setData(strTmp,ItemDelegate::ContentRole);

            if(Interface.attribute(ATTR_EDITABLE) == "false") // 是否可编辑，对于表格可能有些表格是不给编辑的
            {
                Qt::ItemFlags flags =  item->flags();
                flags &= ~Qt::ItemIsEditable;
                item->setFlags(flags);
            }

            QDomElement Val =  Interface.nextSiblingElement(ELEMENT_VAL);
            if(Val.isNull())continue;

            strTmp.clear();strTmp = Val.attribute(ItemUserRoleEnym.valueToKey(ItemDelegate::UnitRole));
            if(!strTmp.isEmpty())item->setData(strTmp,ItemDelegate::UnitRole);

            strTmp.clear();strTmp = Val.attribute(ItemUserRoleEnym.valueToKey(ItemDelegate::DataTypeRole));
            if(!strTmp.isEmpty()) item->setData(strTmp,ItemDelegate::DataTypeRole);

            strTmp.clear();strTmp = Val.attribute(ItemUserRoleEnym.valueToKey(ItemDelegate::RangMinRole));
            if(!strTmp.isEmpty())  item->setData(strTmp,ItemDelegate::RangMinRole);

            strTmp.clear();strTmp = Val.attribute(ItemUserRoleEnym.valueToKey(ItemDelegate::RangMaxRole));
            if(!strTmp.isEmpty())  item->setData(strTmp,ItemDelegate::RangMaxRole);

            strTmp.clear();strTmp = Val.attribute(ItemUserRoleEnym.valueToKey(ItemDelegate::StepRole));
            if(!strTmp.isEmpty())     item->setData(strTmp,ItemDelegate::StepRole);

            strTmp.clear();strTmp = Val.attribute(ItemUserRoleEnym.valueToKey(ItemDelegate::DecimalRole));
            if(!strTmp.isEmpty())  item->setData(strTmp,ItemDelegate::DecimalRole);

            strTmp.clear();strTmp = Val.attribute(ItemUserRoleEnym.valueToKey(ItemDelegate::DefaultValRole));
            if(!strTmp.isEmpty())  item->setData(strTmp,ItemDelegate::DefaultValRole);

            if(!Val.text().isEmpty())
            {
                if(item->data(ItemDelegate::CheckTypeRole).toInt() ==  ItemDelegate::Float)
                {
                    item->setData(QString().setNum(Val.text().toFloat(),'f',item->data(ItemDelegate::DecimalRole).toInt()),Qt::EditRole);
                }
                else
                {
                    item->setData(Val.text(),Qt::EditRole);
                }
                item->setData(item->data(Qt::EditRole),ItemDelegate::TempRole);  // 储存一份，用于撤销修改时使用

            }

            QDomElement xmlCombo =  Val.nextSiblingElement();
            QMap<QString,QVariant> mapCombo;
            while(!xmlCombo.isNull())
            {
                if(xmlCombo.nodeName() != "Combo") continue;
                mapCombo.insert(xmlCombo.attribute("InterfaceName"),xmlCombo.text());
                xmlCombo = xmlCombo.nextSiblingElement();
            }
            if(!mapCombo.isEmpty())  item->setData(mapCombo,ItemDelegate::ComboMapRole);
        }
        connect(widgetModel,SIGNAL(itemChanged(QStandardItem *)),this,SLOT(itemChanged(QStandardItem *)));
    }
    return true;
}
