#include "..\include_opengl_header.h"

// 调用格式: _SDT_NULL 设置材质颜色, 命令说明: "使用当前颜色为材料的颜色。必须启用“#材质颜色”才有效，不使用的时候需要关闭。"
// 参数<1>: 应用面 SDT_INT, 参数说明: "参数值可以为以下常量：\r\n\t1028、#正面；\r\n\t1029、#背面；\r\n\t1030、#正面和背面；\r\n"
// 参数<2>: 模式 SDT_INT, 参数说明: "参数值可以为以下常量：\r\n\t4608、#环境光；\r\n\t4609、#漫反射光；\r\n\t4610、#镜面光；\r\n\t5632、#散射光；\r\n\t5633、#亮光；\r\n\t5634、#环境光和漫反射光"
OPENGL_EXTERN_C void opengl_glColorMaterial_55_opengl(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[0].m_int;
    INT      arg2 = pArgInf[1].m_int;

}

