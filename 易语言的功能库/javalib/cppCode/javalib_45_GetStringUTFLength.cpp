#include "..\include_javalib_header.h"

// 本命令被隐藏, 原始名字 = "取UTF字符串长度"
// 调用格式: SDT_INT (Java本地接口).取UTF字符串长度, 命令说明: "返回指定UTF字符串的长度，以字节(byte)为单位。"
// 参数<1>: 字符串标志符 SDT_INT, 参数说明: "指定欲取其长度的Java字符串对象"
JAVALIB_EXTERN_C void javalib_GetStringUTFLength_45_javalib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;

}

