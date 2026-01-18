#ifndef __E_STATIC_LIB
#include "include_ewizard_header.h"

// 程序项类型  下的枚举常量成员数组
static LIB_DATA_TYPE_ELEMENT s_objEventewizard_AppItemType_static_var_00[] = 
{
    /*000*/ {SDT_INT, 0, "程序集", "Module", NULL, NULL, (INT)0x00000001},
    /*001*/ {SDT_INT, 0, "子程序", "Sub", "子程序可以“子程序名称”或“程序集名.子程序名”格式来表示", NULL, (INT)0x00000002},
    /*002*/ {SDT_INT, 0, "全局变量", "GlobalVar", NULL, NULL, (INT)0x00000004},
    /*003*/ {SDT_INT, 0, "程序集变量", "ModuleVar", "程序集变量以“程序集名.变量名”格式来表示", NULL, (INT)0x00000008},
    /*004*/ {SDT_INT, 0, "局部变量", "LocalVar", "局部变量以“子程序名.局部变量名”或“程序集名.子程序名.局部变量名”格式来表示", NULL, (INT)0x00000010},
    /*005*/ {SDT_INT, 0, "子程序参数", "SubArg", "子程序参数以“子程序名.参数名”或“程序集名.子程序名.参数名”格式来表示", NULL, (INT)0x00000020},
    /*006*/ {SDT_INT, 0, "数据类型", "DataType", NULL, NULL, (INT)0x00000040},
    /*007*/ {SDT_INT, 0, "数据类型成员", "DataTypeElement", "数据类型成员以“数据类型名.成员名”格式来表示", NULL, (INT)0x00000080},
    /*008*/ {SDT_INT, 0, "DLL命令", "DllCmd", NULL, NULL, (INT)0x00000100},
    /*009*/ {SDT_INT, 0, "DLL命令参数", "DllCmdArg", "DLL命令参数以“DLL命令名.参数名”格式来表示", NULL, (INT)0x00000200},
    /*010*/ {SDT_INT, 0, "窗口", "Win", NULL, NULL, (INT)0x00000400},
    /*011*/ {SDT_INT, 0, "窗口组件", "WinControl", "窗口组件以“窗口名.窗口组件名”格式来表示", NULL, (INT)0x00000800},
    /*012*/ {SDT_INT, 0, "常量或资源", "Resource", NULL, NULL, (INT)0x00001000},

};
static int s_objEventewizard_AppItemType_static_var_count_00 = sizeof(s_objEventewizard_AppItemType_static_var_00) / sizeof(s_objEventewizard_AppItemType_static_var_00[0]);

// 程序项属性  下的枚举常量成员数组
static LIB_DATA_TYPE_ELEMENT s_objEventewizard_AppItemProperty_static_var_01[] = 
{
    /*000*/ {SDT_INT, 0, "名称", "Name", "文本型数据", NULL, (INT)0x00000001},
    /*001*/ {SDT_INT, 0, "备注", "Explain", "需要文本型数据，如果设置时提供“$”特定字符串，则只清除已有备注内容中所有的标记和模板条件文本，而不实际置入“$”", NULL, (INT)0x00000002},
    /*002*/ {SDT_INT, 0, "常量或资源值", "Context", "数据类型根据具体常量或资源数据类型定", NULL, (INT)0x00000003},
    /*003*/ {SDT_INT, 0, "数据类型", "DataType", "譬如变量的数据类型、子程序和DLL命令的返回值数据类型等，数据类型为本支持库中所提供的“程序项数据类型”", NULL, (INT)0x00000004},
    /*004*/ {SDT_INT, 0, "数组类型", "AryType", "使用形如“xxx”或“xxx, xxx, ...”的格式文本分别提供单维和多维数组定义，其中“xxx”为数值，代表对应维的上限值", NULL, (INT)0x00000005},
    /*005*/ {SDT_INT, 0, "静态", "Static", "逻辑型数据", NULL, (INT)0x00000006},
    /*006*/ {SDT_INT, 0, "参考", "ByRef", "譬如变量的“参考”类型，DLL命令的“传址”类型等，为逻辑型数据", NULL, (INT)0x00000007},
    /*007*/ {SDT_INT, 0, "可空", "AllowEmpty", "逻辑型数据", NULL, (INT)0x00000008},
    /*008*/ {SDT_INT, 0, "公开", "Public", "逻辑型数据", NULL, (INT)0x00000009},
    /*009*/ {SDT_INT, 0, "收缩", "Hidden", "提供子程序的收缩隐藏状态，为逻辑型数据", NULL, (INT)0x0000000A},
    /*010*/ {SDT_INT, 0, "DLL库文件名", "DllLibFileName", "文本型数据", NULL, (INT)0x0000000B},
    /*011*/ {SDT_INT, 0, "DLL库命令名", "DllLibCmdName", "文本型数据", NULL, (INT)0x0000000C},

};
static int s_objEventewizard_AppItemProperty_static_var_count_01 = sizeof(s_objEventewizard_AppItemProperty_static_var_01) / sizeof(s_objEventewizard_AppItemProperty_static_var_01[0]);

