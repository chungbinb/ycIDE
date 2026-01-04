#include "..\include_opengl_header.h"

// 调用格式: _SDT_NULL 设置图案填充, 命令说明: "设置多边形的填充方式。必须启用“#图案填充”才有效，不使用的时候需要关闭。"
// 参数<1>: 图案 数组 SDT_BYTE, 参数说明: "一个拥有128个成员的数组，其中每个字节代表八个位，每一位代表一个象素，最后呈现出一个32×32位的图案。且从低位开始。1表示用当前颜色绘制一个象素，0表示当前不绘制。多余部分无效。"
OPENGL_EXTERN_C void opengl_glPolygonStipple_29_opengl(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    void*    arg1 = pArgInf[0].m_pAryData;

}

