#include "datastore.h"

#include <QDebug>


DataStore::DataStore(QObject *parent) : QObject(parent)
{
    itemChangedList.clear();
    m_isChangedFlag = false;
}
void DataStore::itemChanged(QStandardItem *item)
{
    itemChangedList.insert(item);
}

bool  DataStore::submit()       //  确认修改，提供定值修改确认
{
    if(!itemChangedList.isEmpty())
    {
        foreach (QStandardItem *value, itemChangedList)
        {
            value->setData(value->data(Qt::EditRole),ItemDelegate::TempRole);  // 如果确定修改，则储存一份，用于撤销修改时使用
        }
        saveToStore();
        itemChangedList.clear();  // 清除记录的改变的项
        emit FixValChanged();  // 发送定值改变事件
    }
    return true;
}

bool  DataStore::revert()       //  丢弃修改，恢复原定值 接口函数
{
    foreach (QStandardItem *value, itemChangedList)  // 只恢复修改过的定值
    {
        value->setData(value->data(ItemDelegate::TempRole),Qt::EditRole);  // 如果确定修改，则储存一份，用于撤销修改时使用
    }
    itemChangedList.clear();
    return true;
}

bool  DataStore::restore()     // 恢复默认，提供定值修改按钮方法 接口函数
{
    for(int pos=0;pos <m_Models.count();pos++)
    {
        for(int row=0;row <m_Models[pos]->model->rowCount();row ++)
            for(int col = 0;col <m_Models[pos]->model->columnCount();col++)
            {
                QModelIndex index = m_Models[pos]->model->index(row,col);
                if(!m_Models[pos]->model->data(index,ItemDelegate::VarNameRole).toString().isEmpty())
                {
                    m_Models[pos]->model->setData(index,m_Models[pos]->model->data(index,ItemDelegate::DefaultValRole),Qt::EditRole);
                }
            }
    }
    return true;
}

QStandardItem * DataStore::initItem(QString VarName,
                                      QString Name,
                                      QString Unit,
                                      QString Desc,
                                      ItemDelegate::ShowType showType,
                                      ItemDelegate::CheckType checkType,
                                      int DataType,
                                      ItemDelegate::EditType editType,
                                      QVariant DefaultVal,
                                      QVariant RangMin,
                                      QVariant RangMax,
                                      QVariant Step,
                                      QString Regexp,
                                      QString Content,
                                      int Decimal,
                                      QMap<QString,QVariant> ComboMap)
{
    QStandardItem *item =new QStandardItem();

    if(!VarName.isEmpty())
        item->setData(VarName,ItemDelegate::VarNameRole);

    if(!Name.isEmpty())
        item->setData(Name,ItemDelegate::NameRole);

    if(!Unit.isEmpty())
        item->setData(Unit,ItemDelegate::UnitRole);

    if(!Desc.isEmpty())
        item->setData(Desc,ItemDelegate::DescRole);
    item->setData(showType,ItemDelegate::ShowTypeRole);
    item->setData(checkType,ItemDelegate::CheckTypeRole);
    item->setData(DataType,ItemDelegate::DataTypeRole);
    item->setData(editType,ItemDelegate::EditTypeRole);

    if(DefaultVal.isValid())
        item->setData(RangMin,ItemDelegate::DefaultValRole);

    if(RangMin.isValid())
        item->setData(RangMin,ItemDelegate::RangMinRole);

    if(RangMax.isValid())
        item->setData(RangMax,ItemDelegate::RangMaxRole);

    if(Step.isValid())
        item->setData(Step,ItemDelegate::StepRole);

    if(!Regexp.isEmpty())
        item->setData(Regexp,ItemDelegate::RegexpRole);

    if(!Content.isEmpty())
        item->setData(Content,ItemDelegate::ContentRole);

    if(Decimal>0)
        item->setData(Decimal,ItemDelegate::DecimalRole);

    if(!ComboMap.isEmpty())
        item->setData(ComboMap,ItemDelegate::ComboMapRole);
    return item;
}


void DataStore::CleanModels()
{
    for(int pos;pos<m_Models.count();pos++)
    {
        delete m_Models[pos]->model;
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
    return true;
}
void DataStore::initItemModel()
{

}

QVariant DataStore::getValData(QString modelName,QString itemName,int role /*= Qt::EditRole*/)
{
    for(int pos= 0;pos<m_Models.count();pos++)
    {
        if(m_Models[pos]->Name == modelName)
        {
            return m_Models[pos]->itemMap.value(itemName)->data(role);
        }
    }
    return QVariant();
}

QVariant DataStore::getValData(QString modelName,int itemRow,int itemCol,int role /*= Qt::EditRole*/)
{
    for(int pos= 0;pos<m_Models.count();pos++)
    {
        if(m_Models[pos]->Name == modelName)
        {
            QModelIndex index =  m_Models[pos]->model->index(itemRow,itemCol);
            return m_Models[pos]->model->data(index,role);
        }
    }
    return QVariant();
}




