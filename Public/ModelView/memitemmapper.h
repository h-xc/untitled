/************************************************************
* Copyright (C), 2018-2018, NZ, Ltd.
* FileName: memitemmapper.h
* Author:   hxc
* Version : v1.0
* Date:     190315
* Description:
* 1、继承QStandardItem
* 2、将模型单个项和内存变量建立映射，可以将在模型内容发生变化后，直接减内容提交保存到内存变量中。
* 3、使用了模板编程，实现对不同的变量类型进行映射
*
* 注：使用模板类，要把函数实现放在头文件中（参照QList），不然会报“无法找到引用”，所以没有对应的.cpp文件
*
* Function List:
* 1. -------
* History:
* <author> <time>   <version > <desc>
* hxc      1900315  v1.0       创建编写
***********************************************************/

#ifndef MEMITEMMAPPER_H
#define MEMITEMMAPPER_H

#include <QObject>
#include <QVariant>
#include <QStandardItem>
#include <QDebug>
#include "item_delegate.h"

//class MemVariantMapper;m;
template<typename T>
class MemItemMapper:public QStandardItem
{
public:
    explicit MemItemMapper(T * = 0);
    //~MemItemMapper(){}
    void setData(const QVariant &value, int role = Qt::UserRole + 1);
    //virtual QVariant data(int role = Qt::UserRole + 1) const;
private:
    T *m_val;         // 内存变量的指针，用于保存数据
};

/* MemItemMapper记录内存变量指针，并将内存变量放入item中显示
 * memVal  输入变量，内存变量指针
 */
template<typename T>
MemItemMapper<T>::MemItemMapper(T *memVal)
{
    m_val       = memVal;
    QStandardItem::setData(qMetaTypeId<T>(),ItemDelegate::DataTypeRole);  // 标记数据类型
    QStandardItem::setData(QVariant::fromValue<T>(*memVal),Qt::EditRole); // 模型中数据
}

/* setData 虚函数当item保存内容时，将内容保存一份到内存变量中
 */
template<typename T>
void MemItemMapper<T>::setData(const QVariant &value, int role)
{
    QStandardItem::setData(value,role);
    //qDebug() << "setData" << value;
    if(role ==  ItemDelegate::StoreCtrlRole)  // 储存库操作 提交 恢复  // 应该要新开一个函数去实现，但这样也可以实现，以后优化再去写了
    {
        if(QStandardItem::data(ItemDelegate::StoreCtrlRole).toInt() == ItemDelegate::_SUBMIT)  // 模型提交到内存
        {
            QVariant tmp = QStandardItem::data(Qt::EditRole);  // 获取到模型中的数据
            (*m_val) = tmp.value<T>();  // 转换后赋值到内存变量   // 赋值到内存中
            qDebug() << "submit" << (*m_val);
        }
        else if(QStandardItem::data(ItemDelegate::StoreCtrlRole).toInt() == ItemDelegate::_REVERT)  // 内存提交到模型
        {
            qDebug() << "revert"  << (*m_val);
            QStandardItem::setData(QVariant::fromValue<T>(*m_val),Qt::EditRole); // 内存中数据提交到模型中
        }
    }
}



//template<typename T>
//QVariant MemItemMapper<T>::data(int role) const
//{
//    if(role == Qt::EditRole)
//   // qDebug() <<"getdata" << QStandardItem::data(NameRole) << QStandardItem::data(Qt::EditRole);
//    return  QStandardItem::data(role);
//}

#endif // MEMITEMMAPPER_H
