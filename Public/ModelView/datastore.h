#ifndef DATASTORE_H
#define DATASTORE_H
#include <QObject>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QList>
#include <QMap>
#include <QSet>

class DataStore : public QObject
{
    Q_OBJECT
public:

    explicit DataStore(QObject *parent = 0);
    virtual bool submit();             // 确认修改，提供定值修改确认
    //virtual bool submitAll();             // 确认修改，提供定值修改确认

    virtual bool revert();             // 丢弃修改，恢复原定值 接口函数
    //virtual bool revertAll();             // 确认修改，提供定值修改确认
    virtual bool restore();            // 恢复默认，提供定值修改按钮方法 接口函数

    bool m_restoreFlag;                // 标记上一次是否恢复默认值操作    false：没有     true：有
    bool m_isChangedFlag;              // 标记定值界面数据是否有改动（避免没有修改而去储存）  false：未有改动  true：有改动
    bool m_initItemModelFlag;          // 标记是否已经初始化model        false：没有     true：没有
    virtual void initItemModel();      // 进行模型初始化

//private:

    QSet <QStandardItem *> itemChangedList; // 记录修改过的定值项
    //QSet <QStandardItem *> itemList;

    /*接口类函数*/
    /*initFixVal 初始化内存定值,应用于首次打开软件，未创建储存载体时 或 恢复默认时  使用
    * return  false 初始化失败  true 成功
    */
    //virtual bool initFixVal(){return true;}

    /*
    * saveToStore 保存定值，将定值内存变量储存到储存载体
    * return    false 储存失败  true 成功
    */
    virtual bool saveToStore();

    /*
    * fetchOfStore 读取定值，将定值从储存载体读取到内存变量
    * return    false 储存载体不存在    true 读取成功
    */
    virtual bool fetchOfStore()
    {
        return true;
    }                       // 读取定值，将定值从储存载体读取到内存

    /*
    * initItemModel 初始化Ttem和model
    * 填充item的属性，并添加到model中
    */

    /*信号与槽*/
signals:
    void FixValChanged();  // 定值修改触发信号
private slots:
    virtual void itemChanged(QStandardItem *item);  // 接收定值变动信号
};

#endif // DATASTORE_H
