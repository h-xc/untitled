#include "item_delegate.h"

#include <QComboBox>
#include <QDoubleSpinBox>
#include <QtDebug>
#include <QStandardItemModel>
#include <QLineEdit>
#include <QValidator>
#include <QRegExp>
#include <QApplication>
#include <QMap>
#include <QToolTip>

#include "mydoublevalidator.h"
#include "mylineedit.h"

ItemDelegate::ItemDelegate(QObject *parent) : QStyledItemDelegate(parent)
{
    m_isView = true;  // 默认对表格代理
}

ItemDelegate::~ItemDelegate()
{

}

QWidget * ItemDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    //qDebug()<< "createEditor";
    int thisEdit = index.data(EditTypeRole).toInt();  // 获取编辑器类型
    //if(thisEdit == CheckBox){}else                      // 复选框编辑器 (使用系统自带，不用创建)
    if(thisEdit == ComboBox)                              // 下拉框编辑器
    {
        QComboBox *ComboEdit = new QComboBox(parent);               // 调用完后会自动释放，不用担心内存泄漏
        initEditor(ComboEdit,index);// 初始化输入检测
        return ComboEdit;  // 下拉框不需要设定其它输入检测，直接返回
    }
    else if(thisEdit == LineEdit || thisEdit ==  DefaultEdit)  // 文本输入框
    {
        // 使用表格视图双击默认返回的lineedit   dynamic_cast<T*>(a) 将a值转换为类型为T的对象指针。如果类型T不是a的某个基类型，该操作将返回一个空指针。
        QLineEdit *LineEdit = new MyLineEdit(parent);
        //dynamic_cast<QLineEdit*>(QStyledItemDelegate::createEditor(parent,option,index));
        if(LineEdit != 0)
        {
            initEditor(LineEdit,index);// 初始化输入检测
            return LineEdit;
        }
    }
    else
    {
        return QStyledItemDelegate::createEditor(parent,option,index);  // 若未定义类型，则调用父类原方法
    }

}

void ItemDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    //qDebug()<< "setEditorData";
    if(m_isView)  // 表格显示
    {
        /* 表格编辑器的内容初始化在此定义*/
        if(editor->metaObject()->className() == QComboBox::staticMetaObject.className())  // 下拉框编辑器
        {
            QComboBox *comboBox = static_cast<QComboBox*>(editor);               // 获取下拉框编辑器
            int comboIndex = comboBox->findData(index.model()->data(index,Qt::EditRole));//获取模型数据在下拉框编辑器中的位置
            comboBox->setCurrentIndex(comboIndex);                              // 将下列框的选中项更新为模型数据指定的位置
            return;
        }
        else if(editor->metaObject()->className() == QLineEdit::staticMetaObject.className()) // 文本框编辑器
        {
            QStyledItemDelegate::setEditorData(editor,index);

            /* 显示格式在此定义  */
            return;
        }
        QStyledItemDelegate::setEditorData(editor,index);  
        return;
    }
    else  // 非表格显示
    {
        /* 小控件映射的内容初始化在此定义 */
        /* 显示格式在此定义  */
        if(editor->metaObject()->className() == QComboBox::staticMetaObject.className())  // 下拉框编辑器
        {
            QComboBox *comboBox = static_cast<QComboBox*>(editor);               // 获取下拉框编辑器
            int comboIndex = comboBox->findData(index.model()->data(index,Qt::EditRole));//获取模型数据在下拉框编辑器中的位置
            comboBox->setCurrentIndex(comboIndex);       // 将下列框的选中项更新为模型数据指定的位置
            return;
        }
        else if(editor->metaObject()->className() == QLineEdit::staticMetaObject.className())
        {
            QLineEdit *LineEdit = static_cast<QLineEdit*>(editor);
            LineEdit->setText(index.model()->data(index,Qt::EditRole).toString());   //
            //model->setData(index, comboBox->itemData(comboBox->currentIndex(), Qt::EditRole);
            //model->setData(index, comboBox->currentText(), TempRole);
            //qDebug()<< "m_isView";
            return;
        }

    }
    QStyledItemDelegate::setEditorData(editor,index);
}

void ItemDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    //qDebug()<< "setModelData";
    if(m_isView)   // 把小控件和表格的处理方式分开，以后再考虑要不要合并一起处理
    {
        // 表格编辑器的内容获取在此定义
        if(editor->metaObject()->className() == QComboBox::staticMetaObject.className())
        {
            QComboBox *comboBox = static_cast<QComboBox*>(editor);
            model->setData(index, comboBox->itemData(comboBox->currentIndex()), Qt::EditRole);
            //model->setData(index, comboBox->currentText(), TempRole);
            return;
        }
        if(editor->metaObject()->className() == QLineEdit::staticMetaObject.className())
        {
            QLineEdit *lineEdit = static_cast<QLineEdit*>(editor);
            if(!lineEdit->hasAcceptableInput()) // 如果输入的数值不合法，则直接丢弃，并恢复原来的定值
            {
                lineEdit->setText(index.data(Qt::EditRole).toString());
                // 弹出气泡提示
                return;
            }
            else  // 输入合法，格式化字符，如浮点型，保留小数点
            {
                if(lineEdit->validator() ==NULL) return;
                if(lineEdit->validator()->metaObject()->className() == QDoubleValidator::staticMetaObject.className())
                {
                    QDoubleValidator *doubleValidator = (QDoubleValidator*)(lineEdit->validator());
                    //qDebug() << doubleValidator->decimals();
                    lineEdit->setText(QString().setNum(lineEdit->text().toFloat(),'f',doubleValidator->decimals()));      // 恢复原值
                }
            }
        }
    }
    else
    {
        // 小控件的内容获取在此定义
        // 表格编辑器的内容获取在此定义
        if(editor->metaObject()->className() == QComboBox::staticMetaObject.className())
        {
            QComboBox *comboBox = static_cast<QComboBox*>(editor);
            model->setData(index, comboBox->itemData(comboBox->currentIndex()), Qt::EditRole);
            //model->setData(index, comboBox->currentText(), TempRole);
            return;
        }
    }
    QStyledItemDelegate::setModelData(editor,model,index);
}

void ItemDelegate::paint(QPainter *painter,const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    int thisEdit = index.data(EditTypeRole).toInt();  // 获取编辑器类型
    if(thisEdit == ComboBox)  // 为了让表格显示下拉框的选中的显示内容，而不是显示数字值
    {
        QString strTem;
        QMap<QString,QVariant> ComboMap = index.data(ComboMapRole).toMap();
        QMap<QString, QVariant>::const_iterator i = ComboMap.constBegin();
        while (i != ComboMap.constEnd()) {
            if(i.value() == index.data(Qt::EditRole))
            {
                strTem = i.key();
                break;
            }
            i++;
        }
        if(i == ComboMap.constEnd())
        {
            return;
        }
        QStyleOptionViewItem myOption = option;
        initStyleOption(&myOption,index);
        QApplication::style()->drawItemText ( painter, myOption.rect , myOption.displayAlignment, QApplication::palette(), true,strTem );

        /*此段摘自QT源码*/
 //        QStyleOptionViewItem MyOption = option;

//        initStyleOption(&MyOption,index);
//        // MyOption. = strTem;
//         //const QWidget *widget = option.widget;
//         QStyle *style = widget ? widget->style() : QApplication::style();
//        style()->drawControl(QStyle::CE_ItemViewItem, &MyOption, painter);
    }
    else
    {
        QStyledItemDelegate::paint(painter,option,index);
    }
}


