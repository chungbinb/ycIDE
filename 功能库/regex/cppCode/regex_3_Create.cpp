#include "..\include_regex_header.h"

// 调用格式: SDT_BOOL (正则表达式).创建, 命令说明: "根据指定的正则表达式文本创建一个正则表达式对象。本对象中的原有内容将被释放。成功返回真；否则返回假。"
// 参数<1>: 正则表达式文本 SDT_TEXT, 参数说明: "比如“易语言4\\.0(模块|支持库)?”"
// 参数<2>: [是否区分大小写 SDT_BOOL], 参数说明: "本参数指定这个正则表达式在创建成功后进行匹配、搜索、替换时，是否区分大小写。如果本参数被省略，默认值为“真”"
REGEX_EXTERN_C void regex_Create_3_regex(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR    arg1 = pArgInf[1].m_pText;
    BOOL     arg2 = pArgInf[2].m_bool;

}

