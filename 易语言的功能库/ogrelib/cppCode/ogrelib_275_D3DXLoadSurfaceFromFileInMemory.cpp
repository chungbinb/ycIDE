#include "..\include_ogrelib_header.h"

// 调用格式: SDT_BOOL 从内存中加载页面, 命令说明: "从内存中加载页面。"
// 参数<1>: 页面 MAKELONG(0x08, 0), 参数说明: "目标面页。"
// 参数<2>: 调色板 MAKELONG(0x1F, 0), 参数说明: "目标调色板，成员都为0表示不指定。"
// 参数<3>: 目标区域 MAKELONG(0x01, 0), 参数说明: "目标区域，成员都为0表示全部区域。"
// 参数<4>: 图片资源 SDT_BIN, 参数说明: "指定的图片资源。"
// 参数<5>: 原区域 MAKELONG(0x01, 0), 参数说明: "原区域。"
// 参数<6>: 过滤 MAKELONG(0x40, 0), 参数说明: "决定图像将如何过滤。"
// 参数<7>: 关键色 SDT_INT, 参数说明: "指定的颜色值将原图像和它相同的颜色变透明,设为0表示不用关键色。"
// 参数<8>: 图像信息 MAKELONG(0x28, 0), 参数说明: "要保存的图像信息。"
OGRELIB_EXTERN_C void ogrelib_D3DXLoadSurfaceFromFileInMemory_275_ogrelib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    void*    arg1 = pArgInf[0].m_pCompoundData;
    void*    arg2 = pArgInf[1].m_pCompoundData;
    void*    arg3 = pArgInf[2].m_pCompoundData;
    LPBYTE   arg4 = pArgInf[3].m_pBin;
    void*    arg5 = pArgInf[4].m_pCompoundData;
    void*    arg6 = pArgInf[5].m_pCompoundData;
    INT      arg7 = pArgInf[6].m_int;
    void*    arg8 = pArgInf[7].m_pCompoundData;

}

