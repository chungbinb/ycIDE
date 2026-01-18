#ifndef __E_STATIC_LIB
#include "include_eapi_header.h"

// 接口常量  下的枚举常量成员数组
static LIB_DATA_TYPE_ELEMENT s_objEventeapi_ApiConst_static_var_00[] = 
{
    /*000*/ {SDT_INT, 0, "控制面板", "CONTROL_WINDOWS", NULL, LES_HAS_DEFAULT_VALUE, (INT)0x00000000},
    /*001*/ {SDT_INT, 0, "系统属性", "SYSTEM_WINDOWS", NULL, LES_HAS_DEFAULT_VALUE, (INT)0x00000001},
    /*002*/ {SDT_INT, 0, "显示属性", "DISPLAY_WINDOWS", NULL, LES_HAS_DEFAULT_VALUE, (INT)0x00000002},
    /*003*/ {SDT_INT, 0, "辅助功能选项", "ADD_OPTIONS_WINDOWS", NULL, LES_HAS_DEFAULT_VALUE, (INT)0x00000003},
    /*004*/ {SDT_INT, 0, "添加打印机向导", "ADD_PRINTER_WINDOWS", NULL, LES_HAS_DEFAULT_VALUE, (INT)0x00000004},
    /*005*/ {SDT_INT, 0, "添加或删除程序", "ADD_AND_DEL_WINDOWS", NULL, LES_HAS_DEFAULT_VALUE, (INT)0x00000005},
    /*006*/ {SDT_INT, 0, "日期和时间属性", "DATETIME_WINDOWS", NULL, LES_HAS_DEFAULT_VALUE, (INT)0x00000006},
    /*007*/ {SDT_INT, 0, "字体文件夹", "FONTSFOLDER_WINDOWS", NULL, LES_HAS_DEFAULT_VALUE, (INT)0x00000007},
    /*008*/ {SDT_INT, 0, "游戏控制器", "GAME_SET_WINDOWS", NULL, LES_HAS_DEFAULT_VALUE, (INT)0x00000008},
    /*009*/ {SDT_INT, 0, "鼠标属性", "MOUSE_WINDOWS", NULL, LES_HAS_DEFAULT_VALUE, (INT)0x00000009},
    /*010*/ {SDT_INT, 0, "键盘属性", "KEYBORAD_WINDOWS", NULL, LES_HAS_DEFAULT_VALUE, (INT)0x0000000A},
    /*011*/ {SDT_INT, 0, "声音和音频属性", "MEDIA_WINDOWS", NULL, LES_HAS_DEFAULT_VALUE, (INT)0x0000000B},
    /*012*/ {SDT_INT, 0, "ODBC数据源管理器", "ODBC_WINDOWS", NULL, LES_HAS_DEFAULT_VALUE, (INT)0x0000000C},
    /*013*/ {SDT_INT, 0, "电源选项属性", "POWER_WINDOWS", NULL, LES_HAS_DEFAULT_VALUE, (INT)0x0000000D},
    /*014*/ {SDT_INT, 0, "区域和语言选项", "LANGUAGE_WINDOWS", NULL, LES_HAS_DEFAULT_VALUE, (INT)0x0000000E},
    /*015*/ {SDT_INT, 0, "互联网属性", "INTERNET_WINDOWS", NULL, LES_HAS_DEFAULT_VALUE, (INT)0x0000000F},
    /*016*/ {SDT_INT, 0, "用户帐号", "USERACCOUNT_WINDOWS", NULL, LES_HAS_DEFAULT_VALUE, (INT)0x00000010},
    /*017*/ {SDT_INT, 0, "系统信息对话框", "SYSTEM_INFO_DLG", NULL, LES_HAS_DEFAULT_VALUE, (INT)0x00000011},
    /*018*/ {SDT_INT, 0, "左键单击", "LeftClick", NULL, LES_HAS_DEFAULT_VALUE, (INT)0x00000000},
    /*019*/ {SDT_INT, 0, "左键双击", "LeftDblClick", NULL, LES_HAS_DEFAULT_VALUE, (INT)0x00000001},
    /*020*/ {SDT_INT, 0, "右键单击", "RightClick", NULL, LES_HAS_DEFAULT_VALUE, (INT)0x00000002},
    /*021*/ {SDT_INT, 0, "右键双击", "RightDblClick", NULL, LES_HAS_DEFAULT_VALUE, (INT)0x00000003},
    /*022*/ {SDT_INT, 0, "总物理内存", "PhysMemory", NULL, LES_HAS_DEFAULT_VALUE, (INT)0x00000000},
    /*023*/ {SDT_INT, 0, "可用物理内存", "AvailPhysMemory", NULL, LES_HAS_DEFAULT_VALUE, (INT)0x00000001},
    /*024*/ {SDT_INT, 0, "总虚拟内存", "VitualMemory", NULL, LES_HAS_DEFAULT_VALUE, (INT)0x00000002},
    /*025*/ {SDT_INT, 0, "可用虚拟内存", "AvailVitualMemory", NULL, LES_HAS_DEFAULT_VALUE, (INT)0x00000003},
    /*026*/ {SDT_INT, 0, "总页面文件", "PageFile", NULL, LES_HAS_DEFAULT_VALUE, (INT)0x00000004},
    /*027*/ {SDT_INT, 0, "可用页面文件", "AvailPageFile", NULL, LES_HAS_DEFAULT_VALUE, (INT)0x00000005},
    /*028*/ {SDT_INT, 0, "居中", "Center", NULL, LES_HAS_DEFAULT_VALUE, (INT)0x00000000},
    /*029*/ {SDT_INT, 0, "平铺", "Tile", NULL, LES_HAS_DEFAULT_VALUE, (INT)0x00000001},
    /*030*/ {SDT_INT, 0, "拉伸", "Stretch", NULL, LES_HAS_DEFAULT_VALUE, (INT)0x00000002},
    /*031*/ {SDT_INT, 0, "所有驱动器", "AllDrives", NULL, LES_HAS_DEFAULT_VALUE, (INT)0x00000000},
    /*032*/ {SDT_INT, 0, "硬盘驱动器", "HD", NULL, LES_HAS_DEFAULT_VALUE, (INT)0x00000001},
    /*033*/ {SDT_INT, 0, "光盘驱动器", "CD-ROM", NULL, LES_HAS_DEFAULT_VALUE, (INT)0x00000002},
    /*034*/ {SDT_INT, 0, "可移动驱动器", "Removable", NULL, LES_HAS_DEFAULT_VALUE, (INT)0x00000003},
    /*035*/ {SDT_INT, 0, "系统临时文件", "Temp", NULL, LES_HAS_DEFAULT_VALUE, (INT)0x00000000},
    /*036*/ {SDT_INT, 0, "IE临时文件", "IE Temp", NULL, LES_HAS_DEFAULT_VALUE, (INT)0x00000001},
    /*037*/ {SDT_INT, 0, "回收站", "Recycle", NULL, LES_HAS_DEFAULT_VALUE, (INT)0x00000002},
    /*038*/ {SDT_INT, 0, "存根文件", "Cookies", NULL, LES_HAS_DEFAULT_VALUE, (INT)0x00000003},
    /*039*/ {SDT_INT, 0, "浏览器历史地址记录", NULL, NULL, LES_HAS_DEFAULT_VALUE, (INT)0x00000000},
    /*040*/ {SDT_INT, 0, "表单自动完成历史记录", NULL, NULL, LES_HAS_DEFAULT_VALUE, (INT)0x00000001},
    /*041*/ {SDT_INT, 0, "自动密码历史记录", NULL, NULL, LES_HAS_DEFAULT_VALUE, (INT)0x00000002},
    /*042*/ {SDT_INT, 0, "收藏夹", NULL, NULL, LES_HAS_DEFAULT_VALUE, (INT)0x00000003},
    /*043*/ {SDT_INT, 0, "自动拨号历史记录", NULL, NULL, LES_HAS_DEFAULT_VALUE, (INT)0x00000004},
    /*044*/ {SDT_INT, 0, "运行中的自动匹配历史记录", NULL, NULL, LES_HAS_DEFAULT_VALUE, (INT)0x00000005},
    /*045*/ {SDT_INT, 0, "打开文档历史记录", NULL, NULL, LES_HAS_DEFAULT_VALUE, (INT)0x00000006},
    /*046*/ {SDT_INT, 0, "网络连接历史记录", NULL, NULL, LES_HAS_DEFAULT_VALUE, (INT)0x00000007},
    /*047*/ {SDT_INT, 0, "远程登录历史记录", NULL, NULL, LES_HAS_DEFAULT_VALUE, (INT)0x00000008},
    /*048*/ {SDT_INT, 0, "浏览网址历史记录", NULL, NULL, LES_HAS_DEFAULT_VALUE, (INT)0x00000009},
    /*049*/ {SDT_INT, 0, "数字锁定键盘灯", "Num Lock", NULL, LES_HAS_DEFAULT_VALUE, (INT)0x00000000},
    /*050*/ {SDT_INT, 0, "大小写锁定键盘灯", "Caps Lock", NULL, LES_HAS_DEFAULT_VALUE, (INT)0x00000001},
    /*051*/ {SDT_INT, 0, "滚动锁定键盘灯", "Scroll Lock", NULL, LES_HAS_DEFAULT_VALUE, (INT)0x00000002},
    /*052*/ {SDT_INT, 0, "像素", "UNIT_PT", NULL, LES_HAS_DEFAULT_VALUE, (INT)0x00000000},
    /*053*/ {SDT_INT, 0, "英寸", "UNIT_INCH", NULL, LES_HAS_DEFAULT_VALUE, (INT)0x00000001},
    /*054*/ {SDT_INT, 0, "毫米", "UNIT_MM", NULL, LES_HAS_DEFAULT_VALUE, (INT)0x00000002},
    /*055*/ {SDT_INT, 0, "厘米", "UNIT_CM", NULL, LES_HAS_DEFAULT_VALUE, (INT)0x00000003},
    /*056*/ {SDT_INT, 0, "到文件", "ToFile", NULL, LES_HAS_DEFAULT_VALUE, (INT)0x00000000},
    /*057*/ {SDT_INT, 0, "到剪贴板", "ToClipboard", NULL, LES_HAS_DEFAULT_VALUE, (INT)0x00000001},
    /*058*/ {SDT_INT, 0, "到字节集", "ToBin", NULL, LES_HAS_DEFAULT_VALUE, (INT)0x00000002},
    /*059*/ {SDT_INT, 0, "磁盘0", "Primary Controller - Master drive", NULL, LES_HAS_DEFAULT_VALUE, (INT)0x00000000},
    /*060*/ {SDT_INT, 0, "磁盘1", "Primary Controller - Slave drive", NULL, LES_HAS_DEFAULT_VALUE, (INT)0x00000001},
    /*061*/ {SDT_INT, 0, "磁盘2", "Secondary Controller - Master drive", NULL, LES_HAS_DEFAULT_VALUE, (INT)0x00000002},
    /*062*/ {SDT_INT, 0, "磁盘3", "Secondary Controller - Slave drive", NULL, LES_HAS_DEFAULT_VALUE, (INT)0x00000003},
    /*063*/ {SDT_INT, 0, "开始菜单", NULL, NULL, LES_HAS_DEFAULT_VALUE, (INT)0x00000000},
    /*064*/ {SDT_INT, 0, "开始菜单程序", NULL, NULL, LES_HAS_DEFAULT_VALUE, (INT)0x00000001},
    /*065*/ {SDT_INT, 0, "桌面", NULL, NULL, LES_HAS_DEFAULT_VALUE, (INT)0x00000002},
    /*066*/ {SDT_INT, 0, "到注册表", NULL, NULL, LES_HAS_DEFAULT_VALUE, (INT)0x00000000},
    /*067*/ {SDT_INT, 0, "到启动组", NULL, NULL, LES_HAS_DEFAULT_VALUE, (INT)0x00000001},
    /*068*/ {SDT_INT, 0, "计算机", NULL, NULL, LES_HAS_DEFAULT_VALUE, (INT)0x00000000},
    /*069*/ {SDT_INT, 0, "工作组", NULL, NULL, LES_HAS_DEFAULT_VALUE, (INT)0x00000001},
    /*070*/ {SDT_INT, 0, "广播", NULL, NULL, LES_HAS_DEFAULT_VALUE, (INT)0x00000002},
    /*071*/ {SDT_INT, 0, "所有共享资源", NULL, NULL, LES_HAS_DEFAULT_VALUE, (INT)0x00000000},
    /*072*/ {SDT_INT, 0, "共享目录或文件", NULL, NULL, LES_HAS_DEFAULT_VALUE, (INT)0x00000001},
    /*073*/ {SDT_INT, 0, "共享打印机", NULL, NULL, LES_HAS_DEFAULT_VALUE, (INT)0x00000002},
    /*074*/ {SDT_INT, 0, "网络驱动器", NULL, NULL, LES_HAS_DEFAULT_VALUE, (INT)0x00000004},
    /*075*/ {SDT_INT, 0, "标准打印设置对话框", NULL, NULL, LES_HAS_DEFAULT_VALUE, (INT)0x00000000},
    /*076*/ {SDT_INT, 0, "打印对话框", NULL, NULL, LES_HAS_DEFAULT_VALUE, (INT)0x00000001},
    /*077*/ {SDT_INT, 0, "打印属性对话框", NULL, NULL, LES_HAS_DEFAULT_VALUE, (INT)0x00000002},

};
static int s_objEventeapi_ApiConst_static_var_count_00 = sizeof(s_objEventeapi_ApiConst_static_var_00) / sizeof(s_objEventeapi_ApiConst_static_var_00[0]);

