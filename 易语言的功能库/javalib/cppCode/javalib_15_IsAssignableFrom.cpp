#include "..\include_javalib_header.h"

// 调用格式: SDT_BOOL (Java本地接口).可否强制转换, 命令说明: "判断参数一（“将被转换的类”）的对象是否可安全地强制转换为参数二（“将被转换到的类”）。以下情况下将返回“真”：第一及第二个类参数引用同一个 Java 类；第一个类是第二个类的子类；第二个类是第一个类的某个接口。"
// 参数<1>: 将被转换的类标志符 SDT_INT, 参数说明: "指定将被转换的类"
// 参数<2>: 将被转换到的类标志符 SDT_INT, 参数说明: "指定将被转换到的类"
JAVALIB_EXTERN_C void javalib_IsAssignableFrom_15_javalib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;
    INT      arg2 = pArgInf[2].m_int;

}

