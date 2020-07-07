#include "xmldatastore.h"
#include <QDir>
xmlDataStore::xmlDataStore(QObject *parent):DataStore(parent)
{

}

bool xmlDataStore::createStore()//
{
    return true;
}
bool xmlDataStore::saveToStore()
{
    QMetaEnum ModelViewEnym      = QMetaEnum::fromType<ModelView>();

    QDomDocument doc;
    QDomProcessingInstruction xmlInstruction = doc.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"");
    doc.appendChild(xmlInstruction);  // 开始文档（XML 声明）

    // 根元素 <Blogs>
    QDomElement DataBase = doc.createElement(ELEMENT_DATABASE);
    DataBase.setAttribute(ATTR_VERSION, ATTR_VERSION_1_0);  // 储存载体版本
    doc.appendChild(DataBase);

    int ItemRow = 0;
    int ItemCol = 0;

    // 遍历models进行保存
    for (int pos = 0; pos < m_Models.size(); ++pos)
    {
        /*获取模型数据，生成表格元素*/
        QDomElement Model = doc.createElement(ELEMENT_MODEL);   // 储存单元描述体
        Model.setAttribute("Name",  m_Models[pos]->Name);
        Model.setAttribute("View",  ModelViewEnym.valueToKey(m_Models[pos]->View));
        Model.setAttribute("Type",  m_Models[pos]->Type);
        Model.setAttribute("ContentRole",  m_Models[pos]->Content);

        /*表格标题xml定义*/
        DataBase.appendChild(Model);
        QDomElement Header;
        QDomElement V_Header,H_Header;
        if(ItemRow<m_Models[pos]->View == Table)
        {
            Header = doc.createElement(ELEMENT_HEADER); // 创建表格标题储存
            H_Header = doc.createElement(ELEMENT_H_HEADER); //
            V_Header = doc.createElement(ELEMENT_V_HEADER); //
            Model.appendChild(Header);
            Header.appendChild(H_Header);
            Header.appendChild(V_Header);
        }


        /*遍历模型，进行写xml操作*/
        QDomNode  dataXmlNode; // 添加成功后
        // 遍历行
        for(ItemRow = 0;ItemRow<m_Models[pos]->model->rowCount();ItemRow++)
        {
            /*记录行标题与索引*/
            if(!V_Header.isNull()){
                QDomElement HeaderData = doc.createElement(ELEMENT_DATA); // 创建表格标题储存
                HeaderData.setAttribute(ATTR_NAME,m_Models[pos]->model->headerData(ItemRow,Qt::Vertical,Qt::DisplayRole).toString());
                HeaderData.setAttribute(ATTR_VAR,m_Models[pos]->model->headerData(ItemRow,Qt::Vertical,Qt::UserRole).toString());
                V_Header.appendChild(HeaderData);
            }

            // 遍历列
            for(ItemCol = 0;ItemCol<m_Models[pos]->model->columnCount();ItemCol++)
            {
                /*记录列标题与索引*/
                if(ItemRow == 0 && !H_Header.isNull()){
                    QDomElement HeaderData = doc.createElement(ELEMENT_DATA); // 创建表格标题储存
                    HeaderData.setAttribute(ATTR_NAME,m_Models[pos]->model->headerData(ItemCol,Qt::Horizontal,Qt::DisplayRole).toString());
                    HeaderData.setAttribute(ATTR_VAR,m_Models[pos]->model->headerData(ItemCol,Qt::Horizontal,Qt::UserRole).toString());
                    H_Header.appendChild(HeaderData);
                }

                /*记录数据*/
                QModelIndex ModelIndex = m_Models[pos]->model->index(ItemRow,ItemCol);
                if(ModelIndex.isValid() && !ModelIndex.data(ItemDelegate::VarNameRole).toString().isEmpty())
                {
                    dataXmlNode = addXmlData(doc,Model,&ModelIndex);
                }
            }
            dataXmlNode.toElement().setAttribute(COL_END,COL_END);// 添加换行标识
        }
    }

    // 保存 XML 文件
    QString strFile(xmlFileName);
    QFile file(strFile);
    // 只写模式打开文件
    if (!file.open(QFile::WriteOnly | QFile::Text))
    {
        QDir  dir = QDir();
        dir.mkpath(xmlFileName.left(xmlFileName.lastIndexOf('/')));
        file.open(QFile::WriteOnly | QFile::Text);
    }
    QTextStream out(&file);
    doc.save(out, QDomNode::EncodingFromDocument);
    file.close();
    DataStore::saveToStore();
    return true;
}