// 矩形数据  下的枚举常量成员数组
static LIB_DATA_TYPE_ELEMENT s_objEventeapi_RECT_static_var_01[] = 
{
    /*000*/ {SDT_INT, 0, "左边", "Left", "left", NULL, (INT)0x00000000},
    /*001*/ {SDT_INT, 0, "顶边", "Top", "top", NULL, (INT)0x00000000},
    /*002*/ {SDT_INT, 0, "右边", "Right", "right", NULL, (INT)0x00000000},
    /*003*/ {SDT_INT, 0, "底边", "Bottom", "bottom", NULL, (INT)0x00000000},

};
static int s_objEventeapi_RECT_static_var_count_01 = sizeof(s_objEventeapi_RECT_static_var_01) / sizeof(s_objEventeapi_RECT_static_var_01[0]);

// CPU信息  下的枚举常量成员数组
static LIB_DATA_TYPE_ELEMENT s_objEventeapi_CpuInf_static_var_02[] = 
{
    /*000*/ {SDT_TEXT, 0, "制造商", "Vendor", NULL, NULL, (INT)0x00000000},
    /*001*/ {SDT_TEXT, 0, "名称", "ProcessorName", NULL, NULL, (INT)0x00000000},
    /*002*/ {SDT_INT, 0, "时钟频率", "ClockFrequency", "单位：MHz", NULL, (INT)0x00000000},
    /*003*/ {SDT_TEXT, 0, "描述", "Description", NULL, NULL, (INT)0x00000000},
    /*004*/ {SDT_INT, 0, "一级缓存大小", "L1 Size", "单位：KB", NULL, (INT)0x00000000},
    /*005*/ {SDT_INT, 0, "二级缓存大小", "L2 Size", "单位：KB", NULL, (INT)0x00000000},
    /*006*/ {SDT_TEXT, 0, "序列号", "SerialNumber", NULL, NULL, (INT)0x00000000},
    /*007*/ {SDT_INT, 0, "数据宽度", "bit", "单位：位", NULL, (INT)0x00000000},
    /*008*/ {SDT_INT, 0, "三级缓存大小", "L3 Size", "单位：KB", NULL, (INT)0x00000000},
    /*009*/ {SDT_INT, 0, "核心数", "NumberOfCores", "单位：KB", NULL, (INT)0x00000000},
    /*010*/ {SDT_INT, 0, "线程数", "ThreadCount", "单位：KB", NULL, (INT)0x00000000},

};
static int s_objEventeapi_CpuInf_static_var_count_02 = sizeof(s_objEventeapi_CpuInf_static_var_02) / sizeof(s_objEventeapi_CpuInf_static_var_02[0]);

