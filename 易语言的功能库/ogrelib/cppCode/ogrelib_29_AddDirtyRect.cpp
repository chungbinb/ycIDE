#include "..\include_ogrelib_header.h"

// 调用格式: SDT_BOOL (立方体纹理).增加脏区域, 命令说明: "增加一块脏区域到资源中。"
// 参数<1>: 哪一面 MAKELONG(0x3B, 0), 参数说明: "本参数表示要增加立方体哪一面为脏区域。"
// 参数<2>: 区域 MAKELONG(0x01, 0), 参数说明: "要增加的区域。"
OGRELIB_EXTERN_C void ogrelib_AddDirtyRect_29_ogrelib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    void*    arg1 = pArgInf[1].m_pCompoundData;
    void*    arg2 = pArgInf[2].m_pCompoundData;

}

