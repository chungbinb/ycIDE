#include "..\include_ogrelib_header.h"

// 调用格式: SDT_BOOL (立体纹理).增加脏区域, 命令说明: "增加一块脏区域到资源中。"
// 参数<1>: 立体空间 MAKELONG(0x23, 0), 参数说明: "要增加的立体空间区域。"
OGRELIB_EXTERN_C void ogrelib_AddDirtyBox_102_ogrelib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    void*    arg1 = pArgInf[1].m_pCompoundData;

}

