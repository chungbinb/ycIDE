#include "..\include_opengl_header.h"

// 调用格式: _SDT_NULL 设置纹理映射方式, 命令说明: NULL
// 参数<1>: 处理方式 SDT_INT, 参数说明: "参数值可以为以下常量：\r\n\t8704、#纹理映射模式；映射值可以为以下常量：\r\n\t\t8448、#调节处理；调节当前亮度和颜色 \r\n\t\t8849、#粘贴处理；不受颜色和亮度影响 \r\n\t\t3042、#混合测试；与原来色颜色进行混合测试。\r\n\t8705、#纹理映射颜色；映射值为范围在[0，1]内的四维数组，多余部分无效"
// 参数<2>: 映射值 数组/非数组 SDT_FLOAT, 参数说明: NULL
OPENGL_EXTERN_C void opengl_glTexEnv_64_opengl(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[0].m_int;
    void*    arg2 = pArgInf[1].m_pAryData;

}

