#include "..\include_ogrelib_header.h"

// 调用格式: SDT_INT 合成颜色, 命令说明: "将ARGB四分量合成单一的颜色值。"
// 参数<1>: 透明色 SDT_INT, 参数说明: "透明色(A)，范围从0到255。"
// 参数<2>: 红色 SDT_INT, 参数说明: "红色(R)，范围从0到255。"
// 参数<3>: 绿色 SDT_INT, 参数说明: "绿色(G)，范围从0到255。"
// 参数<4>: 蓝色 SDT_INT, 参数说明: "蓝色(B)，范围从0到255。"
OGRELIB_EXTERN_C void ogrelib_TransColor_287_ogrelib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[0].m_int;
    INT      arg2 = pArgInf[1].m_int;
    INT      arg3 = pArgInf[2].m_int;
    INT      arg4 = pArgInf[3].m_int;

}

