#include "..\include_javalib_header.h"

// 调用格式: SDT_TEXT (Java本地接口).取字符串文本, 命令说明: "返回与指定Java字符串相对应的易语言文本。"
// 参数<1>: 字符串标志符 SDT_INT, 参数说明: "指定欲取其文本的Java字符串对象"
// 参数<2>: [&执行结果 SDT_BOOL], 参数说明: "如果提供本参数，则在方法执行后存放执行结果：执行成功，其值为“真”；执行失败，其值为“假”。"
JAVALIB_EXTERN_C void javalib_GetStringChars_42_javalib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;
    BOOL*    arg2 = pArgInf[2].m_pBool;

}

