
#ifndef XMLDATASTORE_H
#define XMLDATASTORE_H
#include "datastore.h" // 基类应以了各种接口
#include <QString>
#include <QDomDocument>
#include <QFile>
#include <QTextStream>
#include <QMetaEnum>
#include <QDebug>
/*XML操作相关定义 ATTR_属性名  ELEMENT_元素名*/
// 通用属性名称
#define ELEMENT_DATABASE    "DataBase"     // XML的根元素
#define ATTR_VERSION        "Version"      // XML的版本
#define ATTR_VERSION_1_0    "1.0"


// 模型元素及属性名称
#define ELEMENT_MODEL       "Model"        // 模型元素名称 一个文件可以有多个模型

// 表格视图类型相关元素及属性名称
#define ELEMENT_HEADER    "Header"        // 表格属性元素
#define ELEMENT_H_HEADER  "H_Header"
#define ELEMENT_V_HEADER  "V_Header"
#define ATTR_NAME         "disName"         // 表格列名称定义元素
#define ATTR_VAR          "varName"      // 表格列索引名称定义元素



// 数据元素及属性名称
#define ELEMENT_DATA        "Data"         // 数据元素名称 一个模型有多个数据元素
#define COL_END             "ColEnd"       // 列结束标记
#define ROW_END             "RowEnd"       // 行结束标记

// 记录在界面中的显示方式和输入检测的元素与及属性名称
#define ELEMENT_INTERFACE   "Interface"    // 记录在界面中的显示方式和输入检测的元素名称

// 变量值描述体元素
#define ELEMENT_VAL         "Val"
#define ATTR_EDITABLE       "Editable"

// 值选项
#define ELEMENT_COMBO       "Combo"
#define ATTR_INTERFACENAME  "InterfaceName" // 在界面上的显示


class xmlDataStore:public DataStore
{
public:
    xmlDataStore(QObject *parent = 0);

    QString xmlFileName;

    bool createStore();   // 用于在没有储存载体文件，创建对应的储存载体

    /*接口类函数*/
    /*saveToStore 读取定值，将定值从储存载体读取到内存变量
     * return    false 打开储存载体失败  true 写入成功*/
    bool saveToStore();                   // 保存定值，将内存变量保存到储存载体中

    /*addXmlData 通过模型项item初始化对应的Data元素*/
    QDomNode addXmlData(QDomDocument &doc,QDomElement &root,QModelIndex *item);

    /*fetchOfStore 读取定值，将定值从储存载体读取到内存变量
     * return    false 储存载体不存在    true 读取成功 */
    bool fetchOfStore();                   // 读取定值，将定值从储存载体读取到内存

    /*getXmlData 通过对应的Data初始化模型项item*/
    void getXmlData(QDomElement &Data,QStandardItem *item);

};

#endif // XMLDATASTORE_H
