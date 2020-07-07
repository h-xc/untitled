#include "datastore.h"

#include <QDebug>
#include <QMetaProperty>
#include <QHostAddress>

DataStore::DataStore(QObject *parent) : QObject(parent)
{
    itemChangedList.clear();
    m_isChangedFlag = false;
}
void DataStore::itemChanged(QStandardItem *item)
{
    itemChangedList.insert(item); // 先记录修改的模型项，在确认修改的时候，才进行保存和其它操作
}

bool  DataStore::submit()       //  确认修改，提供定值修改确认
{
    if(!itemChangedList.isEmpty())  // 有被修改的模型项
    {
        // 发送预备修改信号

        // 等待可执行修改信号

        foreach (QStandardItem *value, itemChangedList)
        {
            // 对一些值进行更新，如缓存值，
            initItemData(value->model(),value->index(),value->data(Qt::EditRole),Qt::EditRole);
        }
        saveToStore();
        itemChangedList.clear();  // 清除记录的改变的项
        emit FixValChanged();     // 发送定值改变信号
    }
    return true;
}

bool  DataStore::revert()       //  丢弃修改，恢复原定值 接口函数
{
    foreach (QStandardItem *value, itemChangedList)  // 只恢复修改过的定值
    {
        // 将保存的临时值，恢复到编辑角色属性中
        value->setData(value->data(ItemDelegate::TempEditRole),Qt::EditRole);
    }
    itemChangedList.clear();
    return true;
}

bool  DataStore::restore() // 恢复默认，提供定值修改按钮方法 接口函数
{
    QString oldVal,newVal;
    // 遍历所有模型
    for(int pos=0;pos <m_Models.count();pos++)
    {
        // 遍历模型的项
        for(int row=0;row <m_Models[pos]->model->rowCount();row ++)
            for(int col = 0;col <m_Models[pos]->model->columnCount();col++)
            {
                QModelIndex index = m_Models[pos]->model->index(row,col);

                oldVal = m_Models[pos]->model->data(index,Qt::EditRole).toString();  // 当前值，待修改值
                newVal = m_Models[pos]->model->data(index,ItemDelegate::DefaultValRole).toString();  // 默认值，新值
                /* 如果内容不相同，则进行恢复默认*/
                if(!m_Models[pos]->model->data(index,ItemDelegate::VarNameRole).toString().isEmpty()
                        && oldVal != newVal)
                {
                    m_Models[pos]->model->setData(index,newVal,Qt::EditRole);
                }
            }
    }
    return true;
}

QStandardItem * DataStore::initItem(ItemDataStruct &itemDataStruct)
{
    QStandardItem *item =new QStandardItem();
    if(!itemDataStruct.VarName.isEmpty())
    {
        item->setData(itemDataStruct.VarName,ItemDelegate::VarNameRole);
    }
    else
    {
        qDebug() << "DataStore::initItem() VarName is NULL";
        delete item;
        return NULL;
    }
    if(!itemDataStruct.Name.isEmpty())
        item->setData(itemDataStruct.Name,ItemDelegate::NameRole);

    if(!itemDataStruct.Unit.isEmpty())
        item->setData(itemDataStruct.Unit,ItemDelegate::UnitRole);

    if(!itemDataStruct.Desc.isEmpty())
        item->setData(itemDataStruct.Desc,ItemDelegate::DescRole);

    item->setData(itemDataStruct.showType,ItemDelegate::ShowTypeRole);

    item->setData(itemDataStruct.checkType,ItemDelegate::CheckTypeRole);

    //item->setData(itemDataStruct.DataType,ItemDelegate::DataTypeRole);

    item->setData(itemDataStruct.editType,ItemDelegate::EditTypeRole);

    if(itemDataStruct.DefaultVal.isValid())
        item->setData(itemDataStruct.RangMin,ItemDelegate::DefaultValRole);

    if(itemDataStruct.RangMin.isValid())
        item->setData(itemDataStruct.RangMin,ItemDelegate::RangMinRole);

    if(itemDataStruct.RangMax.isValid())
        item->setData(itemDataStruct.RangMax,ItemDelegate::RangMaxRole);

    if(itemDataStruct.Step.isValid())
        item->setData(itemDataStruct.Step,ItemDelegate::StepRole);

    if(!itemDataStruct.Regexp.isEmpty())
        item->setData(itemDataStruct.Regexp,ItemDelegate::RegexpRole);

    if(!itemDataStruct.Content.isEmpty())
        item->setData(itemDataStruct.Content,ItemDelegate::ContentRole);

    if(itemDataStruct.Decimal>0)
        item->setData(itemDataStruct.Decimal,ItemDelegate::DecimalRole);

    if(!itemDataStruct.ComboMap.isEmpty())
        item->setData(itemDataStruct.ComboMap,ItemDelegate::ComboMapRole);

    return item;
}


