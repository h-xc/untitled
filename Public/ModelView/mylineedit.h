/************************************************************
* Copyright (C), 2018-3000, NZ, Ltd.
* FileName: mylineedit.h
* Author:   hxc
* Version : v1.0
* Date:     190424
* Description:
* 1、继承QLineEdit
* 2、QT自带的输入，若失去焦距，对于过度值，也会进行保存，如空值，是所有设定值的过渡值，但并不是最终值，后果是导致输入框可以为空。
*    注：过渡值：为不合法值，但可发展为合法值，如浮点数"0.1"，在输入"0."是过渡的，但却不合法，继续输入"1"才合法。
* 3、在失去焦距或按回车后，会判断输入框输入的值是否为合法值，若不合法，则将值恢复。
*
* Function List:
* 1. -------
* History:
* <author> <time>   <version > <desc>
* hxc      190424  v1.0       创建编写
***********************************************************/
#ifndef MYLINEEDIT_H
#define MYLINEEDIT_H
#include <QLineEdit>

class MyLineEdit : public QLineEdit
{
public:
    QString m_text;
    explicit MyLineEdit(QWidget *parent = 0);
    void focusInEvent(QFocusEvent *e);
    void focusOutEvent(QFocusEvent *e);
    void keyPressEvent(QKeyEvent *event);
    void IntermediateRecover();
};

#endif // MYLINEEDIT_H
