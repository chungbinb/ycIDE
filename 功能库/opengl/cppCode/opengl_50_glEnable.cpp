#include "..\include_opengl_header.h"

// 调用格式: _SDT_NULL 启用功能, 命令说明: "启用某项功能后，会一直保持启用状态。要关闭该功能，必须使用停用功能才能停止。"
// 参数<1>: 功能 SDT_INT, 参数说明: "参数值可以为以下常量：\r\n\t3008、#通道测试；\r\n\t3042、#混合测试；\r\n\t2903、#材质颜色；\r\n\t2929、#深度测试；\r\n\t2912、#雾；\r\n\t2884、#精选面；\r\n\t2896、#光照；\r\n\t2852、#线型；\r\n\t3153、#点质量；\r\n\t3154、#线质量；\r\n\t2847、#光滑点；\r\n\t2848、#光滑线；\r\n\t2977、#单位法向量；\r\n\t2882、#图案填充；\r\n\t3479、#曲线坐标；\r\n\t3511、#曲面坐标；\r\n\t3456、#自动法向量\t3089、#裁剪测试；\r\n\t2960、#模板测试；\r\n\t3552、#一维纹理；\r\n\t3553、#二维纹理；\r\n\t3168、#生成S向纹理；\r\n\t3169、#生成T向纹理；\r\n\t12288--12293、#零号裁剪面--五号裁剪面；\r\n\t16384--16391、#零号光源--七号光源"
OPENGL_EXTERN_C void opengl_glEnable_50_opengl(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[0].m_int;

}