// 系统信息  下的枚举常量成员数组
static LIB_DATA_TYPE_ELEMENT s_objEventeapi_CpuInf_static_var_03[] = 
{
    /*000*/ {SDT_TEXT, 0, "操作系统", "Vendor", NULL, NULL, (INT)0x00000000},
    /*001*/ {SDT_TEXT, 0, "版本", "ProcessorName", NULL, NULL, (INT)0x00000000},
    /*002*/ {SDT_INT, 0, "编译版本", "ProcessorName", NULL, NULL, (INT)0x00000000},
    /*003*/ {SDT_TEXT, 0, "最后服务包", "ProcessorName", NULL, NULL, (INT)0x00000000},
    /*004*/ {SDT_TEXT, 0, "当前用户", "ProcessorName", NULL, NULL, (INT)0x00000000},
    /*005*/ {SDT_TEXT, 0, "系统安装目录", "ProcessorName", NULL, NULL, (INT)0x00000000},
    /*006*/ {SDT_TEXT, 0, "系统目录", "ProcessorName", NULL, NULL, (INT)0x00000000},
    /*007*/ {SDT_INT, 0, "代码", "ProcessorName", NULL, NULL, (INT)0x00000000},
    /*008*/ {SDT_TEXT, 0, "本地语言", "ProcessorName", NULL, NULL, (INT)0x00000000},
    /*009*/ {SDT_INT, 0, "国家代码", "ProcessorName", NULL, NULL, (INT)0x00000000},
    /*010*/ {SDT_TEXT, 0, "国家", "ProcessorName", NULL, NULL, (INT)0x00000000},
    /*011*/ {SDT_TEXT, 0, "货币符号", "ProcessorName", NULL, NULL, (INT)0x00000000},
    /*012*/ {SDT_TEXT, 0, "日期格式", "ProcessorName", NULL, NULL, (INT)0x00000000},
    /*013*/ {SDT_TEXT, 0, "时间格式", "ProcessorName", NULL, NULL, (INT)0x00000000},
    /*014*/ {SDT_TEXT, 0, "时区", "ProcessorName", NULL, NULL, (INT)0x00000000},
    /*015*/ {SDT_TEXT, 0, "国家ISO", "ProcessorName", NULL, NULL, (INT)0x00000000},
    /*016*/ {SDT_BOOL, 0, "是否为x64架构", "ProcessorName", NULL, NULL, (INT)0x00000000},

};
static int s_objEventeapi_CpuInf_static_var_count_03 = sizeof(s_objEventeapi_CpuInf_static_var_03) / sizeof(s_objEventeapi_CpuInf_static_var_03[0]);

