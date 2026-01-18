#ifndef __E_STATIC_LIB
#include "include_pop3_header.h"

// 邮件信息  下的方法索引
static INT s_dtCmdIndexpop3_MailInfo_static_var_00[] = 
{
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 
    21, 22, 23, 26, 
};
static int s_dtCmdIndexpop3_MailInfo_static_var_count_00 = sizeof(s_dtCmdIndexpop3_MailInfo_static_var_00) / sizeof(s_dtCmdIndexpop3_MailInfo_static_var_00[0]);

// 附件信息  下的方法索引
static INT s_dtCmdIndexpop3_AttachmentInfo_static_var_01[] = 
{
    16, 17, 18, 19, 20, 27, 28, 
};
static int s_dtCmdIndexpop3_AttachmentInfo_static_var_count_01 = sizeof(s_dtCmdIndexpop3_AttachmentInfo_static_var_01) / sizeof(s_dtCmdIndexpop3_AttachmentInfo_static_var_01[0]);

// 邮件信息  下的枚举常量成员数组
static LIB_DATA_TYPE_ELEMENT s_objEventpop3_MailInfo_static_var_00[] = 
{
    /*000*/ {SDT_TEXT, 0, "发件人地址指针", "SenderAddress", NULL, LES_HIDED, (INT)0x00000000},
    /*001*/ {SDT_TEXT, 0, "主题", "Subject", NULL, LES_HIDED, (INT)0x00000000},
    /*002*/ {SDT_DATE_TIME, 0, "日期", "DataTime", NULL, LES_HIDED, (INT)0x00000000},
    /*003*/ {SDT_INT, 0, "大小", "Size", "单位为字节", LES_HIDED, (INT)0x00000000},
    /*004*/ {SDT_TEXT, 0, "文本内容", "TextContent", NULL, LES_HIDED, (INT)0x00000000},
    /*005*/ {SDT_INT, 0, "附件个数", "AttachmentNumber", NULL, LES_HIDED, (INT)0x00000000},
    /*006*/ {MAKELONG(0x02, 0), 0, "附件信息", "Attachment", NULL, LES_HIDED, (INT)0x00000000},
    /*007*/ {SDT_TEXT, 0, "发件人姓名", "SenderName", NULL, LES_HIDED, (INT)0x00000000},
    /*008*/ {SDT_TEXT, 0, "回复地址", "ReplyAddress", NULL, LES_HIDED, (INT)0x00000000},
    /*009*/ {SDT_TEXT, 0, "原始信息", "OriMessage", NULL, LES_HIDED, (INT)0x00000000},
    /*010*/ {SDT_TEXT, 0, "超文本内容", "HTMLContent", NULL, LES_HIDED, (INT)0x00000000},

};
static int s_objEventpop3_MailInfo_static_var_count_00 = sizeof(s_objEventpop3_MailInfo_static_var_00) / sizeof(s_objEventpop3_MailInfo_static_var_00[0]);

// 附件信息  下的枚举常量成员数组
static LIB_DATA_TYPE_ELEMENT s_objEventpop3_AttachmentInfo_static_var_01[] = 
{
    /*000*/ {SDT_INT, 0, "大小", "Size", NULL, LES_HIDED, (INT)0x00000000},
    /*001*/ {SDT_TEXT, 0, "类型", "type", NULL, LES_HIDED, (INT)0x00000000},
    /*002*/ {SDT_TEXT, 0, "文件名", "filename", NULL, LES_HIDED, (INT)0x00000000},
    /*003*/ {SDT_TEXT, 0, "编码方式", "encode", NULL, LES_HIDED, (INT)0x00000000},
    /*004*/ {SDT_BIN, 0, "数据", "data", NULL, LES_HIDED, (INT)0x00000000},
    /*005*/ {SDT_BOOL, 0, "是否嵌入附件", "IsRelate", "一般表示此附件是超文本内容里的图片，声音等", LES_HIDED, (INT)0x00000000},
    /*006*/ {SDT_TEXT, 0, "名称", "Name", "一般表示此附件是超文本内容里的图片，声音等的文件名", LES_HIDED, (INT)0x00000000},

};
static int s_objEventpop3_AttachmentInfo_static_var_count_01 = sizeof(s_objEventpop3_AttachmentInfo_static_var_01) / sizeof(s_objEventpop3_AttachmentInfo_static_var_01[0]);

