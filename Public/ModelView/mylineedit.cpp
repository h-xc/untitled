#include "mylineedit.h"
#include <QKeyEvent>
#include <QValidator>
#include <QDoubleValidator>
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
    IntermediateRecover();      //
    QLineEdit::focusOutEvent(e);
}

void MyLineEdit::keyPressEvent(QKeyEvent *event)// 输入的值为过渡值的时候，不会触发该事件
{
    //qDebug() << "MyLineEdit::keyPressEvent" <<event;
    if(event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter)// 大小键盘enter
    {
        IntermediateRecover();
        clearFocus();
    }
    QLineEdit::keyPressEvent(event);
}


void MyLineEdit::IntermediateRecover()
{
    if(!hasAcceptableInput()) // 若输入值不可用，那么只会是过渡值，因为不合法值在输入的时候已经被拦截，是无法到达这里的
    {
       if(text().isEmpty())
       {
            setText(m_text);
       }
    }
    else
    {
        if(validator() !=NULL)
        {
            if(validator()->metaObject()->className() == QDoubleValidator::staticMetaObject.className())
            {
                QDoubleValidator *doubleValidator = (QDoubleValidator*)(validator());
                setText(QString().setNum(text().toFloat(),'f',doubleValidator->decimals()));      // 恢复原值
            }
        }
    }
}