bool ItemDelegate::initEditor(QWidget *editor,const QModelIndex &index)
{
    /* 需要输入检测*/
    if(!index.isValid())
    {
        qDebug() << "temDelegate::initCheck  err:!index.isValid()";
        return false;
    }
    if(editor->metaObject()->className() == QLineEdit::staticMetaObject.className())
    {
        QLineEdit *LineEdit  = static_cast<QLineEdit*>(editor);
        int ThisCheck = index.data(CheckTypeRole).toInt(); // 获取数据检测类型
        // 如果有定义正则不在做其它限制条件
        if(index.data(RegexpRole).isValid())
        {
            QRegExp reg(index.data(RegexpRole).toString());//
            QValidator *validator=new QRegExpValidator(reg,LineEdit);
            LineEdit->setValidator(validator);
            return true;
        }

        if(ThisCheck == Int)  // 整型
        {
            QIntValidator *IntValidator = new QIntValidator(LineEdit);
            // 下限
            if(index.data(RangMinRole).isValid())
            {
                IntValidator->setBottom(index.data(RangMinRole).toInt());
            }
            // 上限
            if(index.data(RangMaxRole).isValid())
            {
                IntValidator->setTop(index.data(RangMaxRole).toInt());
            }
            LineEdit->setValidator(IntValidator);
        }
        else if(ThisCheck == Float)  // 浮点
        {
            MyDoubleValidator*  DoubleValidator = new MyDoubleValidator(LineEdit);
            DoubleValidator->setNotation(QDoubleValidator::StandardNotation);
            // 下限
            if(index.data(RangMinRole).isValid())
            {
                DoubleValidator->setBottom(index.data(RangMinRole).toDouble());
            }
            // 上限
            if(index.data(RangMaxRole).isValid())
            {
                DoubleValidator->setTop(index.data(RangMaxRole).toDouble());
            }
            // 小数点数
            if(index.data(DecimalRole).isValid())
            {
                DoubleValidator->setDecimals(index.data(DecimalRole).toInt());
            }
            else
            {
                DoubleValidator->setDecimals(0);    // 默认两个小数点
            }
            LineEdit->setValidator(DoubleValidator);    //
        }
        else if(ThisCheck == String || ThisCheck == DefaultShow)  // 字符
        {
            int lenMin = 0;
            int lenMax = ITEM_TYPE_STRING_MAXLEN;  // 默认上限
            QString str = index.data(VarNameRole).toString();
            // 字符个数下限
            if(index.data(RangMinRole).isValid())
            {
                lenMin = index.data(RangMinRole).toInt();
            }
            // 字符个数上限
            if(index.data(RangMaxRole).isValid())
            {
                lenMax = index.data(RangMaxRole).toInt();
            }
            QString regStr = QString("^.{%1,%2}$").arg(lenMin).arg(lenMax);
            QRegExp reg(regStr);
            QValidator *validator=new QRegExpValidator(reg,LineEdit);
            LineEdit->setValidator(validator);
        }
        else if(ThisCheck == Ip4)  // 网络IP地址
        {
            QRegExp reg(ITEM_BASE_TYPE_IP4_REGEXP);//
            QValidator *validator=new QRegExpValidator(reg,LineEdit);
            LineEdit->setValidator(validator);
        }
        else if(ThisCheck == MAC)  // 网络MAC地址
        {
            QRegExp reg(ITEM_BASE_TYPE_MAC_REGEXP);//
            QValidator *validator=new QRegExpValidator(reg,LineEdit);
            LineEdit->setValidator(validator);
        }

    }
    else if(editor->metaObject()->className() == QComboBox::staticMetaObject.className())
    {
        // 初始化该下拉框控件
        QComboBox *ComboEdit = static_cast<QComboBox*>(editor);
        if(index.data(ComboMapRole).isValid())
        {
            QMap<QString,QVariant> ComboMap = index.data(ComboMapRole).toMap();
            QMap<QString, QVariant>::const_iterator i = ComboMap.constBegin();
            while (i != ComboMap.constEnd()) {
                ComboEdit->addItem(i.key(),i.value());
                //qDebug() << i.key() << i.value();
                i++;
            }
        }
    }
    else if(editor->metaObject()->className() == QDoubleSpinBox::staticMetaObject.className())
    {
        // 初始化微调框控件

        QDoubleSpinBox *SpinBox = static_cast<QDoubleSpinBox*>(editor);
        // 下限
        if(index.data(RangMinRole).isValid())
        {
            SpinBox->setMinimum(index.data(RangMinRole).toDouble());
        }
        // 上限
        if(index.data(RangMaxRole).isValid())
        {
            SpinBox->setMaximum(index.data(RangMaxRole).toDouble());
        }
        // 步进
        if(index.data(StepRole).isValid())
        {
            SpinBox->setSingleStep(index.data(StepRole).toDouble());
        }
        // 单位
        if(index.data(UnitRole).isValid())
        {
            SpinBox->setSuffix(index.data(UnitRole).toString());
        }
        // 小数点数
        if(index.data(DecimalRole).isValid())
        {
            SpinBox->setDecimals(index.data(DecimalRole).toInt());
        }
        else
        {
            SpinBox->setDecimals(0);
        }

    }
    return true;
}

void ItemDelegate::viewDisp(bool view)
{
    m_isView = view;
}
