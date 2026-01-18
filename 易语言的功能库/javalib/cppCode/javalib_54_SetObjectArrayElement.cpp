#include "..\include_javalib_header.h"

// 调用格式: SDT_BOOL (Java本地接口).置对象数组成员, 命令说明: "为Java“对象数组”的指定成员赋值。"
// 参数<1>: 数组标志符 SDT_INT, 参数说明: "指定欲赋值的Java对象数组"
// 参数<2>: 成员索引 SDT_INT, 参数说明: "指定欲对其赋值的成员索引。索引值从0开始。"
// 参数<3>: 欲赋值的对象标志符 SDT_INT, 参数说明: "指定欲赋值的对象"
JAVALIB_EXTERN_C void javalib_SetObjectArrayElement_54_javalib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;
    INT      arg2 = pArgInf[2].m_int;
    INT      arg3 = pArgInf[3].m_int;

}