void DataStore::CleanModels()
{
    while(!m_Models.isEmpty())
    {
        ModelStruct* modelStruct = m_Models.takeFirst();
        delete modelStruct->model;
        delete modelStruct;
    }
}

bool DataStore::createStore()
{
    return true;
}

bool DataStore::saveToStore()
{
    return true;
}

bool DataStore::fetchOfStore()
{
    createRelevanceModel();  //检测是否有新的结构体被关联，只检测新加，不检测删除
    for(int pos= 0;pos<m_Models.count();pos++)  // 接收模型数据更改的信号
    {
        connect(m_Models[pos]->model,SIGNAL(itemChanged(QStandardItem *)),this,SLOT(itemChanged(QStandardItem *)));
    }
    return true;
}

void DataStore::initItemModel()
{

}

ModelStruct* DataStore::getModel(QString modelName)
{
    for(int pos= 0;pos<m_Models.count();pos++)
    {
        if(m_Models[pos]->Name == modelName)
        {
            return m_Models[pos];
        }
    }
    return NULL;
}

ModelStruct* DataStore::getModel(const QAbstractItemModel* model)
{
    for(int pos= 0;pos<m_Models.count();pos++)
    {
        if(m_Models[pos]->model == model)
        {
            return m_Models[pos];
        }
    }
    return NULL;
}

QVariant DataStore::getValData(QString modelName,QString itemName,int role /*= Qt::EditRole*/)
{
    ModelStruct* modelStruct = getModel(modelName);
    if(modelStruct != NULL)
    {
        return modelStruct->itemMap.value(itemName)->data(role);
    }
    return QVariant();
}

QVariant DataStore::getValData(QString modelName,int itemRow,int itemCol,int role /*= Qt::EditRole*/)
{
    ModelStruct* modelStruct = getModel(modelName);
    if(modelStruct != NULL)
    {
        QModelIndex index =  modelStruct->model->index(itemRow,itemCol);
        return modelStruct->model->data(index,role);
    }
    return QVariant();
}

bool DataStore::initItemData(QAbstractItemModel *model,QModelIndex index,QVariant val,int role)
{
    QVariant dispVal;
    QVariant actuVal;
    // 先获取实际值和显示值
    if(role == ItemDelegate::ActlValRole)// 设置实际值
    {
        actuVal = val;
        if(actlDispConvert(index,dispVal,actuVal,0) == 2)
        {
            dispVal = actuVal;
            actuVal.clear();
        }
    }
    else if(role == Qt::EditRole)   // 设置显示值
    {
        dispVal = val;
        if(actlDispConvert(index,dispVal,actuVal,1) == 2)// 转换为实际值保存，不判断是否转换成功
        {
            actuVal.clear();
        }
    }
    // 先对显示值进行输入检测（以显示值为基础，制定了值的输入检测）
    QValidator *validator = ItemDelegate::createValidator(index);
    if(validator != NULL)
    {
        int pos = 0;
        QString disStr = dispVal.toString();
        if(validator->validate(disStr,pos) == QValidator::Acceptable) // 输入有效
        {
            // 判断值是否相同
            if(dispVal != index.data(ItemDelegate::TempEditRole))
            {
                model->setData(index,dispVal,Qt::EditRole);
                model->setData(index,dispVal,ItemDelegate::TempEditRole);
                if(actuVal.isValid())
                {
                    model->setData(index,actuVal,ItemDelegate::ActlValRole);
                }
                setRelevance(index); // 将保存的数据关联到结构体中
            }
        }
        else  // xml储存的值不符合输入检测
        {
            qDebug() << "DataStore::initItemData:" <<index.data(ItemDelegate::VarNameRole).toString() << " dispVal Invalid"; // 输入无效
            return false;
        }
    }
    return true;
}

