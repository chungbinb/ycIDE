#include "..\include_eapi_header.h"

// 调用格式: SDT_BOOL 设置桌面墙纸, 命令说明: "成功返回真；失败返回假。"
// 参数<1>: 文件名 SDT_TEXT, 参数说明: "墙纸文件名（bmp格式）。将文件名置为空可以取消墙纸。"
// 参数<2>: [样式 SDT_INT], 参数说明: "墙纸样式，可以为以下常量值之一：0、#接口常量.居中；1、#接口常量.平铺；2、#接口常量.拉伸。。"
EAPI_EXTERN_C void eapi_SetDeskWallPaper_65_eapi(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR    arg1 = pArgInf[0].m_pText;
    INT      arg2 = pArgInf[1].m_int;

}

