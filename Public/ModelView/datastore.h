/************************************************************
Copyright (C), 2020-2020, NZ, Ltd.
FileName: datastore.h
Author:   hxc
Version : v1.0
Date:     200323
Description:
1、储存载体与模型的关联操作基类，每个不同的载体，可通过继承该类，进行了模型的初始化后
当前类通过统一和提供操作功能：
提交修改、撤销修改、恢复默认、数据改动消息、外部数据获取。

子类需要具体实现函数（即虚函数）
储存载体创建函数、模型转为储存载体函数、将载体转为模型函数

Function List:
1. -------
History:
<author> <time> <version > <desc>
hxc      200323  v1.0       创建编写
***********************************************************/
#ifndef DATASTORE_H
#define DATASTORE_H
#include <QObject>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QList>
#include <QMap>
#include <QSet>
#include <QDebug>
#include "item_delegate.h"

#define  TO_STR(___NAME___)  (#___NAME___)

/* 此宏只对可直接赋值的变量进行一对一绑定（如果其它不支持传输的，请自行使用Q_PROPERTY）
 * 注：此宏获取的数据默认是实际值，如果实际值不存在，则获取的是显示值
 * _TYPE_： 内存空间的类型
 * _NAME_:  模型中的索引名称
 * _MEMORY_：内存空间*/
