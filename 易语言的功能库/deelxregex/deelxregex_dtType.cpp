#ifndef __E_STATIC_LIB
#include "include_deelxregex_header.h"

// 正则表达式DEELX  下的方法索引
static INT s_dtCmdIndexdeelxregex_DeelxRegex_static_var_00[] = 
{
    0, 1, 2, 26, 5, 27, 14, 12, 35, 36, 
    37, 13, 15, 30, 38, 39, 16, 17, 31, 18, 
    32, 23, 28, 24, 25, 29, 40, 41, 42, 
};
static int s_dtCmdIndexdeelxregex_DeelxRegex_static_var_count_00 = sizeof(s_dtCmdIndexdeelxregex_DeelxRegex_static_var_00) / sizeof(s_dtCmdIndexdeelxregex_DeelxRegex_static_var_00[0]);

// 搜索结果DEELX  下的方法索引
static INT s_dtCmdIndexdeelxregex_DeelxSearchResult_static_var_01[] = 
{
    3, 4, 6, 22, 7, 8, 9, 10, 11, 19, 
    20, 33, 21, 34, 43, 
};
static int s_dtCmdIndexdeelxregex_DeelxSearchResult_static_var_count_01 = sizeof(s_dtCmdIndexdeelxregex_DeelxSearchResult_static_var_01) / sizeof(s_dtCmdIndexdeelxregex_DeelxSearchResult_static_var_01[0]);

// 正则常量  下的枚举常量成员数组
static LIB_DATA_TYPE_ELEMENT s_objEventdeelxregex_DeelxConst_static_var_05[] = 
{
    /*000*/ {SDT_INT, 0, "单行模式", "SINGLELINE", "使小数点 \".\" 可以匹配包含换行符（\\n）在内的任意字符。默认情况下，小数点只匹配换行符以外的任意字符，不匹配换行符。", LES_HAS_DEFAULT_VALUE, (INT)0x00000001},
    /*001*/ {SDT_INT, 0, "多行模式", "MULTILINE", "使 ^ 符号除了能够匹配字符串开始位置外，还能匹配换行符（\\n）之后的位置；使 $ 符号除了能够匹配字符串结束位置外，还能匹配换行符之前的位置。\r\n    默认情况下， ^ 符号只能匹配字符串开始位置， $ 符号只能匹配字符串结束位置。\r\n    单行模式(SINGLELINE) 和 多行模式(MULTILINE) 虽然听起来相互矛盾，但却是作用在不同的地方。因此它们是可以组合使用的。\r\n    在指定了 多行模式(MULTILINE) 之后，如果需要仅匹配字符串开始和结束位置，可以使用 \\A 和 \\Z。", LES_HAS_DEFAULT_VALUE, (INT)0x00000002},
    /*002*/ {SDT_INT, 0, "全局模式", "GLOBAL", "使 \\G 可以用来匹配本次查找匹配的开始位置，对于连续的匹配来说，也就是上次匹配的结束位置。默认情况下， \\G 没有作用。", LES_HAS_DEFAULT_VALUE, (INT)0x00000004},
    /*003*/ {SDT_INT, 0, "忽略大小写", "IGNORECASE", "匹配时忽略大小写。默认情况下，正则表达式是要区分大小写的。不管是否指定忽略大小写模式，字符类，比如 [A-Z] 是要区分大小写的。", LES_HAS_DEFAULT_VALUE, (INT)0x00000008},
    /*004*/ {SDT_INT, 0, "从右向左", "RIGHTTOLEFT", "从右向左的进行匹配。从被匹配字符串的结束位置向前进行查找匹配，同时，在表达式中也是右侧的表达式先进行匹配。", LES_HAS_DEFAULT_VALUE, (INT)0x00000010},
    /*005*/ {SDT_INT, 0, "扩展模式", "EXTENDED", "使 本正则支持库忽略表达式中的空白字符，并且把从 # 开始到该行行末的内容视为注释。默认情况下，正则表达式中的空格，换行等字符将可以匹配相应的字符。指定了 扩展模式(EXTENDED) 模式后，如果要在正则表达式中表示空白字符比如空格符号（space）时，应该用 \\x20 表示，如果要在表达式中表示 # 符号，应该用 \\# 表示。", LES_HAS_DEFAULT_VALUE, (INT)0x00000020},

};
static int s_objEventdeelxregex_DeelxConst_static_var_count_05 = sizeof(s_objEventdeelxregex_DeelxConst_static_var_05) / sizeof(s_objEventdeelxregex_DeelxConst_static_var_05[0]);

LIB_DATA_TYPE_INFO g_DataType_deelxregex_global_var[] = 
{
    //1=中文名字,2=英文名字,3=详细解释,4=命令数量,5=索引值,6=标志 LDT_
    //类型为窗口或菜单组件时有效 7=资源ID,8=事件数量,9=组件事件数组,10=属性数  11=属性数组 12=组件交互子程序
    //不为窗口、菜单组件或为枚举数据类型时才有效 13=成员数量,14=成员数据数组
    
/*000*/ { "正则表达式DEELX", "DeelxRegex", "封装了DEELX，支持 单行模式(SINGLELINE)、多行模式(MULTILINE)、全局模式(GLOBAL)、忽略大小写(IGNORECASE)、从右向左(RIGHTTOLEFT)、扩展模式(EXTENDED) 等常见匹配模式。支持与 Perl 兼容的正则表达式语法。注意，本支持库中的所有命令均为高级命令，要求使用者具有一定的正则表达式知识基础。",
            s_dtCmdIndexdeelxregex_DeelxRegex_static_var_count_00, s_dtCmdIndexdeelxregex_DeelxRegex_static_var_00, _DT_OS(__OS_WIN),
            0, NULL, NULL,
            NULL, NULL,
            NULL,
            NULL, NULL
        },
/*001*/ { "搜索结果DEELX", "DeelxSearchResult", "本对象中记录了所匹配到的字符串在整个文本中的位置，以及各个捕获组的位置。",
            s_dtCmdIndexdeelxregex_DeelxSearchResult_static_var_count_01, s_dtCmdIndexdeelxregex_DeelxSearchResult_static_var_01, _DT_OS(__OS_WIN),
            0, NULL, NULL,
            NULL, NULL,
            NULL,
            NULL, NULL
        },
/*002*/ { NULL, NULL, NULL,
            NULL, NULL, _DT_OS(__OS_WIN) | LDT_IS_HIDED,
            0, NULL, NULL,
            NULL, NULL,
            NULL,
            NULL, NULL
        },
/*003*/ { NULL, NULL, NULL,
            NULL, NULL, _DT_OS(__OS_WIN) | LDT_IS_HIDED,
            0, NULL, NULL,
            NULL, NULL,
            NULL,
            NULL, NULL
        },
/*004*/ { NULL, NULL, NULL,
            NULL, NULL, _DT_OS(__OS_WIN) | LDT_IS_HIDED,
            0, NULL, NULL,
            NULL, NULL,
            NULL,
            NULL, NULL
        },
/*005*/ { "正则常量", "DeelxConst", "DEELX正则表达式用到的常量",
            NULL, NULL, _DT_OS(__OS_WIN) | LDT_ENUM,
            0, NULL, NULL,
            NULL, NULL,
            NULL,
            s_objEventdeelxregex_DeelxConst_static_var_count_05, s_objEventdeelxregex_DeelxConst_static_var_05
        },
};
int g_DataType_deelxregex_global_var_count = sizeof(g_DataType_deelxregex_global_var) / sizeof(g_DataType_deelxregex_global_var[0]);

#endif

