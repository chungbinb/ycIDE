#include "..\include_javalib_header.h"

// 调用格式: SDT_BOOL (Java本地接口).是否相同, 命令说明: "判断两个变量是否引用同一Java对象。是则返回“真”，否则返回“假”。注意：如果两个参数标志符有其一为0，必将返回“假”（这一点与JNI不同）。"
// 参数<1>: 对象一标志符 SDT_INT, 参数说明: "指定一个对象。该标志符不应为0。"
// 参数<2>: 对象二标志符 SDT_INT, 参数说明: "指定一个对象。该标志符不应为0。"
JAVALIB_EXTERN_C void javalib_IsSameObject_14_javalib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;
    INT      arg2 = pArgInf[2].m_int;

}

