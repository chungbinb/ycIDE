#include "..\include_javalib_header.h"

// 调用格式: SDT_BOOL (Java本地接口).抛出新异常, 命令说明: "创建一个异常对象，并向Java虚拟机抛出该异常对象。成功返回“真”，失败返回“假”。"
// 参数<1>: 异常类标志符 SDT_INT, 参数说明: "指定欲创建的异常类，应为java.lang.Throwable或其子类。如果本参数为0，默认使用java.lang.Throwable。"
// 参数<2>: 异常描述文本 SDT_TEXT, 参数说明: NULL
JAVALIB_EXTERN_C void javalib_ThrowNew_56_javalib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;
    LPSTR    arg2 = pArgInf[2].m_pText;

}

