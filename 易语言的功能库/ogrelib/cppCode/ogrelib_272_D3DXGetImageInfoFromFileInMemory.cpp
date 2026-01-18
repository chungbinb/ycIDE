#include "..\include_ogrelib_header.h"

// 调用格式: SDT_BOOL 从资源中取图像, 命令说明: "从指定的资源(图片资源)中获取图像信息。"
// 参数<1>: 图片资源 SDT_BIN, 参数说明: "指定的图片资源。"
// 参数<2>: 图像信息 MAKELONG(0x28, 0), 参数说明: "要保存的图像信息。"
OGRELIB_EXTERN_C void ogrelib_D3DXGetImageInfoFromFileInMemory_272_ogrelib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPBYTE   arg1 = pArgInf[0].m_pBin;
    void*    arg2 = pArgInf[1].m_pCompoundData;

}

