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
#include "item_delegate.h"
#define  TO_STR(___NAME___)  (#___NAME___)

/*模型结构体，用于记录模型属性*/
struct ModelStruct{
    QString Name;     // 模型名称
    QString View;     // 模型使用的视图
    QString Type;     // 模型类型
    QString Content;  // 标签记录
    QAbstractItemModel* model;  // 模型对象指针
    QMap<QString,QStandardItem*> itemMap;
};
class DataStore : public QObject
{
    Q_OBJECT
public:

    explicit DataStore(QObject *parent = 0);
    virtual bool submit();             // 确认修改，提供定值修改确认
    virtual bool revert();             // 丢弃修改，恢复原定值 接口函数
    virtual bool restore();            // 恢复默认，提供定值修改按钮方法 接口函数

    bool m_isChangedFlag;

    virtual void initItemModel();      // 进行模型初始化
    QStandardItem *initItem(QString VarName,
                             QString Name ,
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
                             int Decimal = 0,
                             QMap<QString,QVariant> ComboMap = QMap<QString,QVariant>());


    /*标准模型(文件读取后，将读取到的内容映射到此模型中，显示控件使用此模型进行显示)*/
    QList<ModelStruct*> m_Models;
    void CleanModels();

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

    // 使用字符名进行索引
    QVariant getValData(QString modelName,QString itemName,int role = Qt::EditRole);

    // 使用行列进行索引
    QVariant getValData(QString modelName,int itemRow,int itemCol,int role = Qt::EditRole);


    /*信号与槽*/
signals:
    void FixValChanged();  // 用于发送定值修改触发信号
private slots:
    virtual void itemChanged(QStandardItem *item);  // 模型中有数据被修改了，则会调用此函数，并将被修改的模型项传入
};

#endif // DATASTORE_H
