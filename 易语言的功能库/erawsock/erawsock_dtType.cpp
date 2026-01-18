#ifndef __E_STATIC_LIB
#include "include_erawsock_header.h"

// 原始套接字  下的方法索引
static INT s_dtCmdIndexerawsock_RawSocket_static_var_00[] = 
{
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 
};
static int s_dtCmdIndexerawsock_RawSocket_static_var_count_00 = sizeof(s_dtCmdIndexerawsock_RawSocket_static_var_00) / sizeof(s_dtCmdIndexerawsock_RawSocket_static_var_00[0]);

// ARP协议  下的方法索引
static INT s_dtCmdIndexerawsock_ARP_static_var_01[] = 
{
    10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 
};
static int s_dtCmdIndexerawsock_ARP_static_var_count_01 = sizeof(s_dtCmdIndexerawsock_ARP_static_var_01) / sizeof(s_dtCmdIndexerawsock_ARP_static_var_01[0]);

// IP报头  下的枚举常量成员数组
static LIB_DATA_TYPE_ELEMENT s_objEventerawsock_IPHeader_static_var_02[] = 
{
    /*000*/ {SDT_BYTE, 0, "首部长度及版本号", "h_lenver", "4位首部长度 + 4位IP版本号", NULL, (INT)0x00000000},
    /*001*/ {SDT_BYTE, 0, "服务类型", "tos", NULL, NULL, (INT)0x00000000},
    /*002*/ {SDT_SHORT, 0, "总长度", "total_len", NULL, NULL, (INT)0x00000000},
    /*003*/ {SDT_SHORT, 0, "标识", "ident", NULL, LES_HAS_DEFAULT_VALUE, (INT)0x00000001},
    /*004*/ {SDT_SHORT, 0, "标志位", "frag_and_flags", NULL, NULL, (INT)0x00000000},
    /*005*/ {SDT_BYTE, 0, "生存时间", "ttl", NULL, LES_HAS_DEFAULT_VALUE, (INT)0x00000080},
    /*006*/ {SDT_BYTE, 0, "协议", "proto", NULL, LES_HAS_DEFAULT_VALUE, (INT)0x00000006},
    /*007*/ {SDT_SHORT, 0, "IP首部校验和", "checksum", NULL, NULL, (INT)0x00000000},
    /*008*/ {SDT_TEXT, 0, "源IP地址", "sourceIP", NULL, NULL, (INT)0x00000000},
    /*009*/ {SDT_TEXT, 0, "目的IP地址", "destIP", NULL, NULL, (INT)0x00000000},
    /*010*/ {SDT_INT, 0, "选项和填充", "options", NULL, NULL, (INT)0x00000000},

};
static int s_objEventerawsock_IPHeader_static_var_count_02 = sizeof(s_objEventerawsock_IPHeader_static_var_02) / sizeof(s_objEventerawsock_IPHeader_static_var_02[0]);

// TCP报头  下的枚举常量成员数组
static LIB_DATA_TYPE_ELEMENT s_objEventerawsock_TCPHeader_static_var_03[] = 
{
    /*000*/ {SDT_SHORT, 0, "源端口", "th_sport", NULL, NULL, (INT)0x00000000},
    /*001*/ {SDT_SHORT, 0, "目的端口", "th_dport", NULL, NULL, (INT)0x00000000},
    /*002*/ {SDT_INT, 0, "序列号", "th_seq", NULL, NULL, (INT)0x00000000},
    /*003*/ {SDT_INT, 0, "确认号", "th_ack", NULL, NULL, (INT)0x00000000},
    /*004*/ {SDT_BYTE, 0, "首部长度及保留字", "th_lenres", NULL, NULL, (INT)0x00000000},
    /*005*/ {SDT_BYTE, 0, "标志位", "th_flag", "实现不同的标志位探测，2是SYN，1是FIN，16是ACK探测 等等", NULL, (INT)0x00000000},
    /*006*/ {SDT_SHORT, 0, "窗口大小", "th_win", NULL, NULL, (INT)0x00000000},
    /*007*/ {SDT_SHORT, 0, "校验和", "th_sum", NULL, NULL, (INT)0x00000000},
    /*008*/ {SDT_SHORT, 0, "紧急数据偏移量", "th_urp", NULL, NULL, (INT)0x00000000},
    /*009*/ {SDT_INT, 0, "选项和填充", "th_options", NULL, NULL, (INT)0x00000000},

};
static int s_objEventerawsock_TCPHeader_static_var_count_03 = sizeof(s_objEventerawsock_TCPHeader_static_var_03) / sizeof(s_objEventerawsock_TCPHeader_static_var_03[0]);

