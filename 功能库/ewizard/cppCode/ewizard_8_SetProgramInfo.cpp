#include "..\include_ewizard_header.h"

// 调用格式: _SDT_NULL 置程序信息, 命令说明: "设置程序整体的相关信息"
// 参数<1>: [程序名称 SDT_TEXT], 参数说明: "如果本参数被省略，则不改动此项"
// 参数<2>: [程序类型 SDT_INT], 参数说明: "具体值为： 1、Windows窗口程序； 2、Windows控制台程序； 3、Windows动态链接库； 4、Windows易语言模块； 5、Linux控制台程序； 6、Linux易语言模块。 如果本参数被省略，则不改动此项"
// 参数<3>: [备注 SDT_TEXT], 参数说明: "如果本参数被省略，则不改动此项"
// 参数<4>: [版本 SDT_TEXT], 参数说明: "版本文本格式为“主版本号.次版本号”，如果本参数被省略，则不改动此项"
// 参数<5>: [图标 SDT_BIN], 参数说明: "如果本参数被省略，则不改动此项"
EWIZARD_EXTERN_C void ewizard_SetProgramInfo_8_ewizard(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR    arg1 = pArgInf[0].m_pText;
    INT      arg2 = pArgInf[1].m_int;
    LPSTR    arg3 = pArgInf[2].m_pText;
    LPSTR    arg4 = pArgInf[3].m_pText;
    LPBYTE   arg5 = pArgInf[4].m_pBin;

}

