#include "..\include_javalib_header.h"

// 调用格式: SDT_INT (Java本地接口).取字符串长度, 命令说明: "返回Java字符串的长度（Unicode字符数），数字、字母、汉字等都被当作一个字符。失败返回0。"
// 参数<1>: 字符串标志符 SDT_INT, 参数说明: "指定欲取其长度的Java字符串对象"
JAVALIB_EXTERN_C void javalib_GetStringLength_41_javalib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;

}

