/************************************************************
Copyright (C), 2018-2018, NZ, Ltd.
FileName: memitemmapper.h
Author:   hxc
Version : v1.0
Date:     190315
Description:
1、将模型和控件建立映射，
2、可以将在控件内容(非表格视图)发生变化后，直接提交保存到模型数据中。
Function List:
1. -------
History:
<author> <time> <version > <desc>
hxc      1900315 v1.0       创建编写
***********************************************************/

#ifndef WIDGETMAPPERMODEL_H
#define WIDGETMAPPERMODEL_H

#include <QObject>
#include <QDataWidgetMapper> //
#include <QStandardItem> //
#include <QWidget> //
#include <QStandardItemModel> //

#include "item_delegate.h"
#include "memitemmapper.h"

class WidgetMapperModel : public QStandardItemModel
{
    Q_OBJECT
public:
    explicit WidgetMapperModel(QObject *parent = 0);

    void addMapping(QWidget *widget,const void *memAddr = 0, const QByteArray &propertyName = QByteArray());

    void addMapping(QWidget *widget,const QStandardItem * = 0, const QByteArray &propertyName = QByteArray());

    void setItemDelegate(ItemDelegate *delegate);

    template<typename T>
    QStandardItem * addItem(T *memAddr = 0);

    const QStandardItem * getMemItem(const void *memAddr);
    //QStandardItemModel m_model;
//private:
            // 模型类
    QDataWidgetMapper  m_itemBindWidge;   // 用于模型项和小控件建立映射的操作类
    QMap<const void *, QStandardItem *>   m_mem_item_map; // 映射
signals:

public slots:
};

template<typename T>
QStandardItem * WidgetMapperModel::addItem(T *memAddr)
{
    if(memAddr == 0)
    {
        return 0;
    }
    QStandardItem *item = new MemItemMapper<T>(memAddr);
    //qDebug() << "WidgetMapperModel::addItem";
    this->appendRow(item);
    m_mem_item_map.insert(memAddr,item);  // 建立索引
    return item;
}



#endif // WIDGETMAPPERMODEL_H
