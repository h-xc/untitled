#include "mydoublevalidator.h"
#include <QDebug>
MyDoubleValidator::MyDoubleValidator(QObject * parent):QDoubleValidator(parent)
{

}

QValidator::State MyDoubleValidator::validate(QString &str, int &i) const
{
    if (str.isEmpty())
    {
        return QValidator::Intermediate;
    }
    bool cOK = false;
    double val = str.toDouble(&cOK);

    if (!cOK)  // 不能转换为浮点型数据
    {
        return QValidator::Invalid;
    }

    int dotPos = str.indexOf(".");

    if (dotPos > 0)  // 小小数点不在第1位
    {
        int decimalsLen = str.mid(dotPos+1).length();
        if (decimalsLen > decimals() || 0 == decimals())  // 小时点数超过设定值,或没有小数点
        {
            return QValidator::Invalid;
        }

        if(decimalsLen == 0)          // 小数点后没有填入数据
        {
            return QValidator::Intermediate;  // 为过渡值
        }
    }

    if(val <= top() && val >= bottom())  // 在设定范围内
    {
        return QValidator::Acceptable;
    }
    return  QValidator::Invalid;
}
