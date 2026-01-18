#include "..\include_opengl_header.h"

// 调用格式: SDT_BOOL 复制像素, 命令说明: "复制一个矩形范围内的内容到现在的光栅位置处。成功返回真，失败返回假"
// 参数<1>: 左下角横向位置 SDT_INT, 参数说明: "矩形的左下角距离坐标原点的横向素数个数。"
// 参数<2>: 左下角纵向位置 SDT_INT, 参数说明: "矩形的左下角距离坐标原点的纵向素数个数。"
// 参数<3>: 宽度 SDT_INT, 参数说明: "以素数为单位，必须大于0。"
// 参数<4>: 高度 SDT_INT, 参数说明: "以素数为单位，必须大于0。"
// 参数<5>: 类型 SDT_INT, 参数说明: "参数值可以为以下常量：\r\n\t6144、#颜色类型； \r\n\t6145、#深度类型； \r\n\t6146、#模板类型"
OPENGL_EXTERN_C void opengl_glCopyPixels_78_opengl(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[0].m_int;
    INT      arg2 = pArgInf[1].m_int;
    INT      arg3 = pArgInf[2].m_int;
    INT      arg4 = pArgInf[3].m_int;
    INT      arg5 = pArgInf[4].m_int;

}

