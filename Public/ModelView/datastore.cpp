#include "datastore.h"
#include "item_delegate.h"
#include <QDebug>
DataStore::DataStore(QObject *parent) : QObject(parent)
{
    itemChangedList.clear();
    m_isChangedFlag = false;
    m_restoreFlag   = false;
    m_initItemModelFlag = false;
}
void DataStore::itemChanged(QStandardItem *item)
{
    //    if(m_initItemModelFlag == false)
    //    {
    //        itemList.insert(item);
    //        qDebug() << "itemList"<<itemList.size() << item;
    //        return;
    //    }
    itemChangedList.insert(item);
    m_isChangedFlag = true;
    qDebug() << "itemChangedList" <<itemChangedList.size() << item;
}

bool  DataStore::submit()       //  确认修改，提供定值修改确认
{
    foreach (QStandardItem *value, itemChangedList)
    {
        value->setData(_SUBMIT,StoreCtrlRole);
    }
    itemChangedList.clear();  // 清除改变事件

    return true;
}

bool  DataStore::revert()       //  丢弃修改，恢复原定值 接口函数
{
    if(m_restoreFlag == true)   // 进行了恢复默认操作
    {
        //        fetchOfStore();         //  需要从储存载体中获取原设定值，因为内存变量已经在恢复默认时被修改
        //        foreach (QStandardItem *value, itemList)  // 更新所有view
        //        {
        //            value->setData(revert,StoreCtrlRole);
        //        }
        //        qDebug() << "revert"  << m_restoreFlag ;
        qDebug() << "fetchOfStore"  ;
        qDebug() << "AllRevert"  ;
        m_restoreFlag = false;
    }
    else
    {
        foreach (QStandardItem *value, itemChangedList)  // 只恢复修改过的定值
        {
            value->setData(_REVERT,StoreCtrlRole);
        }
        itemChangedList.clear();
    }
    qDebug() << "DataStore::revert"  ;
    return true;
}
bool DataStore::saveToStore()
{
    emit FixValChanged();  // 发送定值改变事件
    return true;
}  // 保存定值，将内存变量保存到储存载体中



bool  DataStore::restore()     // 恢复默认，提供定值修改按钮方法 接口函数
{
    m_restoreFlag = true;
    //    initFixVal();   // 内存变量初始化为默认值
    //    foreach (QStandardItem *value, itemList)  // 更新所有view
    //    {
    //        value->setData(revert,StoreCtrlRole);
    //    }
    qDebug() << "restore" ;
    return true;
}
void DataStore::initItemModel()
{
    m_initItemModelFlag = true; // 标记初始化已经完成
}