int DataStore::actlDispConvert(QModelIndex &index, QVariant &dispVal,QVariant &actuVal,int flag)
{
    if(index.data(ItemDelegate::CheckTypeRole).toInt() == ItemDelegate::Float || index.data(ItemDelegate::CheckTypeRole).toInt() == ItemDelegate::Int)
    {
        //实际值 = factorARole * dispVal + factorBRole
        double dispVal_f;
        double actuVal_f;

        /*获取系数*/
        double factorA = 1;  // 默认状态下
        double factorB = 0;
        if(index.data(ItemDelegate::factorARole).canConvert(QVariant::Double))
        {
            factorA = index.data(ItemDelegate::factorARole).toDouble();
        }
        if(index.data(ItemDelegate::factorBRole).canConvert(QVariant::Double))
        {
            factorB = index.data(ItemDelegate::factorBRole).toDouble();
        }

        if(flag == 0)// 实际值到显示值
        {
            if(actuVal.canConvert(QVariant::Double))
            {
                actuVal_f = actuVal.toDouble();
            }
            else if(actuVal.canConvert(QVariant::Int))
            {
                actuVal_f = actuVal.toInt();
            }
            else
            {
                return 0;
            }

            dispVal_f = (actuVal_f - factorB)/factorA;
            dispVal = dispVal_f;
        }
        else if(flag == 1)// 显示值到实际值
        {
            if(dispVal.canConvert(QVariant::Double))
            {
                dispVal_f = dispVal.toDouble();
            }
            else if(dispVal.canConvert(QVariant::Int))
            {
                dispVal_f = dispVal.toInt();
            }
            else
            {
                return 0;
            }
            actuVal_f = dispVal_f*factorA + factorB;
            actuVal = actuVal_f;
        }
    }
    else if(index.data(ItemDelegate::CheckTypeRole).toInt() == ItemDelegate::Ip4)
    {
        QHostAddress ipAddr;
        if(flag == 0)// 实际值到显示值
        {
            ipAddr.setAddress(actuVal.toUInt());
            dispVal = ipAddr.toString();
        }
        else if(flag == 1)//显示值到实际值
        {
            ipAddr.setAddress(dispVal.toString());
            actuVal = ipAddr.toIPv4Address();
        }
    }
    else if(index.data(ItemDelegate::CheckTypeRole).toInt() == ItemDelegate::MAC)
    {
        if(flag == 0)// 实际值到显示值
        {
            QByteArray byteArr = actuVal.toByteArray();
            dispVal = QString().arg(QString::number(byteArr.at(0), 16).toUpper(),2,'0')
            .arg(QString::number(byteArr.at(1), 16).toUpper(),2,'0')
            .arg(QString::number(byteArr.at(2), 16).toUpper(),2,'0')
            .arg(QString::number(byteArr.at(3), 16).toUpper(),2,'0')
            .arg(QString::number(byteArr.at(4), 16).toUpper(),2,'0')
            .arg(QString::number(byteArr.at(5), 16).toUpper(),2,'0')
            .arg(QString::number(byteArr.at(6), 16).toUpper(),2,'0');
        }
        else  if(flag == 1)//显示值到实际值
        {
            QStringList str_s = dispVal.toString().split(":");
            QByteArray byteArr;

            if(str_s.count() == 6)
            {
                for(int i = 0;i<6 ;i++)
                {
                    bool ok;
                    byteArr[i] = str_s.at(i).toUInt(&ok,16);
                }
            }
        }
    }
    else
    {
       return 2;
    }
    return 1;
}



