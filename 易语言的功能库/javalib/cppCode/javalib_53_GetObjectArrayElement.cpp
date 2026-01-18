#include "..\include_javalib_header.h"

// 调用格式: SDT_INT (Java本地接口).取对象数组成员, 命令说明: "取指定“对象数组”中的某一个成员，由参数“成员索引”指定取该数组的哪一个成员。返回该成员的对象标志符，失败返回0。"
// 参数<1>: 数组标志符 SDT_INT, 参数说明: "指定欲取其成员的Java对象数组"
// 参数<2>: 成员索引 SDT_INT, 参数说明: "指定欲取数组的第几个成员。索引值从0开始。"
JAVALIB_EXTERN_C void javalib_GetObjectArrayElement_53_javalib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;
    INT      arg2 = pArgInf[2].m_int;

}

