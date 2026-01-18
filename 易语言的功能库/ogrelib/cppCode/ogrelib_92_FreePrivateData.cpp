#include "..\include_ogrelib_header.h"

// 调用格式: SDT_BOOL (立体).释放私有数据, 命令说明: "释放关联在本立体上的私有数据。"
// 参数<1>: 全局唯一标识符 MAKELONG(0x0D, 0), 参数说明: "用“置私有数据”方法时传入的全局唯一标识符参数。"
OGRELIB_EXTERN_C void ogrelib_FreePrivateData_92_ogrelib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    void*    arg1 = pArgInf[1].m_pCompoundData;

}

