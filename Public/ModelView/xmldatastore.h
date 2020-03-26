
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

#define ATTR_NAME           "Name"         // 名称属性

// 模型元素及属性名称
#define ELEMENT_MODEL       "Model"        // 模型元素名称 一个文件可以有多个模型

// 表格视图类型相关元素及属性名称
#define ELEMENT_HEADER      "Header"       // 表格属性元素
#define ELEMENT_HORIZONTAL  "Horizontal"   // 表格列名称定义元素
#define ELEMENT_VERTICAL    "Vertical"     // 表格行名称定义元素

// 数据元素及属性名称
#define ELEMENT_DATA        "Data"         // 数据元素名称 一个模型有多个数据元素
#define ATTR_ITEMROW        "ItemRow"      // 数据项在模型中所处行属性
#define ATTR_ITEMCOL        "ItemCol"      // 数据项在模型中所处行属性

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
     * return    false 打开储存载体失败  true 写入成功
     */
    bool saveToStore();                   // 保存定值，将内存变量保存到储存载体中

    /*initMemFixVal 初始化内存定值,应用于首次打开软件，未创建储存载体时使用
     * return  false 初始化失败  true 成功*/
    void addXmlData(QDomDocument &doc,QDomElement &root,QModelIndex *item);

    /*fetchOfStore 读取定值，将定值从储存载体读取到内存变量
     * return    false 储存载体不存在    true 读取成功 */
    bool fetchOfStore();                   // 读取定值，将定值从储存载体读取到内存

};

#endif // XMLDATASTORE_H
