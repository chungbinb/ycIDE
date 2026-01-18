#ifndef __E_STATIC_LIB
#include "include_console_header.h"

// 控制台对象  下的方法索引
static INT s_dtCmdIndexconsole_Console_static_var_00[] = 
{
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 
    10, 11, 
};
static int s_dtCmdIndexconsole_Console_static_var_count_00 = sizeof(s_dtCmdIndexconsole_Console_static_var_00) / sizeof(s_dtCmdIndexconsole_Console_static_var_00[0]);

// 控制台对象  下的枚举常量成员数组
static LIB_DATA_TYPE_ELEMENT s_objEventconsole_Console_static_var_00[] = 
{
    /*000*/ {SDT_INT, 0, "控制台句柄", "ConsoleHandle", "控制台句柄", LES_HIDED, (INT)0x00000000},

};
static int s_objEventconsole_Console_static_var_count_00 = sizeof(s_objEventconsole_Console_static_var_00) / sizeof(s_objEventconsole_Console_static_var_00[0]);

// 控制台颜色  下的枚举常量成员数组
static LIB_DATA_TYPE_ELEMENT s_objEventconsole_ConsoleColor_static_var_01[] = 
{
    /*000*/ {SDT_INT, 0, "黑色", NULL, "黑色", LES_HAS_DEFAULT_VALUE, (INT)0x00000001},
    /*001*/ {SDT_INT, 0, "红褐", NULL, "红褐色", LES_HAS_DEFAULT_VALUE, (INT)0x00000002},
    /*002*/ {SDT_INT, 0, "墨绿", NULL, "墨绿色", LES_HAS_DEFAULT_VALUE, (INT)0x00000003},
    /*003*/ {SDT_INT, 0, "褐绿", NULL, "褐绿色", LES_HAS_DEFAULT_VALUE, (INT)0x00000004},
    /*004*/ {SDT_INT, 0, "藏青", NULL, "藏青色", LES_HAS_DEFAULT_VALUE, (INT)0x00000005},
    /*005*/ {SDT_INT, 0, "紫红", NULL, "紫红色", LES_HAS_DEFAULT_VALUE, (INT)0x00000006},
    /*006*/ {SDT_INT, 0, "深青", NULL, "深青色", LES_HAS_DEFAULT_VALUE, (INT)0x00000007},
    /*007*/ {SDT_INT, 0, "银白", NULL, "银白色", LES_HAS_DEFAULT_VALUE, (INT)0x00000008},
    /*008*/ {SDT_INT, 0, "灰色", NULL, "灰色", LES_HAS_DEFAULT_VALUE, (INT)0x00000009},
    /*009*/ {SDT_INT, 0, "红色", NULL, "红色", LES_HAS_DEFAULT_VALUE, (INT)0x0000000A},
    /*010*/ {SDT_INT, 0, "绿色", NULL, "绿色", LES_HAS_DEFAULT_VALUE, (INT)0x0000000B},
    /*011*/ {SDT_INT, 0, "黄色", NULL, "黄色", LES_HAS_DEFAULT_VALUE, (INT)0x0000000C},
    /*012*/ {SDT_INT, 0, "蓝色", NULL, "蓝色", LES_HAS_DEFAULT_VALUE, (INT)0x0000000D},
    /*013*/ {SDT_INT, 0, "品红", NULL, "品红色", LES_HAS_DEFAULT_VALUE, (INT)0x0000000E},
    /*014*/ {SDT_INT, 0, "艳青", NULL, "艳青", LES_HAS_DEFAULT_VALUE, (INT)0x0000000F},
    /*015*/ {SDT_INT, 0, "白色", NULL, "白色", LES_HAS_DEFAULT_VALUE, (INT)0x00000010},

};
static int s_objEventconsole_ConsoleColor_static_var_count_01 = sizeof(s_objEventconsole_ConsoleColor_static_var_01) / sizeof(s_objEventconsole_ConsoleColor_static_var_01[0]);

LIB_DATA_TYPE_INFO g_DataType_console_global_var[] = 
{
    //1=中文名字,2=英文名字,3=详细解释,4=命令数量,5=索引值,6=标志 LDT_
    //类型为窗口或菜单组件时有效 7=资源ID,8=事件数量,9=组件事件数组,10=属性数  11=属性数组 12=组件交互子程序
    //不为窗口、菜单组件或为枚举数据类型时才有效 13=成员数量,14=成员数据数组
    
/*000*/ { "控制台对象", "Console", NULL,
            s_dtCmdIndexconsole_Console_static_var_count_00, s_dtCmdIndexconsole_Console_static_var_00, _DT_OS(__OS_WIN),
            0, NULL, NULL,
            NULL, NULL,
            NULL,
            s_objEventconsole_Console_static_var_count_00, s_objEventconsole_Console_static_var_00
        },
/*001*/ { "控制台颜色", "ConsoleColor", "本数据类型包括了控制台颜色常量",
            NULL, NULL, _DT_OS(__OS_WIN) | LDT_ENUM,
            0, NULL, NULL,
            NULL, NULL,
            NULL,
            s_objEventconsole_ConsoleColor_static_var_count_01, s_objEventconsole_ConsoleColor_static_var_01
        },
};
int g_DataType_console_global_var_count = sizeof(g_DataType_console_global_var) / sizeof(g_DataType_console_global_var[0]);

#endif

