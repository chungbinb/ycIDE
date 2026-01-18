#ifndef __E_STATIC_LIB
#include "include_sock_header.h"

// 网络服务器  下的方法索引
static INT s_dtCmdIndexsock_socket_server_static_var_00[] = 
{
    0, 1, 2, 3, 4, 5, 6, 7, 20, 
};
static int s_dtCmdIndexsock_socket_server_static_var_count_00 = sizeof(s_dtCmdIndexsock_socket_server_static_var_00) / sizeof(s_dtCmdIndexsock_socket_server_static_var_00[0]);

// 网络客户端  下的方法索引
static INT s_dtCmdIndexsock_socket_client_static_var_01[] = 
{
    8, 9, 10, 11, 21, 18, 
};
static int s_dtCmdIndexsock_socket_client_static_var_count_01 = sizeof(s_dtCmdIndexsock_socket_client_static_var_01) / sizeof(s_dtCmdIndexsock_socket_client_static_var_01[0]);

// 网络数据报  下的方法索引
static INT s_dtCmdIndexsock_socket_udp_static_var_02[] = 
{
    12, 13, 14, 15, 22, 18, 
};
static int s_dtCmdIndexsock_socket_udp_static_var_count_02 = sizeof(s_dtCmdIndexsock_socket_udp_static_var_02) / sizeof(s_dtCmdIndexsock_socket_udp_static_var_02[0]);

// 网络服务器  下的枚举常量成员数组
static LIB_DATA_TYPE_ELEMENT s_objEventsock_socket_server_static_var_00[] = 
{
    /*000*/ {SDT_INT, 0, "服务器句柄", "S_SOCKET", "服务器的句柄", LES_HIDED, (INT)0x00000000},

};
static int s_objEventsock_socket_server_static_var_count_00 = sizeof(s_objEventsock_socket_server_static_var_00) / sizeof(s_objEventsock_socket_server_static_var_00[0]);

// 网络客户端  下的枚举常量成员数组
static LIB_DATA_TYPE_ELEMENT s_objEventsock_socket_client_static_var_01[] = 
{
    /*000*/ {SDT_INT, 0, "连接句柄", "C_SOCKET", "连接句柄", LES_HIDED, (INT)0x00000000},

};
static int s_objEventsock_socket_client_static_var_count_01 = sizeof(s_objEventsock_socket_client_static_var_01) / sizeof(s_objEventsock_socket_client_static_var_01[0]);

// 网络数据报  下的枚举常量成员数组
static LIB_DATA_TYPE_ELEMENT s_objEventsock_socket_udp_static_var_02[] = 
{
    /*000*/ {SDT_INT, 0, "数据报句柄", "U_SOCKET", "数据报句柄", LES_HIDED, (INT)0x00000000},

};
static int s_objEventsock_socket_udp_static_var_count_02 = sizeof(s_objEventsock_socket_udp_static_var_02) / sizeof(s_objEventsock_socket_udp_static_var_02[0]);

// 对方信息  下的枚举常量成员数组
static LIB_DATA_TYPE_ELEMENT s_objEventsock_halve_info_static_var_03[] = 
{
    /*000*/ {SDT_TEXT, 0, "对方IP", "halve_ip", "对方IP", NULL, (INT)0x00000000},
    /*001*/ {SDT_INT, 0, "对方端口", "halve_port", "对方端口", NULL, (INT)0x00000000},

};
static int s_objEventsock_halve_info_static_var_count_03 = sizeof(s_objEventsock_halve_info_static_var_03) / sizeof(s_objEventsock_halve_info_static_var_03[0]);

LIB_DATA_TYPE_INFO g_DataType_sock_global_var[] = 
{
    //1=中文名字,2=英文名字,3=详细解释,4=命令数量,5=索引值,6=标志 LDT_
    //类型为窗口或菜单组件时有效 7=资源ID,8=事件数量,9=组件事件数组,10=属性数  11=属性数组 12=组件交互子程序
    //不为窗口、菜单组件或为枚举数据类型时才有效 13=成员数量,14=成员数据数组
    
/*000*/ { "网络服务器", "socket_server", NULL,
            s_dtCmdIndexsock_socket_server_static_var_count_00, s_dtCmdIndexsock_socket_server_static_var_00, _DT_OS(__OS_WIN),
            0, NULL, NULL,
            NULL, NULL,
            NULL,
            s_objEventsock_socket_server_static_var_count_00, s_objEventsock_socket_server_static_var_00
        },
/*001*/ { "网络客户端", "socket_client", NULL,
            s_dtCmdIndexsock_socket_client_static_var_count_01, s_dtCmdIndexsock_socket_client_static_var_01, _DT_OS(__OS_WIN),
            0, NULL, NULL,
            NULL, NULL,
            NULL,
            s_objEventsock_socket_client_static_var_count_01, s_objEventsock_socket_client_static_var_01
        },
/*002*/ { "网络数据报", "socket_udp", NULL,
            s_dtCmdIndexsock_socket_udp_static_var_count_02, s_dtCmdIndexsock_socket_udp_static_var_02, _DT_OS(__OS_WIN),
            0, NULL, NULL,
            NULL, NULL,
            NULL,
            s_objEventsock_socket_udp_static_var_count_02, s_objEventsock_socket_udp_static_var_02
        },
/*003*/ { "对方信息", "halve_info", NULL,
            NULL, NULL, _DT_OS(__OS_WIN),
            0, NULL, NULL,
            NULL, NULL,
            NULL,
            s_objEventsock_halve_info_static_var_count_03, s_objEventsock_halve_info_static_var_03
        },
};
int g_DataType_sock_global_var_count = sizeof(g_DataType_sock_global_var) / sizeof(g_DataType_sock_global_var[0]);

#endif

