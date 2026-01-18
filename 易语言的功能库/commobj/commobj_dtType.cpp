#ifndef __E_STATIC_LIB
#include "include_commobj_header.h"

// 快速文本对象  下的方法索引
static INT s_dtCmdIndexcommobj_RapidString_static_var_00[] = 
{
    20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 
    30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 
    40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 
    50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 
    60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 
    70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 
};
static int s_dtCmdIndexcommobj_RapidString_static_var_count_00 = sizeof(s_dtCmdIndexcommobj_RapidString_static_var_00) / sizeof(s_dtCmdIndexcommobj_RapidString_static_var_00[0]);

// 快速字节集对象  下的方法索引
static INT s_dtCmdIndexcommobj_RapidBinary_static_var_01[] = 
{
    80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 
    90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 
    100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 
    110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 
    120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 
    130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 
};
static int s_dtCmdIndexcommobj_RapidBinary_static_var_count_01 = sizeof(s_dtCmdIndexcommobj_RapidBinary_static_var_01) / sizeof(s_dtCmdIndexcommobj_RapidBinary_static_var_01[0]);

// 快速文本对象  下的枚举常量成员数组
static LIB_DATA_TYPE_ELEMENT s_objEventcommobj_RapidString_static_var_00[] = 
{
    /*000*/ {SDT_INT, 0, NULL, NULL, NULL, LES_HIDED, (INT)0x00000000},
    /*001*/ {SDT_INT, 0, NULL, NULL, NULL, LES_HIDED, (INT)0x00000000},
    /*002*/ {SDT_INT, 0, NULL, NULL, NULL, LES_HIDED, (INT)0x00000000},
    /*003*/ {SDT_INT, 0, NULL, NULL, NULL, LES_HIDED, (INT)0x00000000},
    /*004*/ {SDT_INT, 0, NULL, NULL, NULL, LES_HIDED, (INT)0x00000000},
    /*005*/ {SDT_INT, 0, NULL, NULL, NULL, LES_HIDED, (INT)0x00000000},
    /*006*/ {SDT_INT, 0, NULL, NULL, NULL, LES_HIDED, (INT)0x00000000},
    /*007*/ {SDT_INT, 0, NULL, NULL, NULL, LES_HIDED, (INT)0x00000000},
    /*008*/ {SDT_INT, 0, NULL, NULL, NULL, LES_HIDED, (INT)0x00000000},
    /*009*/ {SDT_INT, 0, NULL, NULL, NULL, LES_HIDED, (INT)0x00000000},

};
static int s_objEventcommobj_RapidString_static_var_count_00 = sizeof(s_objEventcommobj_RapidString_static_var_00) / sizeof(s_objEventcommobj_RapidString_static_var_00[0]);

// 快速字节集对象  下的枚举常量成员数组
static LIB_DATA_TYPE_ELEMENT s_objEventcommobj_RapidBinary_static_var_01[] = 
{
    /*000*/ {SDT_INT, 0, NULL, NULL, NULL, LES_HIDED, (INT)0x00000000},
    /*001*/ {SDT_INT, 0, NULL, NULL, NULL, LES_HIDED, (INT)0x00000000},
    /*002*/ {SDT_INT, 0, NULL, NULL, NULL, LES_HIDED, (INT)0x00000000},
    /*003*/ {SDT_INT, 0, NULL, NULL, NULL, LES_HIDED, (INT)0x00000000},
    /*004*/ {SDT_INT, 0, NULL, NULL, NULL, LES_HIDED, (INT)0x00000000},
    /*005*/ {SDT_INT, 0, NULL, NULL, NULL, LES_HIDED, (INT)0x00000000},
    /*006*/ {SDT_INT, 0, NULL, NULL, NULL, LES_HIDED, (INT)0x00000000},
    /*007*/ {SDT_INT, 0, NULL, NULL, NULL, LES_HIDED, (INT)0x00000000},
    /*008*/ {SDT_INT, 0, NULL, NULL, NULL, LES_HIDED, (INT)0x00000000},
    /*009*/ {SDT_INT, 0, NULL, NULL, NULL, LES_HIDED, (INT)0x00000000},

};
static int s_objEventcommobj_RapidBinary_static_var_count_01 = sizeof(s_objEventcommobj_RapidBinary_static_var_01) / sizeof(s_objEventcommobj_RapidBinary_static_var_01[0]);

LIB_DATA_TYPE_INFO g_DataType_commobj_global_var[] = 
{
    //1=中文名字,2=英文名字,3=详细解释,4=命令数量,5=索引值,6=标志 LDT_
    //类型为窗口或菜单组件时有效 7=资源ID,8=事件数量,9=组件事件数组,10=属性数  11=属性数组 12=组件交互子程序
    //不为窗口、菜单组件或为枚举数据类型时才有效 13=成员数量,14=成员数据数组
    
/*000*/ { "快速文本对象", "RapidString", "提供对文本数据的相关操作。在大批量数据操作时，相对于“文本型”有较高的执行效率。",
            s_dtCmdIndexcommobj_RapidString_static_var_count_00, s_dtCmdIndexcommobj_RapidString_static_var_00, _DT_OS(__OS_WIN),
            0, NULL, NULL,
            NULL, NULL,
            NULL,
            s_objEventcommobj_RapidString_static_var_count_00, s_objEventcommobj_RapidString_static_var_00
        },
/*001*/ { "快速字节集对象", "RapidBinary", "提供对字节集数据的相关操作。在大批量数据操作时，相对于“字节集”有较高的执行效率。",
            s_dtCmdIndexcommobj_RapidBinary_static_var_count_01, s_dtCmdIndexcommobj_RapidBinary_static_var_01, _DT_OS(__OS_WIN),
            0, NULL, NULL,
            NULL, NULL,
            NULL,
            s_objEventcommobj_RapidBinary_static_var_count_01, s_objEventcommobj_RapidBinary_static_var_01
        },
};
int g_DataType_commobj_global_var_count = sizeof(g_DataType_commobj_global_var) / sizeof(g_DataType_commobj_global_var[0]);

#endif