// ICMP报头  下的枚举常量成员数组
static LIB_DATA_TYPE_ELEMENT s_objEventerawsock_ICMPHeader_static_var_04[] = 
{
    /*000*/ {SDT_BYTE, 0, "ICMP类型", "type", "0->回送应答  8->回送请求", LES_HAS_DEFAULT_VALUE, (INT)0x00000008},
    /*001*/ {SDT_BYTE, 0, "子类型码", "code", "保存与特定ICMP 报文类型相关的细节信息", NULL, (INT)0x00000000},
    /*002*/ {SDT_SHORT, 0, "ICMP校验和", "checksum", NULL, NULL, (INT)0x00000000},
    /*003*/ {SDT_SHORT, 0, "序号", "seq", NULL, NULL, (INT)0x00000000},
    /*004*/ {SDT_SHORT, 0, "标识符", "id", NULL, LES_HAS_DEFAULT_VALUE, (INT)0x00000001},

};
static int s_objEventerawsock_ICMPHeader_static_var_count_04 = sizeof(s_objEventerawsock_ICMPHeader_static_var_04) / sizeof(s_objEventerawsock_ICMPHeader_static_var_04[0]);

// 以太网头  下的枚举常量成员数组
static LIB_DATA_TYPE_ELEMENT s_objEventerawsock_ETHHeader_static_var_05[] = 
{
    /*000*/ {SDT_TEXT, 0, "以太网目的地址", "eh_dst", "当目的地址为ffffffffffff时的地址是广播地址，即所有的以太网接口都要接收广播数据帧。", NULL, (INT)0x00000000},
    /*001*/ {SDT_TEXT, 0, "以太网源地址", "eh_src", NULL, NULL, (INT)0x00000000},
    /*002*/ {SDT_SHORT, 0, "帧类型", "eh_type", "帧类型表示后面数据的类型，对于ARP请求和应答来说，该字段的值是0x0806。", LES_HAS_DEFAULT_VALUE, (INT)0x00000806},

};
static int s_objEventerawsock_ETHHeader_static_var_count_05 = sizeof(s_objEventerawsock_ETHHeader_static_var_05) / sizeof(s_objEventerawsock_ETHHeader_static_var_05[0]);

// ARP头  下的枚举常量成员数组
static LIB_DATA_TYPE_ELEMENT s_objEventerawsock_ARPHeader_static_var_06[] = 
{
    /*000*/ {SDT_SHORT, 0, "硬件类型", "hw_type", "表示硬件地址类型，以太网地址的值为1。", LES_HAS_DEFAULT_VALUE, (INT)0x00000001},
    /*001*/ {SDT_SHORT, 0, "协议类型", "prot_type", "表示要映射的协议地址类型，它的值为0x0800表示IP地址。", LES_HAS_DEFAULT_VALUE, (INT)0x00000800},
    /*002*/ {SDT_BYTE, 0, "硬件地址长度", "hw_addr_size", "以太网为6字节的硬件地址。", LES_HAS_DEFAULT_VALUE, (INT)0x00000006},
    /*003*/ {SDT_BYTE, 0, "协议地址长度", "prot_addr_size", "IP协议为4字节。", LES_HAS_DEFAULT_VALUE, (INT)0x00000004},
    /*004*/ {SDT_SHORT, 0, "操作类型", "opt", "1表示ARP请求，2表示ARP应答", LES_HAS_DEFAULT_VALUE, (INT)0x00000001},
    /*005*/ {SDT_TEXT, 0, "发送端以太网地址", "src_hw_addr", NULL, NULL, (INT)0x00000000},
    /*006*/ {SDT_TEXT, 0, "发送端IP地址", "src_ip_addr", NULL, NULL, (INT)0x00000000},
    /*007*/ {SDT_TEXT, 0, "目的以太网地址", "des_hw_addr", NULL, NULL, (INT)0x00000000},
    /*008*/ {SDT_TEXT, 0, "目的IP地址", "des_ip_addr", NULL, NULL, (INT)0x00000000},

};
static int s_objEventerawsock_ARPHeader_static_var_count_06 = sizeof(s_objEventerawsock_ARPHeader_static_var_06) / sizeof(s_objEventerawsock_ARPHeader_static_var_06[0]);

