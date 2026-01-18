#include "..\include_javalib_header.h"

// 调用格式: SDT_BOOL (Java本地接口).致命错误, 命令说明: "抛出致命错误并且不希望Java虚拟机进行修复。成功返回“真”，失败返回“假”。"
// 参数<1>: 错误描述文本 SDT_TEXT, 参数说明: NULL
JAVALIB_EXTERN_C void javalib_FatalError_62_javalib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR    arg1 = pArgInf[1].m_pText;

}

