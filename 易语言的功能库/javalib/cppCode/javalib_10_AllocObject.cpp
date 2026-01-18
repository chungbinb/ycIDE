#include "..\include_javalib_header.h"

// 调用格式: SDT_INT (Java本地接口).分配对象, 命令说明: "分配指定Java类的新对象而不调用该类的任何构造函数。返回分配后的对象标志符，失败返回0。"
// 参数<1>: 类标志符 SDT_INT, 参数说明: "指定要分配的对象所属类型。"
JAVALIB_EXTERN_C void javalib_AllocObject_10_javalib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;

}