QDomNode xmlDataStore::addXmlData(QDomDocument &doc,QDomElement &root,QModelIndex *item)
{
    QMetaEnum EditTypeEnym      = QMetaEnum::fromType<ItemDelegate::EditType>();    // 枚举值与字符转换
    QMetaEnum ItemUserRoleEnym  = QMetaEnum::fromType<ItemDelegate::ItemUserRole>();
    QMetaEnum CheckTypeEnym     = QMetaEnum::fromType<ItemDelegate::CheckType>();
    QMetaEnum ShowTypeEnym      = QMetaEnum::fromType<ItemDelegate::ShowType>();
    QDomNode  dataXmlNode;

    QDomElement Data = doc.createElement(ELEMENT_DATA);   // 储存单元描述体
    dataXmlNode = root.appendChild(Data);
    Data.setAttribute(ItemUserRoleEnym.valueToKey(ItemDelegate::VarNameRole),  item->data(ItemDelegate::VarNameRole).toString());              // 储存单元名称(英文)
    //Data.setAttribute("ModelType", ModelType);      // 使用的模型类型
    //Data.setAttribute("ModelName", ModelName);      // 使用的模型名称
    //Data.setAttribute(ATTR_ITEMROW,   QString("%1").arg(item->row()));     // 处在模型的行位置 0开始
    //Data.setAttribute(ATTR_ITEMCOL,   QString("%1").arg(item->column()));  // 处在模型的列位置 0开始

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
    if(item->data(ItemDelegate::factorARole).isValid())
    {
        Val.setAttribute(ItemUserRoleEnym.valueToKey(ItemDelegate::factorARole),item->data(ItemDelegate::factorARole).toString());  // A系数
    }
    if(item->data(ItemDelegate::factorBRole).isValid())
    {
        Val.setAttribute(ItemUserRoleEnym.valueToKey(ItemDelegate::factorBRole),item->data(ItemDelegate::factorBRole).toString());  // B系数
    }

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

    return dataXmlNode;
}

bool xmlDataStore::fetchOfStore()
{
    QMetaEnum ItemUserRoleEnym  = QMetaEnum::fromType<ItemDelegate::ItemUserRole>();
    QMetaEnum ModelViewEnym      = QMetaEnum::fromType<ModelView>();
    QDomDocument doc;
    QFile file(xmlFileName);

    /*判断储存文件是否可用*/
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
        /*创建模型描述结构体*/
        if(Model.nodeName() != ELEMENT_MODEL) continue;
        ModelStruct *modelStruct = new ModelStruct;
        modelStruct->Name = Model.attribute("Name");
        modelStruct->View = ModelViewEnym.keysToValue(Model.attribute("View").toLatin1().data());
        modelStruct->Type = Model.attribute("Type");
        modelStruct->Content = Model.attribute("ContentRole");
        widgetModel = new QStandardItemModel(this);
        modelStruct->model = widgetModel;
        addModel(modelStruct);


        /*如果有对应的表标题，则读取*/
        QDomElement Header  = Model.firstChildElement(ELEMENT_HEADER);
        if(!Header.isNull())
        {
            for(QDomElement m_Header = Header.firstChildElement(); !m_Header.isNull(); m_Header = m_Header.nextSiblingElement())
            {
                for(QDomElement Data = m_Header.firstChildElement(ELEMENT_DATA); !Data.isNull(); Data = Data.nextSiblingElement(ELEMENT_DATA))  //如果节点不空
                {
                    item = new QStandardItem();
                    item->setData(Data.attribute(ATTR_NAME),Qt::DisplayRole);
                    item->setData(Data.attribute(ATTR_VAR),Qt::UserRole);
                    if(m_Header.nodeName() == ELEMENT_H_HEADER)
                    {
                        widgetModel->setColumnCount(widgetModel->columnCount() + 1);
                        widgetModel->setHorizontalHeaderItem(widgetModel->columnCount()-1,item);
                    }
                    else if(m_Header.nodeName() == ELEMENT_V_HEADER)
                    {
                        widgetModel->setRowCount(widgetModel->rowCount() + 1);
                        widgetModel->setVerticalHeaderItem(widgetModel->rowCount()-1,item);
                    }
                }
            }
        }

        /*遍历读取*/
        int ItemRow = 0;
        int ItemCol = 0;
        for(QDomElement Data = Model.firstChildElement(ELEMENT_DATA); !Data.isNull(); Data = Data.nextSiblingElement(ELEMENT_DATA))  //如果节点不空
        {
            /*首先检测xml中VarNameRole变量名称是否为空和是否重复*/
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

            /*创建模型项，并通过xml初始化*/
            item = new QStandardItem(); // 创建模型项储存体对象
            widgetModel->setItem(ItemRow,ItemCol,item);  // 将项添加到模型中对应的行列中
            modelStruct->itemMap.insert(XmlVarName,item);// 创建映射表，用于快速定位对应的模型项
            item->setData(XmlVarName,ItemDelegate::VarNameRole);  // 设置项角色（属性）

            // 获取xml数据进行初始化
            getXmlData(Data,item);
            // 输入检测失败，采用默认值
            if(!initItemData(item->model(),item->index(),item->data(Qt::EditRole),Qt::EditRole))
            {
                initItemData(item->model(),item->index(),item->data(ItemDelegate::DefaultValRole),Qt::EditRole);
            }

            if(Data.attribute(COL_END) == COL_END) // 判断是否是最后列的元素
            {
                ItemRow++;     // 换行
                ItemCol = 0;   // 列拨到最首列
            }
            else
            {
                ItemCol++;    // 继续在当前行添加列
            }
        }
    }
    DataStore::fetchOfStore();
    return true;
}

