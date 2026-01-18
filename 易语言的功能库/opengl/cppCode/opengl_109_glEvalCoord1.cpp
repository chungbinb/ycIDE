#include "..\include_opengl_header.h"

// 调用格式: _SDT_NULL 设置曲线点, 命令说明: "插入到“开始画”和“停止画”中来设置曲线上的点。先使用“设置曲线控制点”命令生成曲线，然后指定U参数的值，计算出曲线上点的坐标。必须启用“#曲线坐标”才有效，不使用的时候需要关闭。"
// 参数<1>: U参数 SDT_DOUBLE, 参数说明: "参数值应该介于“设置曲线控制点”命令中的“U参数上限”和“U参数下限”之间。"
OPENGL_EXTERN_C void opengl_glEvalCoord1_109_opengl(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    double   arg1 = pArgInf[0].m_double;

}