// 文件版本信息  下的枚举常量成员数组
static LIB_DATA_TYPE_ELEMENT s_objEventeapi_CpuInf_static_var_04[] = 
{
    /*000*/ {SDT_TEXT, 0, "文件版本", "Vendor", NULL, NULL, (INT)0x00000000},
    /*001*/ {SDT_TEXT, 0, "文件描述", "ProcessorName", NULL, NULL, (INT)0x00000000},
    /*002*/ {SDT_TEXT, 0, "内部名称", "ProcessorName", NULL, NULL, (INT)0x00000000},
    /*003*/ {SDT_TEXT, 0, "公司名称", "ProcessorName", NULL, NULL, (INT)0x00000000},
    /*004*/ {SDT_TEXT, 0, "合法版权", "ProcessorName", NULL, NULL, (INT)0x00000000},
    /*005*/ {SDT_TEXT, 0, "合法商标", "ProcessorName", NULL, NULL, (INT)0x00000000},
    /*006*/ {SDT_TEXT, 0, "源文件名", "ProcessorName", NULL, NULL, (INT)0x00000000},
    /*007*/ {SDT_TEXT, 0, "产品名称", "ProcessorName", NULL, NULL, (INT)0x00000000},
    /*008*/ {SDT_TEXT, 0, "产品版本", "ProcessorName", NULL, NULL, (INT)0x00000000},
    /*009*/ {SDT_TEXT, 0, "语言", "ProcessorName", NULL, NULL, (INT)0x00000000},
    /*010*/ {SDT_TEXT, 0, "备注", "ProcessorName", NULL, NULL, (INT)0x00000000},
    /*011*/ {SDT_TEXT, 0, "开发者构建", "ProcessorName", " 个人编译版本说明", NULL, (INT)0x00000000},
    /*012*/ {SDT_TEXT, 0, "特殊构建", "ProcessorName", " 特殊编译版本说明", NULL, (INT)0x00000000},

};
static int s_objEventeapi_CpuInf_static_var_count_04 = sizeof(s_objEventeapi_CpuInf_static_var_04) / sizeof(s_objEventeapi_CpuInf_static_var_04[0]);

