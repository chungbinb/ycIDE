#include "..\include_ogrelib_header.h"

// 调用格式: SDT_BOOL (纹理).取私有数据, 命令说明: "将已关联到本资源的数据，复制到指定的字节集中。"
// 参数<1>: 全局唯一标识符 MAKELONG(0x0D, 0), 参数说明: "用“置私有数据”方法时传入的全局唯一标识符参数。"
// 参数<2>: &数据 SDT_BIN, 参数说明: "要获取的数据。"
OGRELIB_EXTERN_C void ogrelib_GetPrivateData_72_ogrelib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    void*    arg1 = pArgInf[1].m_pCompoundData;
    LPBYTE*  arg2 = pArgInf[2].m_ppBin;

}

