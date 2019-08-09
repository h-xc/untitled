#ifndef QQSETUP_H
#define QQSETUP_H

#include <QWidget>
#include <QMap>
#include <QStandardItemModel>
#include <QModelIndex>
#include ".\Public\ModelView\settingpanel.h"
namespace Ui {
class qqSetUp;
}

class qqSetUp : public QWidget
{
    Q_OBJECT

public:
    explicit qqSetUp(QWidget *parent = 0);
    ~qqSetUp();
    QMap<QString,QStandardItemModel*> m_Models;
private:
    Ui::qqSetUp *ui;
    bool fetchOfStore();
public slots:

};

#endif // QQSETUP_H