// BIOS信息  下的枚举常量成员数组
static LIB_DATA_TYPE_ELEMENT s_objEventeapi_CpuInf_static_var_05[] = 
{
    /*000*/ {SDT_TEXT, 0, "名称", "Vendor", NULL, NULL, (INT)0x00000000},
    /*001*/ {SDT_TEXT, 0, "制造商", "ProcessorName", NULL, NULL, (INT)0x00000000},
    /*002*/ {SDT_TEXT, 0, "版权信息", "ProcessorName", NULL, NULL, (INT)0x00000000},
    /*003*/ {SDT_TEXT, 0, "日期", "ProcessorName", NULL, NULL, (INT)0x00000000},
    /*004*/ {SDT_TEXT, 0, "序列号", "ProcessorName", NULL, NULL, (INT)0x00000000},
    /*005*/ {SDT_TEXT, 0, "OEM信息", "ProcessorName", NULL, NULL, (INT)0x00000000},

};
static int s_objEventeapi_CpuInf_static_var_count_05 = sizeof(s_objEventeapi_CpuInf_static_var_05) / sizeof(s_objEventeapi_CpuInf_static_var_05[0]);

// 硬盘信息  下的枚举常量成员数组
static LIB_DATA_TYPE_ELEMENT s_objEventeapi_CpuInf_static_var_06[] = 
{
    /*000*/ {SDT_TEXT, 0, "型号", "Vendor", NULL, NULL, (INT)0x00000000},
    /*001*/ {SDT_TEXT, 0, "版本号", "ProcessorName", NULL, NULL, (INT)0x00000000},
    /*002*/ {SDT_TEXT, 0, "序列号", "ProcessorName", NULL, NULL, (INT)0x00000000},
    /*003*/ {SDT_INT, 0, "容量", "ProcessorName", NULL, NULL, (INT)0x00000000},
    /*004*/ {SDT_INT, 0, "缓存大小", "ProcessorName", NULL, NULL, (INT)0x00000000},
    /*005*/ {SDT_INT, 0, "每磁道扇区数", "ProcessorName", NULL, NULL, (INT)0x00000000},
    /*006*/ {SDT_INT, 0, "磁头数", "NumHeaders", NULL, NULL, (INT)0x00000000},
    /*007*/ {SDT_INT, 0, "柱面数", "ProcessorName", NULL, NULL, (INT)0x00000000},

};
static int s_objEventeapi_CpuInf_static_var_count_06 = sizeof(s_objEventeapi_CpuInf_static_var_06) / sizeof(s_objEventeapi_CpuInf_static_var_06[0]);

