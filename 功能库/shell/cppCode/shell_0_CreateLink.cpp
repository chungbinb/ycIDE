#include "..\include_shell_header.h"

// 调用格式: SDT_BOOL 创建快捷方式, 命令说明: "建立指定的快捷方式文件,返回是否成功."
// 参数<1>: 快捷方式文件名 SDT_TEXT, 参数说明: "如果文件名不包含后缀,将自动加上\".lnk\"后缀名."
// 参数<2>: [指向文件路径 SDT_TEXT], 参数说明: "提供快捷方式所指向文件的全路径名"
// 参数<3>: [备注信息 SDT_TEXT], 参数说明: "提供对应此快捷方式的描述文本信息"
// 参数<4>: [命令行文本 SDT_TEXT], 参数说明: "提供启动快捷方式指向文件时附加的命令行文本"
// 参数<5>: [工作目录 SDT_TEXT], 参数说明: "提供启动快捷方式指向文件时所需预置的工作目录"
// 参数<6>: [热键 SDT_INT], 参数说明: "热键键值，具体参照微软接口"
SHELL_EXTERN_C void shell_CreateLink_0_shell(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR    arg1 = pArgInf[0].m_pText;
    LPSTR    arg2 = pArgInf[1].m_pText;
    LPSTR    arg3 = pArgInf[2].m_pText;
    LPSTR    arg4 = pArgInf[3].m_pText;
    LPSTR    arg5 = pArgInf[4].m_pText;
    INT      arg6 = pArgInf[5].m_int;

}

