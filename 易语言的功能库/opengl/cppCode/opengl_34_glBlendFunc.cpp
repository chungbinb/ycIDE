#include "..\include_opengl_header.h"

// 调用格式: _SDT_NULL 混合测试, 命令说明: "为混合运算设定运算方法。必须启用“#混合测试”才有效。不使用的时候需要关闭。"
// 参数<1>: 源值 SDT_INT, 参数说明: "参数值可以为以下常量：\r\n\t0、0；\r\n\t1、1；\r\n\t770、#源值通道比例；\r\n\t771、#源值通道比例反；\r\n\t774、#目标值颜色比例；\r\n\t775、#目标值颜色比例反；\r\n\t776、#源值饱和通道"
// 参数<2>: 目标值 SDT_INT, 参数说明: "参数值可以为以下常量：\r\n\t0、0； \r\n\t1、1； \r\n\t768、#源值颜色比例；\r\n\t769、#源值颜色比例反；\r\n\t770、#源值通道比例；\r\n\t771、#源值通道比例反；\r\n\t772、#目标值通道比例；\r\n\t773、#目标值通道比例反"
OPENGL_EXTERN_C void opengl_glBlendFunc_34_opengl(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[0].m_int;
    INT      arg2 = pArgInf[1].m_int;

}

