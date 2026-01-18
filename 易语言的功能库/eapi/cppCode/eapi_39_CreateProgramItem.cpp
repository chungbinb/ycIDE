#include "..\include_eapi_header.h"

// 调用格式: SDT_BOOL 创建程序项, 命令说明: "在指定的特殊位置位置程序组文件夹中创建快捷方式。"
// 参数<1>: 程序组文件夹 SDT_TEXT, 参数说明: "要创建的快捷方式的程序组文件夹名称，非路径。"
// 参数<2>: 程序项文件夹 SDT_TEXT, 参数说明: "要创建的快捷方式名称"
// 参数<3>: 快捷方式链接的程序路径 SDT_TEXT, 参数说明: "绑定执行程序文件名的路径。"
// 参数<4>: [特殊目录位置 SDT_INT], 参数说明: "程序组的起始位置。可以为以下常量值之一：0、#接口常量.开始菜单；1、#接口常量.开始菜单程序；2、#接口常量.桌面。"
EAPI_EXTERN_C void eapi_CreateProgramItem_39_eapi(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR    arg1 = pArgInf[0].m_pText;
    LPSTR    arg2 = pArgInf[1].m_pText;
    LPSTR    arg3 = pArgInf[2].m_pText;
    INT      arg4 = pArgInf[3].m_int;

}

