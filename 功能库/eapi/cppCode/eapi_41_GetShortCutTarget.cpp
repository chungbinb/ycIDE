#include "..\include_eapi_header.h"

// 调用格式: SDT_TEXT 取快捷方式目标, 命令说明: "获取指定快捷方式（.lnk）的目标和参数。成功返回目标，失败返回空。只有第一个参数为输入参数，后面的其它参数均为输出参数（可省略）。"
// 参数<1>: 快捷方式文件名 SDT_TEXT, 参数说明: NULL
// 参数<2>: [&目标 SDT_TEXT], 参数说明: "快捷方式文件名绑定的程序路径"
// 参数<3>: [&参数 SDT_TEXT], 参数说明: "默认参数"
// 参数<4>: [&启始位置 SDT_TEXT], 参数说明: "快捷方式的上级目录"
// 参数<5>: [&图标 SDT_TEXT], 参数说明: "返回的文本格式为：图标所属文件 + , + 图标序号（0为第一个），例如：“c:\\x.exe,1”。"
// 参数<6>: [&运行方式 SDT_INT], 参数说明: "。1：常规窗口；2：最小化；3：最大化"
// 参数<7>: [&快捷键 _SDT_ALL], 参数说明: "可以传入文本型或整数型型变量。如果为文本型，返回快捷键文本形式，如“Ctrl + Shift + Alt + A”。如果为整数型，返回一个表示该快捷键的数值（参见http://msdn.microsoft.com/en-us/library/bb774926"
// 参数<8>: [&备注 SDT_TEXT], 参数说明: "快捷方式的描述"
EAPI_EXTERN_C void eapi_GetShortCutTarget_41_eapi(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR    arg1 = pArgInf[0].m_pText;
    LPSTR*   arg2 = pArgInf[1].m_ppText;
    LPSTR*   arg3 = pArgInf[2].m_ppText;
    LPSTR*   arg4 = pArgInf[3].m_ppText;
    LPSTR*   arg5 = pArgInf[4].m_ppText;
    PINT     arg6 = pArgInf[5].m_pInt;
    PVOID    arg7 = pArgInf[6].m_pByte;
    LPSTR*   arg8 = pArgInf[7].m_ppText;

}

