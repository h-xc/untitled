/************************************************************
* Copyright (C), 2018-3000, NZ, Ltd.
* FileName: mydoublevalidator.h
* Author:   hxc
* Version : v1.0
* Date:     190424
* Description:
* 1、继承QDoubleValidator
* 2、QT自带的输入浮点型输入检测是支持科学计数法的，且对于上下限的限制也是错误的。
*
* Function List:
* 1. -------
* History:
* <author> <time>   <version > <desc>
* hxc      190424  v1.0       创建编写
***********************************************************/

#ifndef MYDOUBLEVALIDATOR_H
#define MYDOUBLEVALIDATOR_H
#include <QDoubleValidator>

class MyDoubleValidator : public QDoubleValidator
{
public:
    MyDoubleValidator(QObject * parent = 0);

    virtual QValidator::State validate(QString &str, int &i) const;
};

#endif // MYDOUBLEVALIDATOR_H