// 网卡信息  下的枚举常量成员数组
static LIB_DATA_TYPE_ELEMENT s_objEventeapi_NetCardInf_static_var_07[] = 
{
    /*000*/ {SDT_TEXT, 0, "网卡名称", "Vendor", NULL, NULL, (INT)0x00000000},
    /*001*/ {SDT_INT, 0, "序号", "ProcessorName", NULL, NULL, (INT)0x00000000},
    /*002*/ {SDT_TEXT, 0, "物理地址", "ProcessorName", NULL, NULL, (INT)0x00000000},
    /*003*/ {SDT_TEXT, 0, "IP地址", "ProcessorName", NULL, NULL, (INT)0x00000000},
    /*004*/ {SDT_TEXT, 0, "子网掩码", "ProcessorName", NULL, NULL, (INT)0x00000000},
    /*005*/ {SDT_TEXT, 0, "默认网关", "ProcessorName", NULL, NULL, (INT)0x00000000},
    /*006*/ {SDT_TEXT, 0, "Wins服务器地址", "NumHeaders", NULL, NULL, (INT)0x00000000},
    /*007*/ {SDT_TEXT, 0, "DHCP服务器地址", "ProcessorName", NULL, NULL, (INT)0x00000000},

};
static int s_objEventeapi_NetCardInf_static_var_count_07 = sizeof(s_objEventeapi_NetCardInf_static_var_07) / sizeof(s_objEventeapi_NetCardInf_static_var_07[0]);

