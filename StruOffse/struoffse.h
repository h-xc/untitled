#ifndef STRUOFFSE_H
#define STRUOFFSE_H

#include <QObject>

#define GET_OFFSET(data,member)   (size_t)(&(((decltype(data)*)0)->member))

struct student{
    int a = 2;
    char ch[2] = {1,2};
    double d = 8;
};

class struOffse : public QObject
{
    Q_OBJECT
public:
    explicit struOffse(QObject *parent = nullptr);

signals:

public slots:
};

#endif // STRUOFFSE_H
