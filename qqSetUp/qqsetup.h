#ifndef QQSETUP_H
#define QQSETUP_H

#include <QWidget>
#include <QMap>
#include <QStandardItemModel>
namespace Ui {
class qqSetUp;
}

class qqSetUp : public QWidget
{
    Q_OBJECT

public:
    explicit qqSetUp(QWidget *parent = 0);
    ~qqSetUp();

private:
    Ui::qqSetUp *ui;
    bool fetchOfStore();
    QMap<QString,QStandardItemModel*> m_Models;
};

#endif // QQSETUP_H
