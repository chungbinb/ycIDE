#include "..\include_ogrelib_header.h"

// 调用格式: SDT_BOOL (设备).置像素着色器常量, 命令说明: "常量数据类型为小数型数组。"
// 参数<1>: 注册编号 SDT_INT, 参数说明: "第一个常量值的注册编号。"
// 参数<2>: &常量数组 数组 SDT_FLOAT, 参数说明: "要置入的常量数组。"
// 参数<3>: 四维向量个数 SDT_INT, 参数说明: "常量数组中的四维向量个数。"
OGRELIB_EXTERN_C void ogrelib_SetPixelShaderConstantF_216_ogrelib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;
    void**   arg2 = pArgInf[2].m_ppAryData;
    INT      arg3 = pArgInf[3].m_int;

}

