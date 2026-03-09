#include "..\include_spec_header.h"

// 调用格式: _SDT_NULL 取文本_属性读取, 命令说明: "本命令用于获取“取文本”命令所使用的各项属性设置，结果将存入相应的参数变量中。"
// 参数<1>: [&半角双引号替换符 SDT_TEXT], 参数说明: NULL
// 参数<2>: [&全角左引号替换符 SDT_TEXT], 参数说明: NULL
// 参数<3>: [&全角右引号替换符 SDT_TEXT], 参数说明: NULL
// 参数<4>: [&文本替换符前缀 SDT_TEXT], 参数说明: NULL
SPEC_EXTERN_C void spec_GetText_Get_14_spec(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR*   arg1 = pArgInf[0].m_ppText;
    LPSTR*   arg2 = pArgInf[1].m_ppText;
    LPSTR*   arg3 = pArgInf[2].m_ppText;
    LPSTR*   arg4 = pArgInf[3].m_ppText;

}

