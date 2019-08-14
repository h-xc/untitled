/************************************************************
Copyright (C), 2018-2018, NZ, Ltd.
FileName: item_delegate.h
Author:   hxc
Version : v1.0
Date:     190102
Description:
1、此处大部分代码是针对表格单元格编辑器创建，输入检测等，因为表格的编辑器
（控件）是在双击后创建的，一些输入限制，需要先记录在模型中，创建编辑器后再设置。
2、控件映射也可使用。
Function List:
1. -------
History:
<author> <time> <version > <desc>
hxc      190102 v1.0       创建编写
***********************************************************/
#ifndef MYDELEGATE_H
#define MYDELEGATE_H

#include <QValidator>
#include <QStyledItemDelegate>
#include <QMap>


#define ITEM_TYPE_STRING_MAXLEN      10     // 定义字符默认最大长度
// 网址IP4类型 正则
#define ITEM_BASE_TYPE_IP4_REGEXP    "((2[0-4]\\d|25[0-5]|[01]?\\d\\d?)\\.){3}(2[0-4]\\d|25[0-5]|[01]?\\d\\d?)"
// MAC地址类型 正则
#define ITEM_BASE_TYPE_MAC_REGEXP    "(([0-9A-F]{2}:){5}[0-9A-F]{2})"

#define ADDR_POS  0

/*编辑器类型（表格视图使用）*/
enum EditType
{
    DefaultEdit,                    // 默认
    ComboBox,                       // 下拉框
    LineEdit,                       // 文本输入框
    CheckBox,                       // 复选框
};

/*数据角色*/
enum ItemUserRole
{
    /*通用属性储存角色*/
    VarNameRole = Qt::UserRole,        // 值变量名称
    NameRole,                       // 值名称（中文）
    UnitRole,                       // 单位
    DescRole,                       // 值描述
    ShowTypeRole,                   // 显示类型，定义16进制，标准浮点，字符
    CheckTypeRole,                  // 输入检测类型
    DataTypeRole,                   // 数据类型，值采用QT自带的MetaType，用于标记内存变量类型
    EditTypeRole,                   // 编辑器类型 可定义单元格双击后弹出的编辑框
    RangMinRole,                    // 标记输入上下限
    RangMaxRole,                    // 标记输入上下限
    StepRole,                       // 步进数（如步进为2，则值为2的倍数）
    RegexpRole,                     // 正则，启用后（只针对文本输入框，可设置限制某个字符不能输入，如空格）

    ContentRole,                    // 用于目录标签的显示

    /*辅助属性储存角色*/
    DecimalRole,                    // 小数点位数
    ComboMapRole,                   // 控件数据填充源,针对多项选择的控件，如下拉框，
    TempRole,                       // 临时存储数据
    StoreCtrlRole,                  // 储存操作，内存数据和模型数据的提交方向
};

enum StoreCtrl
{
    notCtl,
    _SUBMIT,    // 模型数据提交到内存
    _REVERT,    // 内存数据提交到模型
};


/*输入检测类型  主要针对文本框*/
enum CheckType
{
    String,                         // 输入字符   可以定义长度
    Ip4,                            // 只能自定义IP4
    MAC,                            // 只能输入MAC地址
    Int,                            // 只能输入整型数据
    Float,                          // 只能输入浮点数据
};

/*显示类型 输入检测会使用*/
enum ShowType
{
    DefaultShow,                    // 直接显示（字符）
    Bit,                            // 二进制显示
    Dec,                            // 十进制数值
    Hex,                            // 16进制数值
    //未来可以添加不同显示
};



class ItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    /*
     * 构造函数ItemDelegate
     * parent：   输入，父窗口
     * isView    输入，用于标记当前是表格视图，还是小控件 false:小控件   true：view视图
     */
    explicit ItemDelegate(QObject *parent = 0);
    ~ItemDelegate();
    /*
     * 虚函数createEditor，函数会在表格视图被双击后调用,创建编辑器（注意：只有使用了表格视图且双击单元格才会被调用到）
     * parent：   输入，父窗口
     * option：   输入，绘制参数，用于绘制自定义编辑器小部件，（包含大小、颜色、字体等内容）
     * index：    输入，项索引
     */
    QWidget * createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;

    /*
     * 虚函数setEditorData，将模型中的数据传输给控件进行显示
     * editor：    输入，如果是表格，则是createEditor创建的控件，否则是小控件，
     */
    void setEditorData(QWidget *editor, const QModelIndex &index) const;

    /*虚函数sizeHint，用于调整单元格大小的，未使用*/
    //QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;

    /*
     * 虚函数，setModelData，将控件编辑内容传输回单元格模型
     * editor  输入，表格视图时，是createEditor创建的窗口类
     * model   输入，模型类
     * index   输入，项索引
     */
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;

    /*虚函数paint，在表格视图中会使用到，绘制显示小部件*/
    void paint(QPainter *painter,const QStyleOptionViewItem &option, const QModelIndex &index) const;


    /*静态函数initCheck，用于初始化表格编辑器和小控件的输入合法性的初始化*/
    static bool initEditor(QWidget *editor,const QModelIndex &index);   //

    void viewDisp(bool view);
private:

    /* 因为部分表格和小控件的处理方式（部分控件的内容获取和初始化）不同，需要标记是否是表格的委托  0：不是  1：是  默认 1*/
    bool m_isView;

    //signals:

    //public slots:
};

#endif // MYDELEGATE_H
