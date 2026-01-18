#include "..\include_ogrelib_header.h"

// 调用格式: SDT_BOOL (设备).显示光标, 命令说明: "显示或隐藏光标。返回真表示光标先前可见；假表示光标先前不可见。"
// 参数<1>: 是否显示 SDT_BOOL, 参数说明: "真为显示；假为隐藏。"
OGRELIB_EXTERN_C void ogrelib_ShowCursor_188_ogrelib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    BOOL     arg1 = pArgInf[1].m_bool;

}

