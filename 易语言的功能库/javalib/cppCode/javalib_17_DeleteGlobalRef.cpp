#include "..\include_javalib_header.h"

// 调用格式: SDT_BOOL (Java本地接口).销毁全局引用, 命令说明: "销毁由参数所指定的全局引用对象。成功返回“真”，失败返回“假”。"
// 参数<1>: 欲销毁的全局引用对象标志符 SDT_INT, 参数说明: NULL
JAVALIB_EXTERN_C void javalib_DeleteGlobalRef_17_javalib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;

}

