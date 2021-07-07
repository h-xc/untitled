#include "struoffse.h"
#include <QDebug>
struOffse::struOffse(QObject *parent) : QObject(parent)
{
    struct student stu;
    int offset;
    char str[5][40];

    qDebug() << "str size" << QString("%1").arg(sizeof(str[0]));

    qDebug() << "student size" << QString("%1").arg(sizeof(stu));

    offset = GET_OFFSET(stu, d);
    qDebug() << "student offset" << QString("%1").arg(offset);

    double *d = &stu.d;
    qDebug() << "student pd" << QString("%1").arg(*d);

    d = (double *)(((char*)&stu)+offset);
    qDebug() << "student offset get pd " << QString("%1").arg(*d);


}