// 协议类型常量  下的枚举常量成员数组
static LIB_DATA_TYPE_ELEMENT s_objEventerawsock_protocol_static_var_07[] = 
{
    /*000*/ {SDT_INT, 0, "IP协议", "IPPROTO_IP", NULL, LES_HAS_DEFAULT_VALUE, (INT)0x00000000},
    /*001*/ {SDT_INT, 0, "ICMP协议", "IPPROTO_ICMP", NULL, LES_HAS_DEFAULT_VALUE, (INT)0x00000001},
    /*002*/ {SDT_INT, 0, "IGMP协议", "IPPROTO_IGMP", NULL, LES_HAS_DEFAULT_VALUE, (INT)0x00000002},
    /*003*/ {SDT_INT, 0, "GGP协议", "IPPROTO_GGP", NULL, LES_HAS_DEFAULT_VALUE, (INT)0x00000003},
    /*004*/ {SDT_INT, 0, "TCP协议", "IPPROTO_TCP", NULL, LES_HAS_DEFAULT_VALUE, (INT)0x00000006},
    /*005*/ {SDT_INT, 0, "PUP协议", "IPPROTO_PUP", NULL, LES_HAS_DEFAULT_VALUE, (INT)0x0000000C},
    /*006*/ {SDT_INT, 0, "UDP协议", "IPPROTO_UDP", NULL, LES_HAS_DEFAULT_VALUE, (INT)0x00000011},
    /*007*/ {SDT_INT, 0, "IDP协议", "IPPROTO_IDP", NULL, LES_HAS_DEFAULT_VALUE, (INT)0x00000016},
    /*008*/ {SDT_INT, 0, "ND协议", "IPPROTO_ND", NULL, LES_HAS_DEFAULT_VALUE, (INT)0x0000004D},
    /*009*/ {SDT_INT, 0, "RAW协议", "IPPROTO_RAW", NULL, LES_HAS_DEFAULT_VALUE, (INT)0x000000FF},

};
static int s_objEventerawsock_protocol_static_var_count_07 = sizeof(s_objEventerawsock_protocol_static_var_07) / sizeof(s_objEventerawsock_protocol_static_var_07[0]);

// ARP常量  下的枚举常量成员数组
static LIB_DATA_TYPE_ELEMENT s_objEventerawsock_ArpConst_static_var_08[] = 
{
    /*000*/ {SDT_INT, 0, "IP协议", "IP_PROTO_TYPE", NULL, LES_HAS_DEFAULT_VALUE, (INT)0x00000800},
    /*001*/ {SDT_INT, 0, "ARP协议", "ARP_FRAME_TYPE", NULL, LES_HAS_DEFAULT_VALUE, (INT)0x00000806},
    /*002*/ {SDT_INT, 0, "ARP请求", "ARP_REQUEST", NULL, LES_HAS_DEFAULT_VALUE, (INT)0x00000001},
    /*003*/ {SDT_INT, 0, "ARP应答", "ARP_REPLY", NULL, LES_HAS_DEFAULT_VALUE, (INT)0x00000002},
    /*004*/ {SDT_INT, 0, "直接模式", "NDIS_PACKET_TYPE_DIRECTED", NULL, LES_HAS_DEFAULT_VALUE, (INT)0x00000001},
    /*005*/ {SDT_INT, 0, "混杂模式", "NDIS_PACKET_TYPE_PROMISCUOUS", NULL, LES_HAS_DEFAULT_VALUE, (INT)0x00000002},

};
static int s_objEventerawsock_ArpConst_static_var_count_08 = sizeof(s_objEventerawsock_ArpConst_static_var_08) / sizeof(s_objEventerawsock_ArpConst_static_var_08[0]);

