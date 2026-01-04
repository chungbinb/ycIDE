#include "..\include_ogrelib_header.h"

// 调用格式: SDT_INT 纹理关联, 命令说明: "将纹理的第几层和维数关联起来也就是说第几层是几维纹理。本命令的返回值可以与“灵活顶点格式”数据类型中的常量连用(位或)。返回-1表示失败原因是参数不正确。"
// 参数<1>: 几维纹理 MAKELONG(0x3C, 0), 参数说明: "可以为“灵活顶点格式”常量数据类型中的“一维纹理”、“二维纹理”、“三维纹理”、“四维纹理”。"
// 参数<2>: 第几层 SDT_INT, 参数说明: "第几层维理。"
OGRELIB_EXTERN_C void ogrelib_TextureCombine_244_ogrelib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    void*    arg1 = pArgInf[0].m_pCompoundData;
    INT      arg2 = pArgInf[1].m_int;

}

