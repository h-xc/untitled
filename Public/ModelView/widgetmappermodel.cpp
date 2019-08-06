#include "widgetmappermodel.h"
#include <QDebug>


WidgetMapperModel::WidgetMapperModel(QObject *parent) : QStandardItemModel(parent)
{
    m_itemBindWidge.setModel(this);
    m_itemBindWidge.setOrientation(Qt::Vertical); // 映射方向为垂直
}

void WidgetMapperModel::setItemDelegate(ItemDelegate *delegate)
{
    delegate->viewDisp(false);   // 非表格视图模式
    m_itemBindWidge.setItemDelegate(delegate);
}

const QStandardItem * WidgetMapperModel::getMemItem(const void *memAddr)
{
    return m_mem_item_map.value(memAddr,0);
}

void WidgetMapperModel::addMapping(QWidget *widget,const void *memAddr, const QByteArray &propertyName)
{
    QStandardItem *item = m_mem_item_map.value(memAddr,0);
    if(item == 0)
    {
        qDebug() << "err WidgetMapperModel::addMapping  memAddr is not map mapping";
        return;
    }
    m_itemBindWidge.addMapping(widget,item->row(),propertyName);
    ItemDelegate::initEditor(widget,item->index());
}
void WidgetMapperModel::addMapping(QWidget *widget,const QStandardItem *item, const QByteArray &propertyName)
{
    if(item == 0)
    {
        qDebug() << "err WidgetMapperModel::addMapping  item is not map mapping";
        return;
    }
    m_itemBindWidge.addMapping(widget,item->row(),propertyName);
    ItemDelegate::initEditor(widget,item->index());
}



