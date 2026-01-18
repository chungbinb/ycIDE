#include "..\include_shellex_header.h"

// 调用格式: SDT_BOOL 添加提示, 命令说明: "创建一个提示标签。"
// 参数<1>: 控件句柄 SDT_INT, 参数说明: "需要添加提示标签的控件的句柄。"
// 参数<2>: 提示文本 _SDT_ALL, 参数说明: "提示标签上显示的文字，如果要显示多行，请加上#换行符。"
// 参数<3>: 是否为气泡型 SDT_BOOL, 参数说明: "comctl32.dll版本必须≥v5.8，以第一次调用为准，因为所有提示都相同，不能分开定义。"
// 参数<4>: 提示工具位置 SDT_BOOL, 参数说明: "为真表示总显示在控件的中下方，为假表示跟随鼠标。"
SHELLEX_EXTERN_C void shellex_AddTooltip_0_shellex(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[0].m_int;
    PVOID    arg2 = pArgInf[1].m_pByte;
    BOOL     arg3 = pArgInf[2].m_bool;
    BOOL     arg4 = pArgInf[3].m_bool;

}