LIB_DATA_TYPE_INFO g_DataType_erawsock_global_var[] = 
{
    //1=中文名字,2=英文名字,3=详细解释,4=命令数量,5=索引值,6=标志 LDT_
    //类型为窗口或菜单组件时有效 7=资源ID,8=事件数量,9=组件事件数组,10=属性数  11=属性数组 12=组件交互子程序
    //不为窗口、菜单组件或为枚举数据类型时才有效 13=成员数量,14=成员数据数组
    
/*000*/ { "原始套接字", "RawSocket", NULL,
            s_dtCmdIndexerawsock_RawSocket_static_var_count_00, s_dtCmdIndexerawsock_RawSocket_static_var_00, _DT_OS(__OS_WIN),
            0, NULL, NULL,
            NULL, NULL,
            NULL,
            NULL, NULL
        },
/*001*/ { "ARP协议", "ARP", "为了正常使用ARP协议的命令，请先安装WinPcap 3.1驱动。",
            s_dtCmdIndexerawsock_ARP_static_var_count_01, s_dtCmdIndexerawsock_ARP_static_var_01, _DT_OS(__OS_WIN),
            0, NULL, NULL,
            NULL, NULL,
            NULL,
            NULL, NULL
        },
/*002*/ { "IP报头", "IPHeader", NULL,
            NULL, NULL, _DT_OS(__OS_WIN),
            0, NULL, NULL,
            NULL, NULL,
            NULL,
            s_objEventerawsock_IPHeader_static_var_count_02, s_objEventerawsock_IPHeader_static_var_02
        },
/*003*/ { "TCP报头", "TCPHeader", NULL,
            NULL, NULL, _DT_OS(__OS_WIN),
            0, NULL, NULL,
            NULL, NULL,
            NULL,
            s_objEventerawsock_TCPHeader_static_var_count_03, s_objEventerawsock_TCPHeader_static_var_03
        },
/*004*/ { "ICMP报头", "ICMPHeader", NULL,
            NULL, NULL, _DT_OS(__OS_WIN),
            0, NULL, NULL,
            NULL, NULL,
            NULL,
            s_objEventerawsock_ICMPHeader_static_var_count_04, s_objEventerawsock_ICMPHeader_static_var_04
        },
/*005*/ { "以太网头", "ETHHeader", NULL,
            NULL, NULL, _DT_OS(__OS_WIN),
            0, NULL, NULL,
            NULL, NULL,
            NULL,
            s_objEventerawsock_ETHHeader_static_var_count_05, s_objEventerawsock_ETHHeader_static_var_05
        },
/*006*/ { "ARP头", "ARPHeader", NULL,
            NULL, NULL, _DT_OS(__OS_WIN),
            0, NULL, NULL,
            NULL, NULL,
            NULL,
            s_objEventerawsock_ARPHeader_static_var_count_06, s_objEventerawsock_ARPHeader_static_var_06
        },
/*007*/ { "协议类型常量", "protocol", "本对象枚举了所有用于指定套接字时候的协议。",
            NULL, NULL, _DT_OS(__OS_WIN) | LDT_ENUM,
            0, NULL, NULL,
            NULL, NULL,
            NULL,
            s_objEventerawsock_protocol_static_var_count_07, s_objEventerawsock_protocol_static_var_07
        },
/*008*/ { "ARP常量", "ArpConst", "本对象枚举了所有用于ARP操作的常量。",
            NULL, NULL, _DT_OS(__OS_WIN) | LDT_ENUM,
            0, NULL, NULL,
            NULL, NULL,
            NULL,
            s_objEventerawsock_ArpConst_static_var_count_08, s_objEventerawsock_ArpConst_static_var_08
        },
};
int g_DataType_erawsock_global_var_count = sizeof(g_DataType_erawsock_global_var) / sizeof(g_DataType_erawsock_global_var[0]);

#endif

