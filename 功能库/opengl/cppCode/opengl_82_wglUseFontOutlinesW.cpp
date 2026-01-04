#include "..\include_opengl_header.h"

// 调用格式: SDT_BOOL 创建文字轮廓, 命令说明: "创建三维的文字外形轮廓。如果成功返回真，否则返回假。"
// 参数<1>: 设备句柄 SDT_INT, 参数说明: NULL
// 参数<2>: 欲使用其外形的首个字符的代码 SDT_INT, 参数说明: NULL
// 参数<3>: 字符数 SDT_INT, 参数说明: "必须是大于0。如果为1就显示首个字符，2就显示首个字符后面的一个字符。中文为双字节一个字符，不能大于1"
// 参数<4>: 基列表号 SDT_INT, 参数说明: NULL
// 参数<5>: 轮廓偏移量 SDT_FLOAT, 参数说明: "数值越小越逼真，0的时候最好。"
// 参数<6>: 拉伸厚度 SDT_FLOAT, 参数说明: "沿着Z轴方向拉伸的距离。"
// 参数<7>: 实心 SDT_BOOL, 参数说明: "真表示使用实心字体，否则生成空心轮廓。"
OPENGL_EXTERN_C void opengl_wglUseFontOutlinesW_82_opengl(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[0].m_int;
    INT      arg2 = pArgInf[1].m_int;
    INT      arg3 = pArgInf[2].m_int;
    INT      arg4 = pArgInf[3].m_int;
    FLOAT    arg5 = pArgInf[4].m_float;
    FLOAT    arg6 = pArgInf[5].m_float;
    BOOL     arg7 = pArgInf[6].m_bool;

}

