#include "..\include_opengl_header.h"

// 调用格式: _SDT_NULL 画出位图, 命令说明: "必须先使用“设置像素存储字节数”。坐标的原点只与“设置光栅位置”有关与坐标变换命令无关。"
// 参数<1>: 宽度 SDT_INT, 参数说明: "“位图数据”中将要显示的宽度，必须大于0。"
// 参数<2>: 高度 SDT_INT, 参数说明: "“位图数据”中将要显示的高度，必须大于0。"
// 参数<3>: 原点横坐标 SDT_FLOAT, 参数说明: "坐标原点与位图左下角的横向素数距离，正数时在原点左边，反之在右边。"
// 参数<4>: 原点竖坐标 SDT_FLOAT, 参数说明: "坐标原点与位图左下角的竖向素数距离，正数时在原点下边，反之在上边。"
// 参数<5>: 横向平移 SDT_FLOAT, 参数说明: "本次画出结束后，坐标原点向右移动的像素距离，负数为反方向。只会影响下一次“画出位图”时的画出位置，与本次画出无关。"
// 参数<6>: 纵向平移 SDT_FLOAT, 参数说明: "本次画出结束后，坐标原点向上移动的像素距离，负数为反方向。只会影响下一次“画出位图”时的画出位置，与本次画出无关。"
// 参数<7>: 位图数据 SDT_BIN, 参数说明: NULL
OPENGL_EXTERN_C void opengl_glBitmap_77_opengl(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[0].m_int;
    INT      arg2 = pArgInf[1].m_int;
    FLOAT    arg3 = pArgInf[2].m_float;
    FLOAT    arg4 = pArgInf[3].m_float;
    FLOAT    arg5 = pArgInf[4].m_float;
    FLOAT    arg6 = pArgInf[5].m_float;
    LPBYTE   arg7 = pArgInf[6].m_pBin;

}

