#include "..\include_ogrelib_header.h"

// 调用格式: SDT_INT 浮点型到整型, 命令说明: "将浮点型数值在内存中的形式转为整型表示，主要用于“设备”中的“置渲染状态”方法的参数“渲染状态值”要求是浮点型的。"
// 参数<1>: 浮点数 SDT_FLOAT, 参数说明: "要转换为整型的浮点数。"
OGRELIB_EXTERN_C void ogrelib_FloatToInt_288_ogrelib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    FLOAT    arg1 = pArgInf[0].m_float;

}

