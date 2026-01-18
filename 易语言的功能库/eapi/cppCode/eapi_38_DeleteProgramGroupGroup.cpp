#include "..\include_eapi_header.h"

// 调用格式: SDT_BOOL 删除程序组, 命令说明: "删除指定的特殊位置建立过的程序组。成功返回真，失败返回假。"
// 参数<1>: 文件夹名称 SDT_TEXT, 参数说明: "要删除的程序组名称。"
// 参数<2>: [特殊目录位置 SDT_INT], 参数说明: "程序组的起始位置。可以为以下常量值之一：0、#接口常量.开始菜单；1、#接口常量.开始菜单程序；2、#接口常量.桌面。"
EAPI_EXTERN_C void eapi_DeleteProgramGroupGroup_38_eapi(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR    arg1 = pArgInf[0].m_pText;
    INT      arg2 = pArgInf[1].m_int;

}

