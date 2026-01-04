#ifndef __E_STATIC_LIB
#include "include_bmpoperate_header.h"

// 位图  下的方法索引
static INT s_dtCmdIndexbmpoperate_DIB_static_var_00[] = 
{
    0, 1, 2, 3, 16, 10, 27, 4, 5, 6, 
    7, 8, 9, 21, 13, 14, 17, 18, 15, 11, 
    19, 12, 20, 23, 22, 28, 24, 25, 26, 29, 
    30, 31, 32, 33, 
};
static int s_dtCmdIndexbmpoperate_DIB_static_var_count_00 = sizeof(s_dtCmdIndexbmpoperate_DIB_static_var_00) / sizeof(s_dtCmdIndexbmpoperate_DIB_static_var_00[0]);

// 位图  下的枚举常量成员数组
static LIB_DATA_TYPE_ELEMENT s_objEventbmpoperate_DIB_static_var_00[] = 
{
    /*000*/ {SDT_INT, 0, "this", NULL, NULL, LES_HIDED, (INT)0x00000000},
    /*001*/ {SDT_INT, 0, "错误信息(STL 的 string)", NULL, NULL, LES_HIDED, (INT)0x00000000},

};
static int s_objEventbmpoperate_DIB_static_var_count_00 = sizeof(s_objEventbmpoperate_DIB_static_var_00) / sizeof(s_objEventbmpoperate_DIB_static_var_00[0]);

LIB_DATA_TYPE_INFO g_DataType_bmpoperate_global_var[] = 
{
    //1=中文名字,2=英文名字,3=详细解释,4=命令数量,5=索引值,6=标志 LDT_
    //类型为窗口或菜单组件时有效 7=资源ID,8=事件数量,9=组件事件数组,10=属性数  11=属性数组 12=组件交互子程序
    //不为窗口、菜单组件或为枚举数据类型时才有效 13=成员数量,14=成员数据数组
    
/*000*/ { "位图", "DIB", "该类型支持对标准的bmp文件格式数据的相关操作，各种压缩格式暂不支持。注：16位的bmp仅支持BI_RGB格式",
            s_dtCmdIndexbmpoperate_DIB_static_var_count_00, s_dtCmdIndexbmpoperate_DIB_static_var_00, _DT_OS(__OS_WIN),
            0, NULL, NULL,
            NULL, NULL,
            NULL,
            s_objEventbmpoperate_DIB_static_var_count_00, s_objEventbmpoperate_DIB_static_var_00
        },
};
int g_DataType_bmpoperate_global_var_count = sizeof(g_DataType_bmpoperate_global_var) / sizeof(g_DataType_bmpoperate_global_var[0]);

#endif

