#ifndef __E_STATIC_LIB
#include "include_regex_header.h"

// 正则表达式  下的方法索引
static INT s_dtCmdIndexregex_regex_static_var_00[] = 
{
    1, 2, 3, 4, 5, 6, 13, 0, 7, 15, 
    16, 17, 
};
static int s_dtCmdIndexregex_regex_static_var_count_00 = sizeof(s_dtCmdIndexregex_regex_static_var_00) / sizeof(s_dtCmdIndexregex_regex_static_var_00[0]);

// 搜索结果  下的方法索引
static INT s_dtCmdIndexregex_SearchResult_static_var_01[] = 
{
    8, 9, 10, 11, 12, 14, 
};
static int s_dtCmdIndexregex_SearchResult_static_var_count_01 = sizeof(s_dtCmdIndexregex_SearchResult_static_var_01) / sizeof(s_dtCmdIndexregex_SearchResult_static_var_01[0]);

// 正则表达式  下的枚举常量成员数组
static LIB_DATA_TYPE_ELEMENT s_objEventregex_regex_static_var_00[] = 
{
    /*000*/ {SDT_INT, 0, "this", NULL, NULL, LES_HIDED, (INT)0x00000000},
    /*001*/ {SDT_INT, 0, "pattern", NULL, NULL, LES_HIDED, (INT)0x00000000},
    /*002*/ {SDT_BOOL, 0, "多行模式", "MultiLine", "指定在搜索、替换时，是否使用多行模式。\r\n    如果本成员值为真，字符“^”不仅匹配目标文本的开头，而且匹配目标文本中换行符（字符(10)，字符(13)或 #换行符）的后面；字符“$”不仅匹配目标文本的结尾，而且匹配目标文本中换行符的前面。否则字符“^”仅仅匹配目标文本的开头，“$”也仅仅匹配目标文本的结尾", LES_HAS_DEFAULT_VALUE, (INT)0x00000001},

};
static int s_objEventregex_regex_static_var_count_00 = sizeof(s_objEventregex_regex_static_var_00) / sizeof(s_objEventregex_regex_static_var_00[0]);

// 搜索结果  下的枚举常量成员数组
static LIB_DATA_TYPE_ELEMENT s_objEventregex_SearchResult_static_var_01[] = 
{
    /*000*/ {SDT_INT, 0, "this", NULL, NULL, LES_HIDED, (INT)0x00000000},

};
static int s_objEventregex_SearchResult_static_var_count_01 = sizeof(s_objEventregex_SearchResult_static_var_01) / sizeof(s_objEventregex_SearchResult_static_var_01[0]);

LIB_DATA_TYPE_INFO g_DataType_regex_global_var[] = 
{
    //1=中文名字,2=英文名字,3=详细解释,4=命令数量,5=索引值,6=标志 LDT_
    //类型为窗口或菜单组件时有效 7=资源ID,8=事件数量,9=组件事件数组,10=属性数  11=属性数组 12=组件交互子程序
    //不为窗口、菜单组件或为枚举数据类型时才有效 13=成员数量,14=成员数据数组
    
/*000*/ { "正则表达式", "regex", "一个正则表达式（regular expression）描述了一种文本匹配的模式，可以被用来检查一段文本是否符合某种特定的格式（匹配模式）或在一段文本中寻找、提取特定格式的子文本等。比如校验一段文本是否为有效的URL地址、从文本文件中提取所包含的E-Mail地址（请参考本支持库附带的例程）。关于正则表达式的更多知识，请查阅相关的书籍、资料。",
            s_dtCmdIndexregex_regex_static_var_count_00, s_dtCmdIndexregex_regex_static_var_00, _DT_OS(__OS_WIN),
            0, NULL, NULL,
            NULL, NULL,
            NULL,
            s_objEventregex_regex_static_var_count_00, s_objEventregex_regex_static_var_00
        },
/*001*/ { "搜索结果", "SearchResult", "该类型的变量一般用来接收和分析“正则表达式.搜索（）”的返回值",
            s_dtCmdIndexregex_SearchResult_static_var_count_01, s_dtCmdIndexregex_SearchResult_static_var_01, _DT_OS(__OS_WIN),
            0, NULL, NULL,
            NULL, NULL,
            NULL,
            s_objEventregex_SearchResult_static_var_count_01, s_objEventregex_SearchResult_static_var_01
        },
};
int g_DataType_regex_global_var_count = sizeof(g_DataType_regex_global_var) / sizeof(g_DataType_regex_global_var[0]);

#endif

