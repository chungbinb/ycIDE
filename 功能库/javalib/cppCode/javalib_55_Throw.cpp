#include "..\include_javalib_header.h"

// 调用格式: SDT_BOOL (Java本地接口).抛出异常, 命令说明: "向Java虚拟机抛出一个已存在的异常对象。成功返回“真”，失败返回“假”。"
// 参数<1>: 异常对象标志符 SDT_INT, 参数说明: "指定欲抛出的Java异常对象。"
JAVALIB_EXTERN_C void javalib_Throw_55_javalib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;

}

