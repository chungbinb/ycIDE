#ifndef __E_STATIC_LIB
#include "include_wnet_header.h"

// 局域网操作  下的方法索引
static INT s_dtCmdIndexwnet_WNet_static_var_00[] = 
{
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 
    10, 11, 
};
static int s_dtCmdIndexwnet_WNet_static_var_count_00 = sizeof(s_dtCmdIndexwnet_WNet_static_var_00) / sizeof(s_dtCmdIndexwnet_WNet_static_var_00[0]);

// 局域网操作  下的枚举常量成员数组
static LIB_DATA_TYPE_ELEMENT s_objEventwnet_WNet_static_var_00[] = 
{
    /*000*/ {SDT_INT, 0, "this", NULL, NULL, LES_HIDED, (INT)0x00000000},

};
static int s_objEventwnet_WNet_static_var_count_00 = sizeof(s_objEventwnet_WNet_static_var_00) / sizeof(s_objEventwnet_WNet_static_var_00[0]);

// 资源类型  下的枚举常量成员数组
static LIB_DATA_TYPE_ELEMENT s_objEventwnet_ResType_static_var_01[] = 
{
    /*000*/ {SDT_INT, 0, "全部", "Any", "共享文件夹和共享打印机", NULL, (INT)0x00000000},
    /*001*/ {SDT_INT, 0, "共享文件夹", "SharedFolder", "共享文件夹类型", NULL, (INT)0x00000001},
    /*002*/ {SDT_INT, 0, "共享打印机", "SharedPrinter", "共享打印机类型", NULL, (INT)0x00000002},

};
static int s_objEventwnet_ResType_static_var_count_01 = sizeof(s_objEventwnet_ResType_static_var_01) / sizeof(s_objEventwnet_ResType_static_var_01[0]);

LIB_DATA_TYPE_INFO g_DataType_wnet_global_var[] = 
{
    //1=中文名字,2=英文名字,3=详细解释,4=命令数量,5=索引值,6=标志 LDT_
    //类型为窗口或菜单组件时有效 7=资源ID,8=事件数量,9=组件事件数组,10=属性数  11=属性数组 12=组件交互子程序
    //不为窗口、菜单组件或为枚举数据类型时才有效 13=成员数量,14=成员数据数组
    
/*000*/ { "局域网操作", "WNet", "该组件实现对Windows局域网(Windows Netwroking)相关操作的封装",
            s_dtCmdIndexwnet_WNet_static_var_count_00, s_dtCmdIndexwnet_WNet_static_var_00, _DT_OS(__OS_WIN),
            0, NULL, NULL,
            NULL, NULL,
            NULL,
            s_objEventwnet_WNet_static_var_count_00, s_objEventwnet_WNet_static_var_00
        },
/*001*/ { "资源类型", "ResType", NULL,
            NULL, NULL, _DT_OS(__OS_WIN) | LDT_ENUM,
            0, NULL, NULL,
            NULL, NULL,
            NULL,
            s_objEventwnet_ResType_static_var_count_01, s_objEventwnet_ResType_static_var_01
        },
};
int g_DataType_wnet_global_var_count = sizeof(g_DataType_wnet_global_var) / sizeof(g_DataType_wnet_global_var[0]);

#endif

