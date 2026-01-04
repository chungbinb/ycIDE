#include "..\include_javalib_header.h"

// 本命令被隐藏, 原始名字 = "测试函数"
// 调用格式: SDT_TEXT (Java虚拟机/Java本地接口).测试函数, 命令说明: "----"
// 参数<1>: 参数1 SDT_INT, 参数说明: NULL
// 参数<2>: 参数2 SDT_TEXT, 参数说明: NULL
JAVALIB_EXTERN_C void javalib_TestFunction_0_javalib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;
    LPSTR    arg2 = pArgInf[2].m_pText;

}

