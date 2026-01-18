#ifndef __E_STATIC_LIB
#include "include_einterprocess_header.h"

// 邮槽服务器  下的方法索引
static INT s_dtCmdIndexeinterprocess_MailslotServer_static_var_00[] = 
{
    7, 8, 9, 10, 11, 
};
static int s_dtCmdIndexeinterprocess_MailslotServer_static_var_count_00 = sizeof(s_dtCmdIndexeinterprocess_MailslotServer_static_var_00) / sizeof(s_dtCmdIndexeinterprocess_MailslotServer_static_var_00[0]);

// 邮槽客户机  下的方法索引
static INT s_dtCmdIndexeinterprocess_MailslotClient_static_var_01[] = 
{
    12, 13, 14, 15, 16, 
};
static int s_dtCmdIndexeinterprocess_MailslotClient_static_var_count_01 = sizeof(s_dtCmdIndexeinterprocess_MailslotClient_static_var_01) / sizeof(s_dtCmdIndexeinterprocess_MailslotClient_static_var_01[0]);

// 内存映射文件  下的方法索引
static INT s_dtCmdIndexeinterprocess_MapFile_static_var_02[] = 
{
    17, 18, 19, 20, 21, 22, 23, 24, 25, 
};
static int s_dtCmdIndexeinterprocess_MapFile_static_var_count_02 = sizeof(s_dtCmdIndexeinterprocess_MapFile_static_var_02) / sizeof(s_dtCmdIndexeinterprocess_MapFile_static_var_02[0]);

// 邮槽服务器  下的枚举常量成员数组
static LIB_DATA_TYPE_ELEMENT s_objEventeinterprocess_MailslotServer_static_var_00[] = 
{
    /*000*/ {SDT_INT, 0, "邮槽服务器句柄", "MailslotServer", "无", LES_HIDED, (INT)0x00000000},

};
static int s_objEventeinterprocess_MailslotServer_static_var_count_00 = sizeof(s_objEventeinterprocess_MailslotServer_static_var_00) / sizeof(s_objEventeinterprocess_MailslotServer_static_var_00[0]);

// 邮槽客户机  下的枚举常量成员数组
static LIB_DATA_TYPE_ELEMENT s_objEventeinterprocess_MailslotClient_static_var_01[] = 
{
    /*000*/ {SDT_INT, 0, "邮槽客户机句柄", "MailslotClient", "无", LES_HIDED, (INT)0x00000000},

};
static int s_objEventeinterprocess_MailslotClient_static_var_count_01 = sizeof(s_objEventeinterprocess_MailslotClient_static_var_01) / sizeof(s_objEventeinterprocess_MailslotClient_static_var_01[0]);

// 内存映射文件  下的枚举常量成员数组
static LIB_DATA_TYPE_ELEMENT s_objEventeinterprocess_MapFile_static_var_02[] = 
{
    /*000*/ {SDT_INT, 0, "映射文件指针", "MapFileData", "无", LES_HIDED, (INT)0x00000000},

};
static int s_objEventeinterprocess_MapFile_static_var_count_02 = sizeof(s_objEventeinterprocess_MapFile_static_var_02) / sizeof(s_objEventeinterprocess_MapFile_static_var_02[0]);

LIB_DATA_TYPE_INFO g_DataType_einterprocess_global_var[] = 
{
    //1=中文名字,2=英文名字,3=详细解释,4=命令数量,5=索引值,6=标志 LDT_
    //类型为窗口或菜单组件时有效 7=资源ID,8=事件数量,9=组件事件数组,10=属性数  11=属性数组 12=组件交互子程序
    //不为窗口、菜单组件或为枚举数据类型时才有效 13=成员数量,14=成员数据数组
    
/*000*/ { "邮槽服务器", "MailslotServer", "该数据类型提供对邮槽服务器的创建，读写操作的支持",
            s_dtCmdIndexeinterprocess_MailslotServer_static_var_count_00, s_dtCmdIndexeinterprocess_MailslotServer_static_var_00, _DT_OS(__OS_WIN),
            0, NULL, NULL,
            NULL, NULL,
            NULL,
            s_objEventeinterprocess_MailslotServer_static_var_count_00, s_objEventeinterprocess_MailslotServer_static_var_00
        },
/*001*/ { "邮槽客户机", "MailslotClient", "邮槽是一种简单的单向“进程间通信”机制，在两个进程之间建立一种不可靠的单向数据通信。在同一台计算机的不同进程之间，或在跨越整个网络的不同计算机的进程之间，协助进行消息的传输。邮槽的数据通讯方式是单向的，只能从客户机以数据报的形式广播给一个或者多个服务器。在Win9x下使用要注意一个问题:假如在一个读请求尚未完成的前提下，服务器应用突然中止运行，应用程序便会被永远地“挂起”，或者“凝结”。要想取消，唯一的办法就是重新启动Windows。为解决这个问题，可让服务器在单独一个线程中读取数据",
            s_dtCmdIndexeinterprocess_MailslotClient_static_var_count_01, s_dtCmdIndexeinterprocess_MailslotClient_static_var_01, _DT_OS(__OS_WIN),
            0, NULL, NULL,
            NULL, NULL,
            NULL,
            s_objEventeinterprocess_MailslotClient_static_var_count_01, s_objEventeinterprocess_MailslotClient_static_var_01
        },
/*002*/ { "内存映射文件", "MapFile", "该数据类型提供对内存映射文件的创建，读写操作的支持",
            s_dtCmdIndexeinterprocess_MapFile_static_var_count_02, s_dtCmdIndexeinterprocess_MapFile_static_var_02, _DT_OS(__OS_WIN),
            0, NULL, NULL,
            NULL, NULL,
            NULL,
            s_objEventeinterprocess_MapFile_static_var_count_02, s_objEventeinterprocess_MapFile_static_var_02
        },
};
int g_DataType_einterprocess_global_var_count = sizeof(g_DataType_einterprocess_global_var) / sizeof(g_DataType_einterprocess_global_var[0]);

#endif

