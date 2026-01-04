#include "..\include_ecalc_header.h"

// 调用格式: SDT_BOOL (大数).导出文本文件, 命令说明: "导出本对象当前所存储的数字到文本文件，成功返回真，失败返回假"
// 参数<1>: 文本文件名 SDT_TEXT, 参数说明: "要将数据导出的目的文本文件名，例如：\"c:\\1.txt\""
// 参数<2>: 覆盖现有 SDT_BOOL, 参数说明: "本参数指明是否覆盖已存在的文件"
ECALC_EXTERN_C void ecalc_ImportTxtFile_66_ecalc(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR    arg1 = pArgInf[1].m_pText;
    BOOL     arg2 = pArgInf[2].m_bool;

}

