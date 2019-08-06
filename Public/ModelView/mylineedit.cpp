#include "mylineedit.h"
#include <QKeyEvent>
#include <QValidator>
#include <QDebug>
MyLineEdit::MyLineEdit(QWidget *parent):QLineEdit(parent)
{

}
// 输入框获取到焦点，先将原值保存起来。
void MyLineEdit::focusInEvent(QFocusEvent *e)
{
    m_text = text();
    QLineEdit::focusInEvent(e);
}
// 当输入框失去失去焦距时，若为过渡值，则将过渡值恢复为原值，因为过渡值也是不合法值
void MyLineEdit::focusOutEvent(QFocusEvent *e)
{
    IntermediateRecover();
    QLineEdit::focusOutEvent(e);
}

void MyLineEdit::keyPressEvent(QKeyEvent *event)// 输入的值为过渡值的时候，不会触发该事件
{
    if(event->key() == Qt::Key_Enter)
    {
        IntermediateRecover();
    }
    QLineEdit::keyPressEvent(event);
}

void MyLineEdit::IntermediateRecover()
{
    if(!hasAcceptableInput()) // 输入不合法
    {
        setText(m_text);      // 恢复原值
    }
}