// 程序项数据类型  下的枚举常量成员数组
static LIB_DATA_TYPE_ELEMENT s_objEventewizard_AppItemDataType_static_var_02[] = 
{
    /*000*/ {SDT_INT, 0, "空白型", "null", NULL, NULL, (INT)0x00000000},
    /*001*/ {SDT_INT, 0, "字节型", "byte", NULL, NULL, (INT)0x00000001},
    /*002*/ {SDT_INT, 0, "短整数型", "short", NULL, NULL, (INT)0x00000002},
    /*003*/ {SDT_INT, 0, "整数型", "int", NULL, NULL, (INT)0x00000003},
    /*004*/ {SDT_INT, 0, "长整数型", "int64", NULL, NULL, (INT)0x00000004},
    /*005*/ {SDT_INT, 0, "小数型", "float", NULL, NULL, (INT)0x00000005},
    /*006*/ {SDT_INT, 0, "双精度小数型", "double", NULL, NULL, (INT)0x00000006},
    /*007*/ {SDT_INT, 0, "逻辑型", "bool", NULL, NULL, (INT)0x00000007},
    /*008*/ {SDT_INT, 0, "日期时间型", "DateTime", NULL, NULL, (INT)0x00000008},
    /*009*/ {SDT_INT, 0, "文本型", "text", NULL, NULL, (INT)0x00000009},
    /*010*/ {SDT_INT, 0, "字节集型", "bin", NULL, NULL, (INT)0x0000000A},

};
static int s_objEventewizard_AppItemDataType_static_var_count_02 = sizeof(s_objEventewizard_AppItemDataType_static_var_02) / sizeof(s_objEventewizard_AppItemDataType_static_var_02[0]);

LIB_DATA_TYPE_INFO g_DataType_ewizard_global_var[] = 
{
    //1=中文名字,2=英文名字,3=详细解释,4=命令数量,5=索引值,6=标志 LDT_
    //类型为窗口或菜单组件时有效 7=资源ID,8=事件数量,9=组件事件数组,10=属性数  11=属性数组 12=组件交互子程序
    //不为窗口、菜单组件或为枚举数据类型时才有效 13=成员数量,14=成员数据数组
    
/*000*/ { "程序项类型", "AppItemType", "本类型用作提供所有可用的程序项类型",
            NULL, NULL, _DT_OS(__OS_WIN) | LDT_ENUM,
            0, NULL, NULL,
            NULL, NULL,
            NULL,
            s_objEventewizard_AppItemType_static_var_count_00, s_objEventewizard_AppItemType_static_var_00
        },
/*001*/ { "程序项属性", "AppItemProperty", "本类型用作提供所有可用的程序项属性",
            NULL, NULL, _DT_OS(__OS_WIN) | LDT_ENUM,
            0, NULL, NULL,
            NULL, NULL,
            NULL,
            s_objEventewizard_AppItemProperty_static_var_count_01, s_objEventewizard_AppItemProperty_static_var_01
        },
/*002*/ { "程序项数据类型", "AppItemDataType", "本类型用作提供程序项的数据类型值",
            NULL, NULL, _DT_OS(__OS_WIN) | LDT_ENUM,
            0, NULL, NULL,
            NULL, NULL,
            NULL,
            s_objEventewizard_AppItemDataType_static_var_count_02, s_objEventewizard_AppItemDataType_static_var_02
        },
};
int g_DataType_ewizard_global_var_count = sizeof(g_DataType_ewizard_global_var) / sizeof(g_DataType_ewizard_global_var[0]);

#endif