// 进程信息  下的枚举常量成员数组
static LIB_DATA_TYPE_ELEMENT s_objEventeapi_ProcInf_static_var_08[] = 
{
    /*000*/ {SDT_INT, 0, "进程标识符", "Vendor", NULL, NULL, (INT)0x00000000},
    /*001*/ {SDT_TEXT, 0, "进程名称", "ProcessorName", NULL, NULL, (INT)0x00000000},
    /*002*/ {SDT_INT, 0, "进程模块标识符", "ProcessorName", NULL, NULL, (INT)0x00000000},
    /*003*/ {SDT_INT, 0, "父进程标识符", "ProcessorName", NULL, NULL, (INT)0x00000000},
    /*004*/ {SDT_INT, 0, "线程优先权", "ProcessorName", NULL, NULL, (INT)0x00000000},
    /*005*/ {SDT_INT, 0, "线程数", "ProcessorName", NULL, NULL, (INT)0x00000000},

};
static int s_objEventeapi_ProcInf_static_var_count_08 = sizeof(s_objEventeapi_ProcInf_static_var_08) / sizeof(s_objEventeapi_ProcInf_static_var_08[0]);

// 显示模式信息  下的枚举常量成员数组
static LIB_DATA_TYPE_ELEMENT s_objEventeapi_DisPlayInf_static_var_09[] = 
{
    /*000*/ {SDT_INT, 0, "宽度", "Vendor", NULL, NULL, (INT)0x00000000},
    /*001*/ {SDT_INT, 0, "高度", "ProcessorName", NULL, NULL, (INT)0x00000000},
    /*002*/ {SDT_INT, 0, "颜色位数", "ProcessorName", NULL, NULL, (INT)0x00000000},
    /*003*/ {SDT_INT, 0, "刷新频率", "ProcessorName", NULL, NULL, (INT)0x00000000},

};
static int s_objEventeapi_DisPlayInf_static_var_count_09 = sizeof(s_objEventeapi_DisPlayInf_static_var_09) / sizeof(s_objEventeapi_DisPlayInf_static_var_09[0]);

// 打印信息  下的枚举常量成员数组
static LIB_DATA_TYPE_ELEMENT s_objEventeapi_PrintInf_static_var_10[] = 
{
    /*000*/ {SDT_TEXT, 0, "打印机名称", "Vendor", NULL, NULL, (INT)0x00000000},
    /*001*/ {SDT_INT, 0, "纸张方向", "Vendor", "指定打印纸的放置方向。可以为以下常量值之一： 1、#纵向； 2、#横向。如省略本参数，默认为“#纵向”。", NULL, (INT)0x00000000},
    /*002*/ {SDT_INT, 0, "纸张类型", "ProcessorName", "指定所使用的打印纸类型。可以为以下常量值之一：-1、自定义纸张（必须在后面参数中指定自定义纸张的大小）； 0、#默认纸； 1、#A3纸(297x420毫米)； 2、#A4纸(210x297毫米)； 3、#A5纸(148x210毫米)； 4、#B4纸(250x354毫米)； 5、#B5纸(182x257毫米)； 6、#四开(215x275毫米)； 7、#十六开(146x215毫米)； 8、#三十二开(97x151毫米)； 9、#信纸(216x279毫米)； 10、#法律用纸(216x355毫米)； 11、#行政用纸(184x266毫米)； 12、#声明(140x216毫米)； 13、#小报(279x432毫米)； 14、#笔记(216x279毫米)； 15、#帐本(432x279毫米)； 16、#对开纸(216x330毫米)。", NULL, (INT)0x00000000},
    /*003*/ {SDT_INT, 0, "纸张宽度", "ProcessorName", "指定自定义纸张的宽度，单位为0.1毫米。只有当前面的“纸张”参数给定值为-1时，本参数才有作用且必须提供具体值。", NULL, (INT)0x00000000},
    /*004*/ {SDT_INT, 0, "纸张高度", "ProcessorName", "指定自定义纸张的高度，单位为0.1毫米。只有当前面的“纸张”参数给定值为-1时，本参数才有作用且必须提供具体值。", NULL, (INT)0x00000000},
    /*005*/ {SDT_INT, 0, "打印份数", "ProcessorName", NULL, NULL, (INT)0x00000000},

};
static int s_objEventeapi_PrintInf_static_var_count_10 = sizeof(s_objEventeapi_PrintInf_static_var_10) / sizeof(s_objEventeapi_PrintInf_static_var_10[0]);

