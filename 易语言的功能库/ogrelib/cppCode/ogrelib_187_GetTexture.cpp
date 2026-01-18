#include "..\include_ogrelib_header.h"

// 调用格式: SDT_BOOL (设备).取纹理, 命令说明: "取设备指定纹理层的纹理。"
// 参数<1>: 纹理层标识 SDT_INT, 参数说明: "要取得的纹理层标识。"
// 参数<2>: 纹理 MAKELONG(0x10, 0), 参数说明: "要取得的纹理。"
OGRELIB_EXTERN_C void ogrelib_GetTexture_187_ogrelib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;
    void*    arg2 = pArgInf[2].m_pCompoundData;

}

