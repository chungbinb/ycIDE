#include "..\include_ogrelib_header.h"

// 调用格式: SDT_BOOL (设备).置变换矩阵, 命令说明: "设置设备的世界变换矩阵。"
// 参数<1>: 变换状态 MAKELONG(0x3A, 0), 参数说明: "指定变换状态。为“变换状态”常量值之一。"
// 参数<2>: 矩阵 MAKELONG(0x04, 0), 参数说明: "这个矩阵将要被设置为当前变换的矩阵。"
OGRELIB_EXTERN_C void ogrelib_SetTransform_158_ogrelib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    void*    arg1 = pArgInf[1].m_pCompoundData;
    void*    arg2 = pArgInf[2].m_pCompoundData;

}

