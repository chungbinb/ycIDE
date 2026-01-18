#include "..\include_ewizard_header.h"

// 调用格式: _SDT_NULL 删除程序, 命令说明: "删除程序中的指定程序项"
// 参数<1>: 欲删除项类型 MAKELONG(0x01, 0), 参数说明: NULL
// 参数<2>: 欲删除项名称 SDT_TEXT, 参数说明: NULL
EWIZARD_EXTERN_C void ewizard_RemoveProgram_2_ewizard(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    void*    arg1 = pArgInf[0].m_pCompoundData;
    LPSTR    arg2 = pArgInf[1].m_pText;

}

