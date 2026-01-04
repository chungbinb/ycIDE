#include "..\include_ogrelib_header.h"

// 调用格式: SDT_BOOL (设备).修改变换矩阵公式, 命令说明: "用指定的矩阵乘以设备的世界、视图或投影矩阵。"
// 参数<1>: 变换状态 MAKELONG(0x3A, 0), 参数说明: "为“变换状态类型”常量值之一。"
// 参数<2>: 矩阵 MAKELONG(0x04, 0), 参数说明: "这个矩阵将要被设置为当前变换的矩阵。"
OGRELIB_EXTERN_C void ogrelib_MultiplyTransform_209_ogrelib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    void*    arg1 = pArgInf[1].m_pCompoundData;
    void*    arg2 = pArgInf[2].m_pCompoundData;

}