#define U_RELEVANCE(_TYPE_,_NAME_,_MEMORY_)     \
Q_PROPERTY(_TYPE_  _NAME_  READ  get##_NAME_ WRITE set##_NAME_)\
_TYPE_  get##_NAME_(){return _MEMORY_;}         \
void  set##_NAME_(_TYPE_ data){_MEMORY_ = data;}

/*因为表格数组数据太过多，一个一个映射很繁琐，且可能会出错，以下宏使数组数据可批量关联，
 * 注：以下所有宏都是基于“列数组存储格式”，如下
 * arr{
 * col1[],
 * col2[],
 * }*/

// 一个可传输类型，可以将数组的结构和名称传入到模型中进行创建；
// QMap<列名称，QList<每一行的值>> ，因为每一个数组的组成类型可能都不一样，所以需要
// 自己具体实现对应的函数，创建ArrayList
struct RowItems
{
    QString  colName;  // 列标题索引
    QList<QVariant> Vals;
    void clear(){colName.clear();Vals.clear();}
};
typedef QList<RowItems> ArrayList;
Q_DECLARE_METATYPE(ArrayList)



// 用于传递数据值到关联类中，目前表格数组在用，以后可以修改为所有数据都使用，因为可以传
// 递更多数据
struct ArrayVal
{
    //
    QString  colName;  // 列标题索引
    int      rowIndex; // 行索引
    int      colIndex; // 列索引
    QVariant DisVal;   // 显示值
    QVariant Val;      // 实际值
};
Q_DECLARE_METATYPE(ArrayVal)

/* 用于获取模型数据字符更新到数组中，数据是以列名称为属性名，通过参数中的下标传递的数据
 * _ARR_NAME_: (会创建一个属性，模型通过该属性传递信息)
 * _ARR_MEM_：数组内存区
 * _ARR_SIZE_：数组长度
 * _STR_SIZE_：字符长度*/
#define U_STR_ARR_RELE(_ARR_NAME_,_ARR_MEM_,_ARR_SIZE_,_STR_SIZE_) \
Q_PROPERTY(ArrayVal _ARR_NAME_ READ get##_ARR_NAME_ WRITE set##_ARR_NAME_)\
ArrayVal get##_ARR_NAME_(){return ArrayVal();}\
void set##_ARR_NAME_(ArrayVal arrayVal)\
{\
    if(arrayVal.colIndex >= 0 && arrayVal.colIndex < _ARR_SIZE_)\
    {\
        memcpy(_ARR_MEM_[arrayVal.rowIndex],\
                arrayVal.DisVal.toString().toLocal8Bit().data(),\
                _STR_SIZE_);\
        _ARR_MEM_[arrayVal.rowIndex][_STR_SIZE_-1] = 0;\
    }\
}

/* 用于获取模型数据BIT8更新到数组中，数据是以列名称为属性名，通过参数中的下标传递的数据
 * _ARR_NAME_: (会创建一个属性，模型通过该属性传递信息)
 * _ARR_MEM_：数组内存区
 * _ARR_SIZE_：数组长度*/
#define U_BIT8_ARR_RELE(_ARR_NAME_,_ARR_MEM_,_ARR_SIZE_) \
Q_PROPERTY(ArrayVal _ARR_NAME_ READ get##_ARR_NAME_ WRITE set##_ARR_NAME_)\
ArrayVal get##_ARR_NAME_(){return ArrayVal();}\
void set##_ARR_NAME_(ArrayVal arrayVal)\
{\
    if(arrayVal.colIndex >= 0 && arrayVal.colIndex < _ARR_SIZE_)\
    {\
        _ARR_MEM_[arrayVal.rowIndex/8].U8 &= ~(0x01 << arrayVal.rowIndex%8);\
        if(arrayVal.DisVal.toBool())\
        {\
            _ARR_MEM_[arrayVal.rowIndex/8].U8 |= 0x01 << arrayVal.rowIndex%8;\
        }\
    }\
}

/* 用于获取模型数据普通类型更新到数组中，数据是以列名称为属性名，通过参数中的下标传递的数据
 * _TYPE_：数组类型
 * _ARR_NAME_: (会创建一个属性，模型通过该属性传递信息)
 * _ARR_MEM_：数组内存区
 * _ARR_SIZE_：数组长度*/
#define U_ARR_RELE(_TYPE_,_ARR_NAME_,_ARR_MEM_,_ARR_SIZE_) \
Q_PROPERTY(ArrayVal _ARR_NAME_ READ get##_ARR_NAME_ WRITE set##_ARR_NAME_)\
ArrayVal get##_ARR_NAME_(){return ArrayVal();}\
void set##_ARR_NAME_(ArrayVal arrayVal)\
{\
    if(arrayVal.colIndex >= 0 && arrayVal.colIndex < _ARR_SIZE_)\
    {\
        if(arrayVal.DisVal.canConvert<_TYPE_>())\
        {\
            _ARR_MEM_[arrayVal.rowIndex] = arrayVal.DisVal.value<_TYPE_>();\
        }\
    }\
}



/*模型结构体，用于记录模型属性************************************************/
struct ModelStruct{
    QString Name;     // 模型名称
    int View;         // 模型类型 表格 小控件
    QString Type;     // 模型类
    QString Content;  // 标签记录
    QAbstractItemModel* model;  // 模型对象指针
    QMap<QString,QStandardItem*> itemMap;
};

/*模型项结构体，用于记录模型项属性************************************************/
struct ItemDataStruct{
    ItemDataStruct(){
        VarName = QString();
        Name = QString();
        Unit  = QString();
        Desc = QString();
        showType = ItemDelegate::DefaultShow;
        checkType = ItemDelegate::String;
        //DataType = 0;
        editType = ItemDelegate::DefaultEdit;
        DefaultVal = QVariant();
        RangMin = QVariant();
        RangMax = QVariant();
        Step = QVariant();
        Regexp = QString();
        Content = QString();
        Decimal = 0;
        ComboMap = QMap<QString,QVariant>();
    }
    QString VarName;
    QString Name;
    QString Unit;
    QString Desc;
    ItemDelegate::ShowType showType;
    ItemDelegate::CheckType checkType;
    //int DataType;
    ItemDelegate::EditType editType;
    QVariant DefaultVal;
    QVariant RangMin;
    QVariant RangMax;
    QVariant Step;
    QString Regexp;
    QString Content;
    int Decimal;
    QMap<QString,QVariant> ComboMap;
};

class DataStore : public QObject
{
    Q_OBJECT
public:

    explicit DataStore(QObject *parent = 0);
     ~DataStore(){}
    /*操作接口*/
    virtual bool submit();             // 确认修改，提供定值修改确认
    virtual bool revert();             // 丢弃修改，恢复原定值 接口函数
    virtual bool restore();            // 恢复默认，提供定值修改按钮方法 接口函数

    bool m_isChangedFlag;
    virtual void initItemModel();      // 进行模型初始化

    QStandardItem *initItem(ItemDataStruct &itemDataStruct);


    /*标准模型(文件读取后，将读取到的内容映射到此模型数组中，显示控件使用此模型数组进行显示)*/
    enum ModelView
    {
        Table,    // 表格视图
        Control,  // 小控件视图
        Table_Tab,    // 放在标签页中
    };
    Q_ENUM(ModelView)

    QList<ModelStruct*> m_Models;
    const QList<ModelStruct*> *getModels()
    {
       return &m_Models;
    }

    void addModels(QList<ModelStruct*> &models)
    {
        m_Models.append(models);
    }

    void addModel(ModelStruct* modelStruct)
    {
        m_Models.append(modelStruct);
    }

    // 更具名称获取模型
    ModelStruct* getModel(QString modelName);
    ModelStruct* getModel(const QAbstractItemModel* model);
    void CleanModels(); // 清除

    QSet <QStandardItem *> itemChangedList; // 记录被修改过的项


    /* createStore 创建储存载体
     * return    false 创建失败        true 成功*/
    virtual bool createStore();

    /* saveToStore 保存定值，将定值内存变量储存到储存载体
     * return    false 储存失败        true 成功*/
    virtual bool saveToStore();

    /* fetchOfStore 读取定值，将定值从储存载体读取到内存变量
    * return    false 储存载体不存在    true 成功*/
    virtual bool fetchOfStore();


    /*模型数据获取*/
    // 使用字符名进行索引
    QVariant getValData(QString modelName,QString itemName,int role = Qt::EditRole);
    // 使用行列编号进行索引
    QVariant getValData(QString modelName,int itemRow,int itemCol,int role = Qt::EditRole);

    /* 在保存或读取定值，对设置模型项数据，默认值，实际值，临时值进行初始化
     * index 模型项索引
     * val   初始化值
     * role  标记“初始化值val”是什么类型值，显示值，实际值*/
    bool initItemData(QAbstractItemModel *model,QModelIndex index,QVariant val,int role = ItemDelegate::ActlValRole);

    /* 显示值与实际值的转换
     *  index   模型项的索引
     *  dispVal 显示值
     *  actuVal 实际值
     *  flag 0  实际值到显示值 1 显示值到实际值
     *  return  0 转换失败    1 转换成功  2 无需转换*/
    static int actlDispConvert(QModelIndex &index, QVariant &dispVal,QVariant &actuVal,int flag = 0);



    /*结构体与模型关联操作函数*/
    QList<QObject*> relevance;// 数据关联类
    void addRelevance(QObject *obj)
    {
        relevance.append(obj);
    }

    // 在模型被读取后，调用该函数，检测读取的模型是否有关联类对应的属性，若没有，则创建
    void createRelevanceModel();

    // 初始化关联类对应的模型项item
    void initRelevanceItem(QString name,QVariant &property,QStandardItem *item);

    // 通过该函数对关联的属性进行赋值
    void setRelevance(QModelIndex &index);


    /*信号与槽*/
signals:
    void FixValChanged();  // 用于发送定值修改触发信号
private slots:
    virtual void itemChanged(QStandardItem *item);  // 模型中有数据被修改了，则会调用此函数，并将被修改的模型项传入
};



#endif // DATASTORE_H
