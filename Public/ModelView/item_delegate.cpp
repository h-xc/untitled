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
#include <QTableView>
#include <QCheckBox>
#include <QNetworkInterface>

#include "mydoublevalidator.h"
#include "mylineedit.h"

ItemDelegate::ItemDelegate(QObject *parent) : QStyledItemDelegate(parent)
{

}

ItemDelegate::~ItemDelegate()
{

}

QWidget * ItemDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    int thisEdit = index.data(EditTypeRole).toInt();  // 获取编辑器类型
    if(thisEdit == CheckBox)// 复选框编辑器
    {
        QCheckBox *CheckBox = new QCheckBox(parent);                // 调用完后会自动释放，不用担心内存泄漏
        CheckBox->setText(index.data(ItemDelegate::NameRole).toString());
        initEditor(CheckBox,index);// 初始化输入检测
        return CheckBox;
    }
    else if(thisEdit == ComboBox)                                   // 下拉框编辑器
    {
        QComboBox *ComboEdit = new QComboBox(parent);               // 调用完后会自动释放，不用担心内存泄漏
        initEditor(ComboEdit,index);// 初始化输入检测
        return ComboEdit;  //
    }
    else if(thisEdit == LineEdit || thisEdit ==  DefaultEdit)  // 文本输入框
    {
        // 使用自定义的行编辑框
        QLineEdit *LineEdit = new MyLineEdit(parent);
        initEditor(LineEdit,index);// 初始化输入检测
        return LineEdit;
    }
    else if(thisEdit == ItemDelegate::SpinBox)
    {
        if(index.data(ItemDelegate::CheckTypeRole).toInt() == ItemDelegate::Float
                || index.data(ItemDelegate::CheckTypeRole).toInt() == ItemDelegate::Int)
        {
            QAbstractSpinBox *spinBox = new QDoubleSpinBox(parent);
            initEditor(spinBox,index);// 初始化输入检测
            return spinBox;
        }
    }
    else
    {
        return QStyledItemDelegate::createEditor(parent,option,index);  // 若未定义类型，则调用父类原方法
    }
    return NULL;
}


void ItemDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    /* 编辑器的内容初始化在此定义*/
    editor->setProperty("isChanged",false);
    if(editor->metaObject()->className() == QComboBox::staticMetaObject.className())  // 下拉框编辑器
    {
        QComboBox *comboBox = static_cast<QComboBox*>(editor);               // 获取下拉框编辑器
        int comboIndex = comboBox->findData(index.model()->data(index,Qt::EditRole));//获取模型数据在下拉框编辑器中的位置
        comboBox->setCurrentIndex(comboIndex);                              // 将下列框的选中项更新为模型数据指定的位置
        return;
    }
    else if(editor->metaObject()->className() == QLineEdit::staticMetaObject.className()) // 文本框编辑器
    {
        QLineEdit *lineEdit = static_cast<QLineEdit*>(editor);               //
        lineEdit->setText(index.data(Qt::EditRole).toString());
        return;
    }
    QStyledItemDelegate::setEditorData(editor,index);
}

void ItemDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    if(editor->metaObject()->className() == QComboBox::staticMetaObject.className())
    {
        QComboBox *comboBox = static_cast<QComboBox*>(editor);
        model->setData(index, comboBox->itemData(comboBox->currentIndex()), Qt::EditRole);
        return;
    }
    if(editor->metaObject()->className() == QLineEdit::staticMetaObject.className())
    {
        QStyledItemDelegate::setModelData(editor,model,index);
        return;
    }
    if(editor->metaObject()->className() == QDoubleSpinBox::staticMetaObject.className())
    {
        QDoubleSpinBox *spinBox =  static_cast<QDoubleSpinBox*>(editor);
        model->setData(index, spinBox->cleanText(), Qt::EditRole);
        return;
    }
    QStyledItemDelegate::setModelData(editor,model,index);
}

