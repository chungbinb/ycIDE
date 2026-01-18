#include "..\include_eapi_header.h"

// 调用格式: SDT_INT 取图片宽度, 命令说明: "取得指定图片的宽度。成功返回图片宽度；失败返回0。本命令为初级命令。"
// 参数<1>: 图片资源或图片路径 _SDT_ALL, 参数说明: "图片路径或资源"
EAPI_EXTERN_C void eapi_GetImageWidth_31_eapi(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    PVOID    arg1 = pArgInf[0].m_pByte;

}

