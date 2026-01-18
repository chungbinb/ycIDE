#include "..\include_ewizard_header.h"

// 调用格式: _SDT_NULL 复制程序, 命令说明: "通过复制的方式建立指定程序项"
// 参数<1>: 欲复制项类型 MAKELONG(0x01, 0), 参数说明: NULL
// 参数<2>: 欲复制项名称 SDT_TEXT, 参数说明: NULL
// 参数<3>: 新项目名称 SDT_TEXT, 参数说明: "必须确保该名称合法且不重复"
EWIZARD_EXTERN_C void ewizard_CopyProgram_0_ewizard(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    void*    arg1 = pArgInf[0].m_pCompoundData;
    LPSTR    arg2 = pArgInf[1].m_pText;
    LPSTR    arg3 = pArgInf[2].m_pText;

}