void ItemDelegate::paint(QPainter *painter,const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    int thisEdit = index.data(EditTypeRole).toInt();  // 获取编辑器类型
    if(thisEdit == ComboBox)                          // 为了让表格显示下拉框的选中的显示文本内容，而不是显示数字值
    {
        QString strTem;
        QMap<QString,QVariant> ComboMap = index.data(ComboMapRole).toMap();

        strTem = ComboMap.key(index.data(Qt::EditRole));
        if(strTem.isEmpty())return;

        QStyleOptionViewItem opt = option;
        initStyleOption(&opt, index);
        opt.text = strTem;
        const QWidget *widget = option.widget;
        QStyle *style = widget ? widget->style() : QApplication::style();
        style->drawControl(QStyle::CE_ItemViewItem, &opt, painter, widget);
    }
    else if(thisEdit == CheckBox)
    {
        QStyledItemDelegate::paint(painter,option,index);
    }
    else
    {
        QStyledItemDelegate::paint(painter,option,index);
    }
}

bool ItemDelegate::eventFilter(QObject *editor, QEvent *event)
{
    if(editor->metaObject()->className() == QComboBox::staticMetaObject.className())
    {
        if(event->type() ==QEvent::Wheel)//屏蔽控件的鼠标滚轮事件
        {
            return true;
        }
    }
    return QStyledItemDelegate::eventFilter(editor, event);
}


bool ItemDelegate::initEditor(QWidget *editor,const QModelIndex &index)
{
    /* 需要输入检测*/
    if(!index.isValid())
    {
        qDebug() << "ItemDelegate::initEditor  err:!index.isValid()";
        return false;
    }

    if(index.data(DescRole).isValid())
    {
        editor->setToolTip(index.data(DescRole).toString());
    }

    if(editor->metaObject()->className() == QLineEdit::staticMetaObject.className())
    {
        QLineEdit *lineEdit  = static_cast<QLineEdit*>(editor);
        QValidator *validator = createValidator(index);
        validator->setParent(lineEdit);
        lineEdit->setValidator(validator);
    }
    else if(editor->metaObject()->className() == QComboBox::staticMetaObject.className())
    {
        QComboBox *ComboEdit = static_cast<QComboBox*>(editor);
        if(index.data(ComboMapRole).isValid())
        {
            QMap<QString,QVariant> ComboMap = index.data(ComboMapRole).toMap();
            QMap<QString, QVariant>::const_iterator i = ComboMap.constBegin();
            while (i != ComboMap.constEnd()) {
                ComboEdit->addItem(i.key(),i.value());
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

QValidator* ItemDelegate::createValidator(const QModelIndex &index)
{
    // 如果有定义正则不再做其它限制条件
    if(index.data(RegexpRole).isValid())
    {
        QRegExp reg(index.data(RegexpRole).toString());//
        return new QRegExpValidator(reg);
    }

    int ThisCheck = index.data(CheckTypeRole).toInt(); // 获取数据检测类型
    if(ThisCheck == Int)  // 整型
    {
        QIntValidator *IntValidator = new QIntValidator();
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
        return IntValidator;
    }
    else if(ThisCheck == Float)  // 浮点
    {
        MyDoubleValidator*  DoubleValidator = new MyDoubleValidator();
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
        return DoubleValidator;
    }
    else if(ThisCheck == String || ThisCheck == DefaultShow)  // 字符
    {
        int lenMin = 0;
        int lenMax = ITEM_TYPE_STRING_MAXLEN;  // 默认上限
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
        return new QRegExpValidator(reg);
    }
    else if(ThisCheck == Ip4)  // 网络IP地址
    {
        QRegExp reg(ITEM_BASE_TYPE_IP4_REGEXP);//
        return new QRegExpValidator(reg);
    }
    else if(ThisCheck == MAC)  // 网络MAC地址
    {
        QRegExp reg(ITEM_BASE_TYPE_MAC_REGEXP);//
        return new QRegExpValidator(reg);
    }
    return NULL;
}





