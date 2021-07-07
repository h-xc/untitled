#ifndef QQSETUP_H
#define QQSETUP_H

#include <QWidget>
#include <QMap>
#include <QStandardItemModel>
#include <QModelIndex>
#include ".\Public\ModelView\settingpanel.h"
#include ".\Public\ModelView\xmldatastore.h"
namespace Ui {
class qqSetUp;
}
struct BIT
{
    quint32 NeutralGndMode:1;
};


struct Mystruct
{
    char DevName[32];
    qint8 NeutralGndMode;
    qint32 OffsetU0Max;
    qint32 val1;
};


class Myclass: public QObject
{
   Q_OBJECT
public:
    Myclass(QObject*parent = 0 ):
        QObject(parent){}
    ~Myclass(){}

   int val1;
   Q_PROPERTY(int val1 MEMBER val1)

};

class qqSetUp : public QWidget
{
    Q_OBJECT
public:
    explicit qqSetUp(QWidget *parent = 0);
    ~qqSetUp();

    xmlDataStore *dataStore;
private:
    Ui::qqSetUp *ui;
    Mystruct mstruct;

public slots:
};

#endif // QQSETUP_H
