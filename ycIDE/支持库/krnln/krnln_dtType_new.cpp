/*******************************************************************************
 * 文件名: krnln_dtType.cpp
 * 描述: 系统核心支持库数据类型定义 (增强版，包含控件接口)
 * 作者: ycIDE
 * 创建时间: 2026-01-07
 * 数据类型总数: 48
 ******************************************************************************/

#ifndef __E_STATIC_LIB
#include "include_krnln_header.h"
#include "control/unit_interface.h"

// 数据类型标志说明:
// LDT_WIN_UNIT         = 窗口组件
// LDT_IS_CONTAINER     = 容器组件
// LDT_IS_TAB_UNIT      = TAB控件
// LDT_IS_FUNCTION_PROVIDER = 功能型组件(不可见)
// LDT_CANNOT_GET_FOCUS = 不能获取焦点

// 数据类型信息数组
LIB_DATA_TYPE_INFO g_DataType_krnln_global_var[] = 
{
    // 索引0: 窗口 - 容器组件
    {"窗口", "window", "本类型对象的方法被除开功能型外所有的窗口组件共享。", 
        0, NULL, LDT_WIN_UNIT | LDT_IS_CONTAINER, 0, 26, (PEVENT_INFO2)NULL, 0, NULL, Window_GetInterface},
    
    // 索引1: 菜单
    {"菜单", "menu", "用作指定某一个菜单项目。", 
        0, NULL, 0, 0, 2, (PEVENT_INFO2)NULL, 0, NULL, NULL},
    
    // 索引2: 字体
    {"字体", "font", "成员<1>的中文名称为\"角度\"，英文名称为\"orient\"，类型为\"整数型（int）\"。", 
        0, NULL, 0, 0, 0, (PEVENT_INFO2)NULL, 0, NULL, NULL},
    
    // 索引3: 编辑框
    {"编辑框", "EditBox", "可供选择的属性值：", 
        0, NULL, LDT_WIN_UNIT, 0, 4, (PEVENT_INFO2)NULL, 0, NULL, EditBox_GetInterface},
    
    // 索引4: 图片框
    {"图片框", "PicBox", "可供选择的属性值：", 
        0, NULL, LDT_WIN_UNIT, 0, 0, (PEVENT_INFO2)NULL, 0, NULL, NullUnit_GetInterface},
    
    // 索引5: 外形框
    {"外形框", "ShapeBox", "可供选择的属性值：", 
        0, NULL, LDT_WIN_UNIT | LDT_CANNOT_GET_FOCUS, 0, 0, (PEVENT_INFO2)NULL, 0, NULL, NullUnit_GetInterface},
    
    // 索引6: 画板
    {"画板", "DrawPanel", "可供选择的属性值：", 
        0, NULL, LDT_WIN_UNIT, 0, 2, (PEVENT_INFO2)NULL, 0, NULL, NullUnit_GetInterface},
    
    // 索引7: 分组框
    {"分组框", "GroupBox", "如果希望某一字符成为本分组框中第一个可停留焦点组件的访问键，可以在该字符前面加上一个 & 字符。", 
        0, NULL, LDT_WIN_UNIT | LDT_IS_CONTAINER, 0, 0, (PEVENT_INFO2)NULL, 0, NULL, NullUnit_GetInterface},
    
    // 索引8: 标签
    {"标签", "Label", "如果希望某一字符成为本标签后可停留焦点组件的访问键，可以在该字符前面加上一个 & 字符。", 
        0, NULL, LDT_WIN_UNIT | LDT_CANNOT_GET_FOCUS, 0, 2, (PEVENT_INFO2)NULL, 0, NULL, Label_GetInterface},
    
    // 索引9: 按钮
    {"按钮", "Button", "本属性指定显示在按钮上的图片。", 
        0, NULL, LDT_WIN_UNIT, 0, 2, (PEVENT_INFO2)NULL, 0, NULL, Button_GetInterface},
    
    // 索引10: 选择框
    {"选择框", "CheckBox", "本属性指定显示在选择框上的图片。", 
        0, NULL, LDT_WIN_UNIT, 0, 2, (PEVENT_INFO2)NULL, 0, NULL, NullUnit_GetInterface},
    
    // 索引11: 单选框
    {"单选框", "RadioBox", "本属性指定显示在单选框上的图片。", 
        0, NULL, LDT_WIN_UNIT, 0, 2, (PEVENT_INFO2)NULL, 0, NULL, NullUnit_GetInterface},
    
    // 索引12: 组合框
    {"组合框", "ComboBox", "注意: 修改本属性会丢失动态添加的项目(以原静态项目覆盖)。", 
        0, NULL, LDT_WIN_UNIT, 0, 10, (PEVENT_INFO2)NULL, 0, NULL, NullUnit_GetInterface},
    
    // 索引13: 列表框
    {"列表框", "ListBox", "可供选择的属性值：", 
        0, NULL, LDT_WIN_UNIT, 0, 4, (PEVENT_INFO2)NULL, 0, NULL, NullUnit_GetInterface},
    
    // 索引14: 选择列表框
    {"选择列表框", "ChkListBox", "可供选择的属性值：", 
        0, NULL, LDT_WIN_UNIT, 0, 6, (PEVENT_INFO2)NULL, 0, NULL, NullUnit_GetInterface},
    
    // 索引15: 横向滚动条
    {"横向滚动条", "HScrollBar", "本属性指定当用户在滚动条的空隙处单击后滚动条位置的增减数值。", 
        0, NULL, LDT_WIN_UNIT, 0, 2, (PEVENT_INFO2)NULL, 0, NULL, NullUnit_GetInterface},
    
    // 索引16: 纵向滚动条
    {"纵向滚动条", "VScrollBar", "本属性指定当用户在滚动条的空隙处单击后滚动条位置的增减数值。", 
        0, NULL, LDT_WIN_UNIT, 0, 2, (PEVENT_INFO2)NULL, 0, NULL, NullUnit_GetInterface},
    
    // 索引17: 进度条
    {"进度条", "ProcessBar", "可供选择的属性值：", 
        0, NULL, LDT_WIN_UNIT | LDT_CANNOT_GET_FOCUS, 0, 0, (PEVENT_INFO2)NULL, 0, NULL, NullUnit_GetInterface},
    
    // 索引18: 滑块条
    {"滑块条", "SliderBar", "可供选择的属性值：", 
        0, NULL, LDT_WIN_UNIT, 0, 2, (PEVENT_INFO2)NULL, 0, NULL, NullUnit_GetInterface},
    
    // 索引19: 选择夹
    {"选择夹", "Tab", "可供选择的属性值：", 
        0, NULL, LDT_WIN_UNIT | LDT_IS_TAB_UNIT | LDT_IS_CONTAINER, 0, 6, (PEVENT_INFO2)NULL, 0, NULL, NullUnit_GetInterface},
    
    // 索引20: 影像框
    {"影像框", "AnimateBox", "本属性指定欲播放影像文件的名称。", 
        0, NULL, LDT_WIN_UNIT, 0, 0, (PEVENT_INFO2)NULL, 0, NULL, NullUnit_GetInterface},
    
    // 索引21: 日期框
    {"日期框", "DatePicker", "可供选择的属性值：", 
        0, NULL, LDT_WIN_UNIT, 0, 2, (PEVENT_INFO2)NULL, 0, NULL, NullUnit_GetInterface},
    
    // 索引22: 月历
    {"月历", "MonthCalendar", "可供选择的属性值：", 
        0, NULL, LDT_WIN_UNIT, 0, 2, (PEVENT_INFO2)NULL, 0, NULL, NullUnit_GetInterface},
    
    // 索引23: 驱动器框
    {"驱动器框", "DriverBox", "本属性在设计时不可用，在运行时可读可写。", 
        0, NULL, LDT_WIN_UNIT, 0, 2, (PEVENT_INFO2)NULL, 0, NULL, NullUnit_GetInterface},
    
    // 索引24: 目录框
    {"目录框", "DirBox", "本属性在设计时不可用，在运行时可读可写。", 
        0, NULL, LDT_WIN_UNIT, 0, 2, (PEVENT_INFO2)NULL, 0, NULL, NullUnit_GetInterface},
    
    // 索引25: 文件框
    {"文件框", "FileBox", "本属性在设计时不可用，在运行时可读可写。", 
        0, NULL, LDT_WIN_UNIT, 0, 4, (PEVENT_INFO2)NULL, 0, NULL, NullUnit_GetInterface},
    
    // 索引26: 颜色选择器
    {"颜色选择器", "ColorPicker", "颜色数值为红绿蓝颜色值或\"#透明\"常量值。", 
        0, NULL, LDT_WIN_UNIT, 0, 2, (PEVENT_INFO2)NULL, 0, NULL, NullUnit_GetInterface},
    
    // 索引27: 超级链接框
    {"超级链接框", "HyperLinker", "可供选择的属性值：", 
        0, NULL, LDT_WIN_UNIT, 0, 0, (PEVENT_INFO2)NULL, 0, NULL, NullUnit_GetInterface},
    
    // 索引28: 调节器
    {"调节器", "Spin", "可供选择的属性值：", 
        0, NULL, LDT_WIN_UNIT, 0, 2, (PEVENT_INFO2)NULL, 0, NULL, NullUnit_GetInterface},
    
    // 索引29: 通用对话框 - 功能型
    {"通用对话框", "CommonDlg", "可供选择的属性值：", 
        0, NULL, LDT_WIN_UNIT | LDT_IS_FUNCTION_PROVIDER, 0, 0, (PEVENT_INFO2)NULL, 0, NULL, NullUnit_GetInterface},
    
    // 索引30: 时钟 - 功能型
    {"时钟", "Timer", "指定时钟事件产生的周期，单位为毫秒。如果设定为0，则无时钟事件产生。", 
        0, NULL, LDT_WIN_UNIT | LDT_IS_FUNCTION_PROVIDER, 0, 2, (PEVENT_INFO2)NULL, 0, NULL, NullUnit_GetInterface},
    
    // 索引31: 打印机 - 功能型
    {"打印机", "printer", "本属性指定绘画时所使用的单位，坐标系的 X 轴从左到右，Y 轴从上到下。", 
        0, NULL, LDT_WIN_UNIT | LDT_IS_FUNCTION_PROVIDER, 0, 0, (PEVENT_INFO2)NULL, 0, NULL, NullUnit_GetInterface},
    
    // 索引32: 字段信息 - 非窗口组件
    {"字段信息", "FieldInf", "成员<1>的中文名称为\"名称\"，英文名称为\"name\"，类型为\"文本型（text）\"。", 
        0, NULL, 0, 0, 0, (PEVENT_INFO2)NULL, 0, NULL, NULL},
    
    // 索引33: 数据报 - 功能型
    {"数据报", "UDP", "数据报是一种不可靠、小数据量的网络数据交互方式。如果传递的数据量过大，有可能会失败，最好不要超过 127 字节。", 
        0, NULL, LDT_WIN_UNIT | LDT_IS_FUNCTION_PROVIDER, 0, 2, (PEVENT_INFO2)NULL, 0, NULL, NullUnit_GetInterface},
    
    // 索引34: 客户 - 功能型
    {"客户", "Client", "客户是基于连接的网络数据交换的客户方组件，用作向另一方的服务器端组件发送数据。", 
        0, NULL, LDT_WIN_UNIT | LDT_IS_FUNCTION_PROVIDER, 0, 4, (PEVENT_INFO2)NULL, 0, NULL, NullUnit_GetInterface},
    
    // 索引35: 服务器 - 功能型
    {"服务器", "Server", "服务器是基于连接的网络数据交换的服务方组件，用作在指定端口监视来自客户方的数据。", 
        0, NULL, LDT_WIN_UNIT | LDT_IS_FUNCTION_PROVIDER, 0, 6, (PEVENT_INFO2)NULL, 0, NULL, NullUnit_GetInterface},
    
    // 索引36: 端口 - 功能型
    {"端口", "SerialPort", "端口组件提供对端口的直接读写操作。", 
        0, NULL, LDT_WIN_UNIT | LDT_IS_FUNCTION_PROVIDER, 0, 4, (PEVENT_INFO2)NULL, 0, NULL, NullUnit_GetInterface},
    
    // 索引37: 打印设置信息 - 非窗口组件
    {"打印设置信息", "PrintInf", "成员<1>的中文名称为\"纸张类型\"，英文名称为\"Paper\"，类型为\"整数型（int）\"。", 
        0, NULL, 0, 0, 0, (PEVENT_INFO2)NULL, 0, NULL, NULL},
    
    // 索引38: 表格
    {"表格", "Grid", "指定提供数据的数据源组件名。", 
        0, NULL, LDT_WIN_UNIT, 0, 12, (PEVENT_INFO2)NULL, 0, NULL, NullUnit_GetInterface},
    
    // 索引39: 数据源
    {"数据源", "DataSrc", "数据源提供对各种来源数据的统一操作接口。", 
        0, NULL, LDT_WIN_UNIT, 0, 6, (PEVENT_INFO2)NULL, 0, NULL, NullUnit_GetInterface},
    
    // 索引40: 通用提供者 - 功能型
    {"通用提供者", "NProvider", "通用提供者用作为数据源提供对数据的通常存取操作。使用内存作为数据的存储仓库。", 
        0, NULL, LDT_WIN_UNIT | LDT_IS_FUNCTION_PROVIDER, 0, 0, (PEVENT_INFO2)NULL, 0, NULL, NullUnit_GetInterface},
    
    // 索引41: 数据库提供者 - 功能型
    {"数据库提供者", "DBProvider", "数据库提供者用作为数据源提供对数据库的存取操作。", 
        0, NULL, LDT_WIN_UNIT | LDT_IS_FUNCTION_PROVIDER, 0, 0, (PEVENT_INFO2)NULL, 0, NULL, NullUnit_GetInterface},
    
    // 索引42: 图形按钮
    {"图形按钮", "PicBtn", "可供选择的属性值：", 
        0, NULL, LDT_WIN_UNIT, 0, 2, (PEVENT_INFO2)NULL, 0, NULL, NullUnit_GetInterface},
    
    // 索引43: 外部数据库 - 功能型
    {"外部数据库", "ODBCDB", "本组件提供通过ODBC对外部数据库直接操作的方法。", 
        0, NULL, LDT_WIN_UNIT | LDT_IS_FUNCTION_PROVIDER, 0, 0, (PEVENT_INFO2)NULL, 0, NULL, NullUnit_GetInterface},
    
    // 索引44: 外部数据提供者 - 功能型
    {"外部数据提供者", "ODBCProvider", "外部数据提供者用作为数据源提供对外部ODBC数据库的存取操作。", 
        0, NULL, LDT_WIN_UNIT | LDT_IS_FUNCTION_PROVIDER, 0, 0, (PEVENT_INFO2)NULL, 0, NULL, NullUnit_GetInterface},
    
    // 索引45: 对象 - 非窗口组件
    {"对象", "ComObject", "提供对微软公司 COM 对象的支持。", 
        0, NULL, 0, 0, 0, (PEVENT_INFO2)NULL, 0, NULL, NULL},
    
    // 索引46: 变体型 - 非窗口组件
    {"变体型", "Variant", "提供对COM接口中Variant数据类型(即VB中的Any数据类型)的支持。", 
        0, NULL, 0, 0, 0, (PEVENT_INFO2)NULL, 0, NULL, NULL},
    
    // 索引47: 变体类型 - 非窗口组件
    {"变体类型", "VariantType", "提供变体型中所能够容纳数据类型的枚举值。", 
        0, NULL, LDT_ENUM, 0, 0, (PEVENT_INFO2)NULL, 0, NULL, NULL},
};

int g_DataType_krnln_global_var_count = 48;

#endif
