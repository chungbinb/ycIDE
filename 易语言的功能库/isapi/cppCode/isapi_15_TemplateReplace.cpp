#include "..\include_isapi_header.h"

// 调用格式: SDT_BOOL (互联网扩展).模板替换, 命令说明: "对给定的HTML模板进行替换, 注如果有相同的变量只替换一次。调用此方法之前要调用“设置替换标志”设置标志"
// 参数<1>: &要替换的模板 SDT_TEXT, 参数说明: "本参数表示要替换的模板。"
// 参数<2>: 变量名称替换数据 SDT_TEXT, 参数说明: "本参数的每一对参数值顺序指定欲被替换的变量名称及其相应的替换值。"
// 变长参数, 命令参数表中最后一个参数可以被重复添加
ISAPI_EXTERN_C void isapi_TemplateReplace_15_isapi(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR*   arg1 = pArgInf[1].m_ppText;
    LPSTR    arg2 = pArgInf[2].m_pText;

}

