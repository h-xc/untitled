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

class qqSetUp : public QWidget
{
    Q_OBJECT
public:
    explicit qqSetUp(QWidget *parent = 0);
    ~qqSetUp();

    xmlDataStore *dataStore;
private:
    Ui::qqSetUp *ui;
public slots:

};

#endif // QQSETUP_H
