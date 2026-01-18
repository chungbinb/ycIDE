#include "..\include_javalib_header.h"

// 调用格式: SDT_INT (Java本地接口).取数组长度, 命令说明: "返回指定Java数组的长度。"
// 参数<1>: 数组标志符 SDT_INT, 参数说明: "指定欲取其长度的Java数组"
JAVALIB_EXTERN_C void javalib_GetArrayLength_48_javalib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;

}