void DataStore::createRelevanceModel()
{
    QSet<QString> relevances; // 防止多个属性类有相同的属性名被记录
    QList<ModelStruct*> relevancesModels;

    bool isRelevance = false; // 属性类是否有被关联
    QStandardItemModel* relevanceModel = NULL;  // 属性类对应的模型

    /*遍历所有的属性类*/
    for(int pos =0;pos < relevance.count();pos++)
    {
        const QMetaObject *metaobject = relevance[pos]->metaObject();// 关联类的元信息
        int count = metaobject->propertyCount(); // 属性个数

        relevanceModel = NULL; //
        int ctrlRow = 0,ctrlCol = 0;
        /*遍历属性类中的属性*/
        for (int i = metaobject->propertyOffset(); i<count; ++i)// 只对当前关联类的属性进行创建，不对基类的属性创建
        {
            isRelevance = false;
            QMetaProperty metaproperty = metaobject->property(i); // 获取属性
            QString name = QString(metaproperty.name());   // 获取属性名
            QVariant property = metaproperty.read(relevance[pos]);

            if(property.canConvert<ArrayVal>())continue;
            if(property.canConvert<ArrayList>()) // 如果是数组，则额外创建一个模型
            {
                /* 此处只查看了模型是否存在，如果存在则不做任何改动
                 * 以后可以做到检测数组结构由改变，则更新对应的模型结构*/
                ModelStruct* modelStruct =  getModel(name); // 是否存在数组对应的模型
                if(modelStruct == NULL)  // 没有则创建新模型储存
                {
                    relevances.insert(name);
                    modelStruct =  new ModelStruct;
                    modelStruct->Name    = name;
                    modelStruct->View    = Table;
                    modelStruct->Type    = QStandardItemModel::staticMetaObject.className();
                    modelStruct->Content = name;
                    QStandardItemModel *arrayModel = new QStandardItemModel(this);
                    modelStruct->model   = arrayModel;
                    relevancesModels.append(modelStruct);

                    int col = 0;
                    QStandardItem* item;
                    ArrayList array = property.value<ArrayList>();

                    for (int pos = 0;pos < array.count();pos++,col++)
                    {
                        QString colName = array[pos].colName;
                        QList<QVariant> rowItems = array[pos].Vals;
                        for(int row = 0;row<rowItems.count();row++)
                        {
                            item = new QStandardItem();
                            initRelevanceItem(QString("%1_%2").arg(colName).arg(row),rowItems[row],item);
                            arrayModel->setItem(row,col,item);
                        }
                        item = new QStandardItem();
                        item->setData(colName,Qt::UserRole);
                        item->setData(colName,Qt::DisplayRole);
                        arrayModel->setHorizontalHeaderItem(col,item);
                    }
                }
            }
            else // 非数组类型，则都创建为小控件
            {
                /*在模型中查找是否有对应的项*/
                for(int pos= 0;pos<m_Models.count();pos++)
                {
                    if(m_Models[pos]->itemMap.value(name,NULL) != NULL)
                    {
                        isRelevance = true;  // 模型中存在记录了该属性的项
                        break;
                    }
                }

                /*若没有对应的模型记录且没有记录过的 则创建*/
                if(!isRelevance && (relevances.find(name) == relevances.constEnd() || relevances.isEmpty()))
                {
                    relevances.insert(name);   // 标记当前的属性，防止重复记录（因为可能在不同关联类中关联了同一个模型项）
                    if(relevanceModel == NULL) // 当前关联类没有对应的模型
                    {
                        ModelStruct* modelStruct =  getModel(metaobject->className()); //
                        if(modelStruct != NULL)
                        {
                            relevanceModel = dynamic_cast<QStandardItemModel*>(modelStruct->model);
                        }
                        else
                        {
                            relevanceModel = new QStandardItemModel(this);
                            modelStruct =  new ModelStruct;
                            modelStruct->View    = Control;
                            modelStruct->Name    = metaobject->className();
                            modelStruct->Type    = QStandardItemModel::staticMetaObject.className();
                            modelStruct->Content = metaobject->className();
                            modelStruct->model   = relevanceModel;
                            relevancesModels.append(modelStruct);
                        }
                        ctrlRow = relevanceModel->rowCount();
                    }

                    QStandardItem* item = new QStandardItem();
                    initRelevanceItem(name,property,item);
                    relevanceModel->setItem(ctrlRow,ctrlCol,item);

                    /*默认分两列*/
                    ctrlCol++;
                    if(ctrlCol > 1)
                    {
                        ctrlCol = 0;
                        ctrlRow = relevanceModel->rowCount();
                    }
                }
            }
        }

        if(!relevances.isEmpty())  // 有新的数据被记录，则保存一下
        {
            addModels(relevancesModels);
            saveToStore();
        }
    }
}
void DataStore::initRelevanceItem(QString name,QVariant &property,QStandardItem *item)
{
    QMap<QString, QVariant> combo; // bool默认编辑器为下拉选择框
    combo.insert("禁止",false);
    combo.insert("使能",true);

    item->setData(ItemDelegate::LineEdit,ItemDelegate::EditTypeRole);
    if(property.type() == QVariant::Int) // 创建的时候，设定检测类型为Int
    {
        item->setData(ItemDelegate::Int,ItemDelegate::CheckTypeRole);
    }
    else if(property.type() == QVariant::Bool)
    {
        item->setData(ItemDelegate::ComboBox,ItemDelegate::EditTypeRole);
        item->setData(combo,ItemDelegate::ComboMapRole);
    }
    else if(property.type() == QVariant::Double)
    {
        item->setData(ItemDelegate::Float,ItemDelegate::EditTypeRole);
    }
    item->setData(name,ItemDelegate::VarNameRole);
    item->setData(name,ItemDelegate::NameRole);
    item->setData(property,Qt::EditRole);
    item->setData(property,ItemDelegate::DefaultValRole);
}

