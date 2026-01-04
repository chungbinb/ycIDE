#include "..\include_javalib_header.h"

// 调用格式: SDT_BOOL (Java本地接口).是否为类实例, 命令说明: "判断指定对象是否为指定类的实例。是则返回“真”，否则返回“假”。"
// 参数<1>: 对象标志符 SDT_INT, 参数说明: "指定一个对象。该标志符不应为0。"
// 参数<2>: 类标志符 SDT_INT, 参数说明: "指定一个类。该标志符不应为0。"
JAVALIB_EXTERN_C void javalib_IsInstanceOf_13_javalib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;
    INT      arg2 = pArgInf[2].m_int;

}

