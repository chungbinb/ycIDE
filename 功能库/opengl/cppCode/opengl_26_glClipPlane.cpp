#include "..\include_opengl_header.h"

// 调用格式: _SDT_NULL 设置裁剪平面, 命令说明: "裁剪面的背面被减去。必须启用“#裁剪测试”和“#零号裁剪面～#五号裁剪面”，才能执行裁剪，不使用的时候需要关闭。"
// 参数<1>: 平面号 SDT_INT, 参数说明: "参数值可以为以下常量：\r\n\t12288、#零号裁剪面；\r\n\t12289、#一号裁剪面；\r\n\t12290、#二号裁剪面；\r\n\t12291、#三号裁剪面；\r\n\t12292、#四号裁剪面；\r\n\t12293、#五号裁剪面"
// 参数<2>: 平面参数 数组 SDT_DOUBLE, 参数说明: "参数值为四维数组。这四个数分别为一般平面方程“\r\n\t\tA（X - X0)+B（Y - Y0）+ C ( Y - Y0 ) + D = 0”\r\n中的A、B、C、D这几个系数。如果D = 0，那么（A，B，C）代表平面的法向量。如果D = -1，那么1/A，1/B，1/C分别代表平面在X，Y，Z轴上的截距。D默认为0。多余部分无效。例如一个平行于XZ平面，正面指向Y正方向的裁剪面就是{0，1，0，0}"
OPENGL_EXTERN_C void opengl_glClipPlane_26_opengl(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[0].m_int;
    void*    arg2 = pArgInf[1].m_pAryData;

}

