#include "..\include_opengl_header.h"

// 调用格式: _SDT_NULL 设置明暗模式, 命令说明: "设置明暗的表示方法。对点图元无效。"
// 参数<1>: 模式 SDT_INT, 参数说明: "参数值可以为以下常量：\r\n\t7424、#平坦，明暗的过度为跳跃地变化；\r\n\t7425、#光滑，明暗的过度为连续地变化"
OPENGL_EXTERN_C void opengl_glShadeModel_58_opengl(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[0].m_int;

}