// 命令类型  下的枚举常量成员数组
static LIB_DATA_TYPE_ELEMENT s_objEventpop3_CmdType_static_var_02[] = 
{
    /*000*/ {SDT_INT, 0, "等待接收数据", NULL, NULL, LES_HAS_DEFAULT_VALUE, (INT)0x00000000},
    /*001*/ {SDT_INT, 0, "验证用户名", NULL, NULL, LES_HAS_DEFAULT_VALUE, (INT)0x00000001},
    /*002*/ {SDT_INT, 0, "验证口令", NULL, NULL, LES_HAS_DEFAULT_VALUE, (INT)0x00000002},
    /*003*/ {SDT_INT, 0, "接收邮件", NULL, NULL, LES_HAS_DEFAULT_VALUE, (INT)0x00000003},
    /*004*/ {SDT_INT, 0, "获取邮件信息", NULL, NULL, LES_HAS_DEFAULT_VALUE, (INT)0x00000004},
    /*005*/ {SDT_INT, 0, "获取邮件大小", NULL, NULL, LES_HAS_DEFAULT_VALUE, (INT)0x00000005},
    /*006*/ {SDT_INT, 0, "删除邮件", NULL, NULL, LES_HAS_DEFAULT_VALUE, (INT)0x00000006},
    /*007*/ {SDT_INT, 0, "重置邮件", NULL, NULL, LES_HAS_DEFAULT_VALUE, (INT)0x00000007},
    /*008*/ {SDT_INT, 0, "获取邮件序号", NULL, NULL, LES_HAS_DEFAULT_VALUE, (INT)0x00000008},
    /*009*/ {SDT_INT, 0, "接收邮件前几行", NULL, NULL, LES_HAS_DEFAULT_VALUE, (INT)0x00000009},
    /*010*/ {SDT_INT, 0, "其它", NULL, NULL, LES_HAS_DEFAULT_VALUE, (INT)0x0000000A},

};
static int s_objEventpop3_CmdType_static_var_count_02 = sizeof(s_objEventpop3_CmdType_static_var_02) / sizeof(s_objEventpop3_CmdType_static_var_02[0]);

LIB_DATA_TYPE_INFO g_DataType_pop3_global_var[] = 
{
    //1=中文名字,2=英文名字,3=详细解释,4=命令数量,5=索引值,6=标志 LDT_
    //类型为窗口或菜单组件时有效 7=资源ID,8=事件数量,9=组件事件数组,10=属性数  11=属性数组 12=组件交互子程序
    //不为窗口、菜单组件或为枚举数据类型时才有效 13=成员数量,14=成员数据数组
    
/*000*/ { "邮件信息", "MailInfo", NULL,
            s_dtCmdIndexpop3_MailInfo_static_var_count_00, s_dtCmdIndexpop3_MailInfo_static_var_00, _DT_OS(__OS_WIN),
            0, NULL, NULL,
            NULL, NULL,
            NULL,
            s_objEventpop3_MailInfo_static_var_count_00, s_objEventpop3_MailInfo_static_var_00
        },
/*001*/ { "附件信息", "AttachmentInfo", NULL,
            s_dtCmdIndexpop3_AttachmentInfo_static_var_count_01, s_dtCmdIndexpop3_AttachmentInfo_static_var_01, _DT_OS(__OS_WIN),
            0, NULL, NULL,
            NULL, NULL,
            NULL,
            s_objEventpop3_AttachmentInfo_static_var_count_01, s_objEventpop3_AttachmentInfo_static_var_01
        },
/*002*/ { "命令类型", "CmdType", "本数据类型包括了接收邮件命令的常量",
            NULL, NULL, _DT_OS(__OS_WIN) | LDT_ENUM,
            0, NULL, NULL,
            NULL, NULL,
            NULL,
            s_objEventpop3_CmdType_static_var_count_02, s_objEventpop3_CmdType_static_var_02
        },
};
int g_DataType_pop3_global_var_count = sizeof(g_DataType_pop3_global_var) / sizeof(g_DataType_pop3_global_var[0]);

#endif