LIB_DATA_TYPE_INFO g_DataType_eapi_global_var[] = 
{
    //1=中文名字,2=英文名字,3=详细解释,4=命令数量,5=索引值,6=标志 LDT_
    //类型为窗口或菜单组件时有效 7=资源ID,8=事件数量,9=组件事件数组,10=属性数  11=属性数组 12=组件交互子程序
    //不为窗口、菜单组件或为枚举数据类型时才有效 13=成员数量,14=成员数据数组
    
/*000*/ { "接口常量", "ApiConst", NULL,
            NULL, NULL, _DT_OS(__OS_WIN) | LDT_ENUM,
            0, NULL, NULL,
            NULL, NULL,
            NULL,
            s_objEventeapi_ApiConst_static_var_count_00, s_objEventeapi_ApiConst_static_var_00
        },
/*001*/ { "矩形数据", "RECT", NULL,
            NULL, NULL, _DT_OS(__OS_WIN),
            0, NULL, NULL,
            NULL, NULL,
            NULL,
            s_objEventeapi_RECT_static_var_count_01, s_objEventeapi_RECT_static_var_01
        },
/*002*/ { "CPU信息", "CpuInf", NULL,
            NULL, NULL, _DT_OS(__OS_WIN),
            0, NULL, NULL,
            NULL, NULL,
            NULL,
            s_objEventeapi_CpuInf_static_var_count_02, s_objEventeapi_CpuInf_static_var_02
        },
/*003*/ { "系统信息", "CpuInf", NULL,
            NULL, NULL, _DT_OS(__OS_WIN),
            0, NULL, NULL,
            NULL, NULL,
            NULL,
            s_objEventeapi_CpuInf_static_var_count_03, s_objEventeapi_CpuInf_static_var_03
        },
/*004*/ { "文件版本信息", "CpuInf", NULL,
            NULL, NULL, _DT_OS(__OS_WIN),
            0, NULL, NULL,
            NULL, NULL,
            NULL,
            s_objEventeapi_CpuInf_static_var_count_04, s_objEventeapi_CpuInf_static_var_04
        },
/*005*/ { "BIOS信息", "CpuInf", NULL,
            NULL, NULL, _DT_OS(__OS_WIN),
            0, NULL, NULL,
            NULL, NULL,
            NULL,
            s_objEventeapi_CpuInf_static_var_count_05, s_objEventeapi_CpuInf_static_var_05
        },
/*006*/ { "硬盘信息", "CpuInf", NULL,
            NULL, NULL, _DT_OS(__OS_WIN),
            0, NULL, NULL,
            NULL, NULL,
            NULL,
            s_objEventeapi_CpuInf_static_var_count_06, s_objEventeapi_CpuInf_static_var_06
        },
/*007*/ { "网卡信息", "NetCardInf", NULL,
            NULL, NULL, _DT_OS(__OS_WIN),
            0, NULL, NULL,
            NULL, NULL,
            NULL,
            s_objEventeapi_NetCardInf_static_var_count_07, s_objEventeapi_NetCardInf_static_var_07
        },
/*008*/ { "进程信息", "ProcInf", NULL,
            NULL, NULL, _DT_OS(__OS_WIN),
            0, NULL, NULL,
            NULL, NULL,
            NULL,
            s_objEventeapi_ProcInf_static_var_count_08, s_objEventeapi_ProcInf_static_var_08
        },
/*009*/ { "显示模式信息", "DisPlayInf", NULL,
            NULL, NULL, _DT_OS(__OS_WIN),
            0, NULL, NULL,
            NULL, NULL,
            NULL,
            s_objEventeapi_DisPlayInf_static_var_count_09, s_objEventeapi_DisPlayInf_static_var_09
        },
/*010*/ { "打印信息", "PrintInf", NULL,
            NULL, NULL, _DT_OS(__OS_WIN),
            0, NULL, NULL,
            NULL, NULL,
            NULL,
            s_objEventeapi_PrintInf_static_var_count_10, s_objEventeapi_PrintInf_static_var_10
        },
};
int g_DataType_eapi_global_var_count = sizeof(g_DataType_eapi_global_var) / sizeof(g_DataType_eapi_global_var[0]);

#endif