void DataStore::setRelevance(QModelIndex &index)
{
    QList<const char*> names;

    QByteArray varName;
    QByteArray colName;

    /* 如果有定义列名称索引，则查看是否有对应的名称索引属性，此方法目前是针对数组数据的传输*/
    if(index.model()->headerData(index.column(),Qt::Horizontal,Qt::UserRole).isValid())
    {
        colName = index.model()->headerData(index.column(),Qt::Horizontal,Qt::UserRole).toString().toLatin1();
        names.append(colName.data());
    }

    /*正常的数据关联，通过数据的名称，查看是否有对应的属性进行更新*/
    varName  = index.data(ItemDelegate::VarNameRole).toString().toLatin1();
    names.append(varName.data());

    /*遍历查找对应的属性进行更新*/
    for(int i = 0;i<names.count();i++)
    {
        const char *name = names[i];
        for(int pos =0;pos < relevance.count();pos++)
        {
            QVariant property = relevance[pos]->property(name);
            if(property.isValid())
            {
                if(property.canConvert<ArrayVal>()) // 详细数据（目前用于数组的赋值，当然普通的关联也可以用）
                {
                    ArrayVal arrayVal;
                    arrayVal.colName  = colName;
                    arrayVal.colIndex = index.column();
                    arrayVal.rowIndex = index.row();
                    arrayVal.DisVal   = index.data(ItemDelegate::TempEditRole);
                    arrayVal.Val      = index.data(ItemDelegate::ActlValRole);
                    relevance[pos]->setProperty(
                                name,
                                QVariant::fromValue<ArrayVal>(arrayVal));
                }
                else // 其它关联项（直接关联，不通过详细数据结构体进行关联）
                {
                    if(index.data(ItemDelegate::ActlValRole).isValid())// 如果模型项中有实际值，则默然传输实际值
                    {
                        relevance[pos]->setProperty(name,index.data(ItemDelegate::ActlValRole)); // 默认映射为实际值
                    }
                    else // 如果没有实际值，则传输显示值
                    {
                        relevance[pos]->setProperty(name,index.data(ItemDelegate::TempEditRole)); //
                    }
                }
            }
        }
    }
}







