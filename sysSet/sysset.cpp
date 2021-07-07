#include "sysset.h"
#include "ui_sysset.h"

/*串口结构体*/




sysSet::sysSet(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::sysSet)
{
    ui->setupUi(this);

    QStandardItem *parentItem = model.invisibleRootItem();
    for (int i = 0; i < 4; ++i) {
        QList<QStandardItem *> rows;
        for(int col = 0; col < 4; ++col)
        {
            rows.append( new QStandardItem(QString("item col%0").arg(col)));
        }
        parentItem->appendRow(rows);
        rows[0]->appendRow(new QStandardItem(QString("item col%0").arg(1)));
        rows[0]->appendRow(new QStandardItem(QString("item col%0").arg(2)));
    }

    ui->listView->setModel(&model);
}

sysSet::~sysSet()
{
    delete ui;
}
