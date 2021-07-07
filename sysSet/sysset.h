#ifndef SYSSET_H
#define SYSSET_H

#include <QWidget>
#include <QStandardItemModel>
#include <QStandardItem>
namespace Ui {
class sysSet;
}



class sysSet : public QWidget
{
    Q_OBJECT

public:
    explicit sysSet(QWidget *parent = 0);
    ~sysSet();

    QStandardItemModel model;
private:
    Ui::sysSet *ui;
};

#endif // SYSSET_H
