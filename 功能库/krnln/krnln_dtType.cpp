#ifndef __E_STATIC_LIB
#include "include_krnln_header.h"

// 系统核心支持库数据类型定义
// 自动生成于 2026-01-04 10:36:10
// 数据类型总数: 48

// 数据类型标志: LDT_WIN_UNIT=窗口组件, LDT_IS_CYCLEREF=支持循环引用

// 数据类型成员属性信息
// 数据类型信息数组
LIB_DATA_TYPE_INFO g_DataType_krnln_global_var[] = 
{
    {"窗口", "window", "本类型对象的方法被除开功能型外所有的窗口组件共享。", 0, NULL, 0, 0, 26, (PEVENT_INFO2)NULL, 0, NULL, NULL},
    {"菜单", "menu", "用作指定某一个菜单项目。", 0, NULL, 0, 0, 2, (PEVENT_INFO2)NULL, 0, NULL, NULL},
    {"字体", "font", "成员<1>的中文名称为“角度”，英文名称为“orient”，类型为“整数型（int）”。", 0, NULL, 0, 0, 0, (PEVENT_INFO2)NULL, 0, NULL, NULL},
    {"编辑框", "EditBox", "可供选择的属性值：", 0, NULL, 0, 0, 4, (PEVENT_INFO2)NULL, 0, NULL, NULL},
    {"图片框", "PicBox", "可供选择的属性值：", 0, NULL, 0, 0, 0, (PEVENT_INFO2)NULL, 0, NULL, NULL},
    {"外形框", "ShapeBox", "可供选择的属性值：", 0, NULL, 0, 0, 0, (PEVENT_INFO2)NULL, 0, NULL, NULL},
    {"画板", "DrawPanel", "可供选择的属性值：", 0, NULL, 0, 0, 2, (PEVENT_INFO2)NULL, 0, NULL, NULL},
    {"分组框", "GroupBox", "如果希望某一字符成为本分组框中第一个可停留焦点组件的访问键，可以在该字符前面加上一个 & 字符。", 0, NULL, 0, 0, 0, (PEVENT_INFO2)NULL, 0, NULL, NULL},
    {"标签", "Label", "如果希望某一字符成为本标签后可停留焦点组件的访问键，可以在该字符前面加上一个 & 字符。", 0, NULL, 0, 0, 2, (PEVENT_INFO2)NULL, 0, NULL, NULL},
    {"按钮", "Button", "本属性指定显示在按钮上的图片。", 0, NULL, 0, 0, 2, (PEVENT_INFO2)NULL, 0, NULL, NULL},
    {"选择框", "CheckBox", "本属性指定显示在选择框上的图片。", 0, NULL, 0, 0, 2, (PEVENT_INFO2)NULL, 0, NULL, NULL},
    {"单选框", "RadioBox", "本属性指定显示在单选框上的图片。", 0, NULL, 0, 0, 2, (PEVENT_INFO2)NULL, 0, NULL, NULL},
    {"组合框", "ComboBox", "注意: 修改本属性会丢失动态添加的项目(以原静态项目覆盖)。", 0, NULL, 0, 0, 10, (PEVENT_INFO2)NULL, 0, NULL, NULL},
    {"列表框", "ListBox", "可供选择的属性值：", 0, NULL, 0, 0, 4, (PEVENT_INFO2)NULL, 0, NULL, NULL},
    {"选择列表框", "ChkListBox", "可供选择的属性值：", 0, NULL, 0, 0, 6, (PEVENT_INFO2)NULL, 0, NULL, NULL},
    {"横向滚动条", "HScrollBar", "本属性指定当用户在滚动条的空隙处单击后滚动条位置的增减数值。", 0, NULL, 0, 0, 2, (PEVENT_INFO2)NULL, 0, NULL, NULL},
    {"纵向滚动条", "VScrollBar", "本属性指定当用户在滚动条的空隙处单击后滚动条位置的增减数值。", 0, NULL, 0, 0, 2, (PEVENT_INFO2)NULL, 0, NULL, NULL},
    {"进度条", "ProcessBar", "可供选择的属性值：", 0, NULL, 0, 0, 0, (PEVENT_INFO2)NULL, 0, NULL, NULL},
    {"滑块条", "SliderBar", "可供选择的属性值：", 0, NULL, 0, 0, 2, (PEVENT_INFO2)NULL, 0, NULL, NULL},
    {"选择夹", "Tab", "可供选择的属性值：", 0, NULL, 0, 0, 6, (PEVENT_INFO2)NULL, 0, NULL, NULL},
    {"影像框", "AnimateBox", "本属性指定欲播放影像文件的名称。", 0, NULL, 0, 0, 0, (PEVENT_INFO2)NULL, 0, NULL, NULL},
    {"日期框", "DatePicker", "可供选择的属性值：", 0, NULL, 0, 0, 2, (PEVENT_INFO2)NULL, 0, NULL, NULL},
    {"月历", "MonthCalendar", "可供选择的属性值：", 0, NULL, 0, 0, 2, (PEVENT_INFO2)NULL, 0, NULL, NULL},
    {"驱动器框", "DriverBox", "本属性在设计时不可用，在运行时可读可写。", 0, NULL, 0, 0, 2, (PEVENT_INFO2)NULL, 0, NULL, NULL},
    {"目录框", "DirBox", "本属性在设计时不可用，在运行时可读可写。", 0, NULL, 0, 0, 2, (PEVENT_INFO2)NULL, 0, NULL, NULL},
    {"文件框", "FileBox", "本属性在设计时不可用，在运行时可读可写。", 0, NULL, 0, 0, 4, (PEVENT_INFO2)NULL, 0, NULL, NULL},
    {"颜色选择器", "ColorPicker", "颜色数值为红绿蓝颜色值或“#透明”常量值。", 0, NULL, 0, 0, 2, (PEVENT_INFO2)NULL, 0, NULL, NULL},
    {"超级链接框", "HyperLinker", "可供选择的属性值：", 0, NULL, 0, 0, 0, (PEVENT_INFO2)NULL, 0, NULL, NULL},
    {"调节器", "Spin", "可供选择的属性值：", 0, NULL, 0, 0, 2, (PEVENT_INFO2)NULL, 0, NULL, NULL},
    {"通用对话框", "CommonDlg", "可供选择的属性值：", 0, NULL, LDT_WIN_UNIT, 0, 0, (PEVENT_INFO2)NULL, 0, NULL, NULL},
    {"时钟", "Timer", "指定时钟事件产生的周期，单位为毫秒。如果设定为0，则无时钟事件产生。", 0, NULL, LDT_WIN_UNIT, 0, 2, (PEVENT_INFO2)NULL, 0, NULL, NULL},
    {"打印机", "printer", "本属性指定绘画时所使用的单位，坐标系的 X 轴从左到右，Y 轴从上到下。", 0, NULL, LDT_WIN_UNIT, 0, 0, (PEVENT_INFO2)NULL, 0, NULL, NULL},
    {"字段信息", "FieldInf", "成员<1>的中文名称为“名称”，英文名称为“name”，类型为“文本型（text）”。", 0, NULL, 0, 0, 0, (PEVENT_INFO2)NULL, 0, NULL, NULL},
    {"数据报", "UDP", "数据报是一种不可靠、小数据量的网络数据交互方式。如果传递的数据量过大，有可能会失败，最好不要超过 127 字节。如需要大", 0, NULL, LDT_WIN_UNIT, 0, 2, (PEVENT_INFO2)NULL, 0, NULL, NULL},
    {"客户", "Client", "客户是基于连接的网络数据交换的客户方组件，用作向另一方的服务器端组件发送数据。基于连接的数据交换是一种可靠的、允许大数据", 0, NULL, LDT_WIN_UNIT, 0, 4, (PEVENT_INFO2)NULL, 0, NULL, NULL},
    {"服务器", "Server", "服务器是基于连接的网络数据交换的服务方组件，用作在指定端口监视来自客户方的数据。", 0, NULL, LDT_WIN_UNIT, 0, 6, (PEVENT_INFO2)NULL, 0, NULL, NULL},
    {"端口", "SerialPort", "端口组件提供对端口的直接读写操作。", 0, NULL, LDT_WIN_UNIT, 0, 4, (PEVENT_INFO2)NULL, 0, NULL, NULL},
    {"打印设置信息", "PrintInf", "成员<1>的中文名称为“纸张类型”，英文名称为“Paper”，类型为“整数型（int）”，初始值为“0”。", 0, NULL, 0, 0, 0, (PEVENT_INFO2)NULL, 0, NULL, NULL},
    {"表格", "Grid", "指定提供数据的数据源组件名。", 0, NULL, 0, 0, 12, (PEVENT_INFO2)NULL, 0, NULL, NULL},
    {"数据源", "DataSrc", "数据源提供对各种来源数据的统一操作接口。数据源组件各个按钮的含义分别是：到首记录、到上一记录、到下一记录、到尾记录、添加", 0, NULL, 0, 0, 6, (PEVENT_INFO2)NULL, 0, NULL, NULL},
    {"通用提供者", "NProvider", "通用提供者用作为数据源提供对数据的通常存取操作。使用内存作为数据的存储仓库，全面支持所有数据操作接口。因此必要时可以将其", 0, NULL, LDT_WIN_UNIT, 0, 0, (PEVENT_INFO2)NULL, 0, NULL, NULL},
    {"数据库提供者", "DBProvider", "数据库提供者用作为数据源提供对数据库的存取操作。它使用数据库作为数据的存储仓库，不支持以下数据操作接口：", 0, NULL, LDT_WIN_UNIT, 0, 0, (PEVENT_INFO2)NULL, 0, NULL, NULL},
    {"图形按钮", "PicBtn", "可供选择的属性值：", 0, NULL, 0, 0, 2, (PEVENT_INFO2)NULL, 0, NULL, NULL},
    {"外部数据库", "ODBCDB", "本组件提供通过ODBC对外部数据库直接操作的方法。", 0, NULL, LDT_WIN_UNIT, 0, 0, (PEVENT_INFO2)NULL, 0, NULL, NULL},
    {"外部数据提供者", "ODBCProvider", "外部数据提供者用作为数据源提供对外部ODBC数据库的存取操作。它使用外部数据库作为数据的存储仓库，不支持备注和长字节集类", 0, NULL, LDT_WIN_UNIT, 0, 0, (PEVENT_INFO2)NULL, 0, NULL, NULL},
    {"对象", "ComObject", "提供对微软公司 COM 对象的支持，注意对象在超出其作用域后会自动被释放，例如：假设对象存在于子程序局部变量中，当子程序", 0, NULL, 0, 0, 0, (PEVENT_INFO2)NULL, 0, NULL, NULL},
    {"变体型", "Variant", "提供对COM接口中Variant数据类型(即VB中的Any数据类型)的支持，它可以容纳文本、数值、逻辑值、日期时间值、C", 0, NULL, 0, 0, 0, (PEVENT_INFO2)NULL, 0, NULL, NULL},
    {"变体类型", "VariantType", "提供变体型中所能够容纳数据类型的枚举值。", 0, NULL, 0, 0, 0, (PEVENT_INFO2)NULL, 0, NULL, NULL},
};

int g_DataType_krnln_global_var_count = 48;

#endif
