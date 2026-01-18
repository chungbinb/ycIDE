#include "..\include_eapi_header.h"

// 调用格式: SDT_BOOL 添加IE工具条按钮, 命令说明: "向IE工具栏添加自己的按钮。"
// 参数<1>: 按钮文字 SDT_TEXT, 参数说明: "标题"
// 参数<2>: 图标文件名 SDT_TEXT, 参数说明: "图标路径"
// 参数<3>: 执行动作 SDT_TEXT, 参数说明: "在IE中按下按钮后执行一个可执行文件或打开网页，就是命令行。"
// 参数<4>: [按钮热点图标 SDT_TEXT], 参数说明: "按钮处于热点状态显示的图标，默认不变更"
// 参数<5>: [按钮是否被显示 SDT_BOOL], 参数说明: "是否在工具栏显示，默认为真"
EAPI_EXTERN_C void eapi_AddButtonToIE_35_eapi(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR    arg1 = pArgInf[0].m_pText;
    LPSTR    arg2 = pArgInf[1].m_pText;
    LPSTR    arg3 = pArgInf[2].m_pText;
    LPSTR    arg4 = pArgInf[3].m_pText;
    BOOL     arg5 = pArgInf[4].m_bool;

}

