#include "..\include_javalib_header.h"

// 调用格式: SDT_INT (Java本地接口).创建数组, 命令说明: "返回创建后的数组标志符。失败返回0。"
// 参数<1>: 数组长度 SDT_INT, 参数说明: NULL
// 参数<2>: 数组类型 SDT_INT, 参数说明: "指定该数组容纳的基本数据类型。请为本参数提供枚举类型“数组类型”中的常量值，如：“#数组类型.数据型”。"
JAVALIB_EXTERN_C void javalib_NewArray_49_javalib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;
    INT      arg2 = pArgInf[2].m_int;

}

