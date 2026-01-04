#include "..\include_opengl_header.h"

// 调用格式: _SDT_NULL 设置线型, 命令说明: "设置线段的类型。默认使用实线。必须启用“#线型”才有效，不使用的时候需要关闭。"
// 参数<1>: 放大倍数 SDT_INT, 参数说明: "用来拉伸线型"
// 参数<2>: 线型 SDT_SHORT, 参数说明: "按照循序排列的16位二进制数其中每一位代表一个象素，且从低位开始，1表示用当前颜色绘制一个象素，0表示当前不绘制。"
OPENGL_EXTERN_C void opengl_glLineStipple_28_opengl(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[0].m_int;
    SHORT    arg2 = pArgInf[1].m_short;

}

