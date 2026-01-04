#include "..\include_javalib_header.h"

// 调用格式: SDT_INT (Java本地接口).取父类, 命令说明: "取指定类的父类（或超类，SuperClass）。返回父类标志符，失败返回0。"
// 参数<1>: 类标志符 SDT_INT, 参数说明: "指定欲取其超类的类。该标志符不应为0。"
JAVALIB_EXTERN_C void javalib_GetSuperclass_9_javalib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;

}