void xmlDataStore::getXmlData(QDomElement &Data,QStandardItem *item)
{
    QMetaEnum EditTypeEnym      = QMetaEnum::fromType<ItemDelegate::EditType>();    // 枚举值与字符转换
    QMetaEnum ItemUserRoleEnym  = QMetaEnum::fromType<ItemDelegate::ItemUserRole>();
    QMetaEnum CheckTypeEnym     = QMetaEnum::fromType<ItemDelegate::CheckType>();
    QMetaEnum ShowTypeEnym      = QMetaEnum::fromType<ItemDelegate::ShowType>();
    QString strTmp;

    QDomElement Interface = Data.firstChildElement(ELEMENT_INTERFACE);     //xml操作获得第一个子节点元素

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
    if(!strTmp.isEmpty())
    {
        item->setData(EditTypeEnym.keyToValue(strTmp.toLatin1().data()),ItemDelegate::EditTypeRole);
        if(EditTypeEnym.keyToValue(strTmp.toLatin1().data()) == ItemDelegate::CheckBox)
        {
            Qt::ItemFlags flags = item->flags();
            flags &=  ~Qt::ItemIsEditable;       // 目前这些标记还只对表格有效，小控件还待完善
            flags |=  Qt::ItemIsUserCheckable;
            item->setFlags( flags);
            item->setData(Qt::Checked,Qt::CheckStateRole);
        }
    }
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

    strTmp.clear();strTmp = Val.attribute(ItemUserRoleEnym.valueToKey(ItemDelegate::UnitRole));
    if(!strTmp.isEmpty()) item->setData(strTmp,ItemDelegate::UnitRole);

    strTmp.clear();strTmp = Val.attribute(ItemUserRoleEnym.valueToKey(ItemDelegate::DataTypeRole));
    if(!strTmp.isEmpty()) item->setData(strTmp,ItemDelegate::DataTypeRole);

    strTmp.clear();strTmp = Val.attribute(ItemUserRoleEnym.valueToKey(ItemDelegate::RangMinRole));
    if(!strTmp.isEmpty()) item->setData(strTmp,ItemDelegate::RangMinRole);

    strTmp.clear();strTmp = Val.attribute(ItemUserRoleEnym.valueToKey(ItemDelegate::RangMaxRole));
    if(!strTmp.isEmpty()) item->setData(strTmp,ItemDelegate::RangMaxRole);

    strTmp.clear();strTmp = Val.attribute(ItemUserRoleEnym.valueToKey(ItemDelegate::StepRole));
    if(!strTmp.isEmpty()) item->setData(strTmp,ItemDelegate::StepRole);

    strTmp.clear();strTmp = Val.attribute(ItemUserRoleEnym.valueToKey(ItemDelegate::DecimalRole));
    if(!strTmp.isEmpty()) item->setData(strTmp,ItemDelegate::DecimalRole);

    strTmp.clear();strTmp = Val.attribute(ItemUserRoleEnym.valueToKey(ItemDelegate::DefaultValRole));
    if(!strTmp.isEmpty())
    {
        if(item->data(ItemDelegate::CheckTypeRole).toInt() ==  ItemDelegate::Float)
        {
            item->setData(QString().setNum(strTmp.toFloat(),'f',item->data(ItemDelegate::DecimalRole).toInt()),ItemDelegate::DefaultValRole);
        }
        else
        {
            item->setData(strTmp,ItemDelegate::DefaultValRole);
        }
    }

    strTmp.clear();strTmp = Val.attribute(ItemUserRoleEnym.valueToKey(ItemDelegate::factorARole));
    if(!strTmp.isEmpty()) item->setData(strTmp,ItemDelegate::factorARole);

    strTmp.clear();strTmp = Val.attribute(ItemUserRoleEnym.valueToKey(ItemDelegate::factorBRole));
    if(!strTmp.isEmpty()) item->setData(strTmp,ItemDelegate::factorBRole);


    if(item->data(ItemDelegate::CheckTypeRole).toInt() ==  ItemDelegate::Float)
    {
        item->setData(QString().setNum(Val.text().toFloat(),'f',item->data(ItemDelegate::DecimalRole).toInt()),Qt::EditRole);
    }
    else
    {
        item->setData(Val.text(),Qt::EditRole);
    }

    QDomElement xmlCombo =  Val.nextSiblingElement(ELEMENT_COMBO);
    QMap<QString,QVariant> mapCombo;
    while(!xmlCombo.isNull())
    {
        if(xmlCombo.nodeName() != "Combo") continue;
        mapCombo.insert(xmlCombo.attribute("InterfaceName"),xmlCombo.text());
        xmlCombo = xmlCombo.nextSiblingElement();
    }
    if(!mapCombo.isEmpty())  item->setData(mapCombo,ItemDelegate::ComboMapRole);  
}


