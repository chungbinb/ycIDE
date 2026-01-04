#ifndef __E_STATIC_LIB
#include "include_exmlrpc_header.h"

// 远程服务  下的方法索引
static INT s_dtCmdIndexexmlrpc_ERPCServer_static_var_00[] = 
{
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 
    10, 11, 12, 13, 27, 28, 
};
static int s_dtCmdIndexexmlrpc_ERPCServer_static_var_count_00 = sizeof(s_dtCmdIndexexmlrpc_ERPCServer_static_var_00) / sizeof(s_dtCmdIndexexmlrpc_ERPCServer_static_var_00[0]);

// 请求客户端  下的方法索引
static INT s_dtCmdIndexexmlrpc_ERPCClient_static_var_01[] = 
{
    14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 
    24, 25, 26, 
};
static int s_dtCmdIndexexmlrpc_ERPCClient_static_var_count_01 = sizeof(s_dtCmdIndexexmlrpc_ERPCClient_static_var_01) / sizeof(s_dtCmdIndexexmlrpc_ERPCClient_static_var_01[0]);

// 远程服务  下的枚举常量成员数组
static LIB_DATA_TYPE_ELEMENT s_objEventexmlrpc_ERPCServer_static_var_00[] = 
{
    /*000*/ {SDT_INT, 0, "服务器的数据结构指针", "ServerDataPtr", NULL, NULL, (INT)0x00000000},

};
static int s_objEventexmlrpc_ERPCServer_static_var_count_00 = sizeof(s_objEventexmlrpc_ERPCServer_static_var_00) / sizeof(s_objEventexmlrpc_ERPCServer_static_var_00[0]);

// 请求客户端  下的枚举常量成员数组
static LIB_DATA_TYPE_ELEMENT s_objEventexmlrpc_ERPCClient_static_var_01[] = 
{
    /*000*/ {SDT_INT, 0, "已经废弃，不用", "ConnectFlag", "1为已经成功连接到服务器 0为没有连接到服务器", NULL, (INT)0x00000000},
    /*001*/ {SDT_INT, 0, "客户端数据结构指针", "ClientDataPtr", NULL, NULL, (INT)0x00000000},

};
static int s_objEventexmlrpc_ERPCClient_static_var_count_01 = sizeof(s_objEventexmlrpc_ERPCClient_static_var_01) / sizeof(s_objEventexmlrpc_ERPCClient_static_var_01[0]);

LIB_DATA_TYPE_INFO g_DataType_exmlrpc_global_var[] = 
{
    //1=中文名字,2=英文名字,3=详细解释,4=命令数量,5=索引值,6=标志 LDT_
    //类型为窗口或菜单组件时有效 7=资源ID,8=事件数量,9=组件事件数组,10=属性数  11=属性数组 12=组件交互子程序
    //不为窗口、菜单组件或为枚举数据类型时才有效 13=成员数量,14=成员数据数组
    
/*000*/ { "远程服务", "ERPCServer", "该数据类型提供对服务器端程序的支持",
            s_dtCmdIndexexmlrpc_ERPCServer_static_var_count_00, s_dtCmdIndexexmlrpc_ERPCServer_static_var_00, _DT_OS(__OS_WIN),
            0, NULL, NULL,
            NULL, NULL,
            NULL,
            s_objEventexmlrpc_ERPCServer_static_var_count_00, s_objEventexmlrpc_ERPCServer_static_var_00
        },
/*001*/ { "请求客户端", "ERPCClient", "该数据类型提供对客户端程序的支持",
            s_dtCmdIndexexmlrpc_ERPCClient_static_var_count_01, s_dtCmdIndexexmlrpc_ERPCClient_static_var_01, _DT_OS(__OS_WIN),
            0, NULL, NULL,
            NULL, NULL,
            NULL,
            s_objEventexmlrpc_ERPCClient_static_var_count_01, s_objEventexmlrpc_ERPCClient_static_var_01
        },
};
int g_DataType_exmlrpc_global_var_count = sizeof(g_DataType_exmlrpc_global_var) / sizeof(g_DataType_exmlrpc_global_var[0]);

#endif

