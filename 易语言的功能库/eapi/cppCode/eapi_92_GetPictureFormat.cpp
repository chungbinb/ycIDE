#include "..\include_eapi_header.h"

// 调用格式: SDT_TEXT 取图片格式, 命令说明: "获取指定图片格式，失败返回空"
// 参数<1>: 图片路径或数据 _SDT_ALL, 参数说明: "可以选择图片路径，或者内存图片字节集。"
EAPI_EXTERN_C void eapi_GetPictureFormat_92_eapi(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    PVOID    arg1 = pArgInf[0].m_pByte;

}

