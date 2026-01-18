#include "..\include_opengl_header.h"

// 调用格式: SDT_BOOL 定义二维纹理图片, 命令说明: "成功返回真，失败返回假"
// 参数<1>: 等级 SDT_INT, 参数说明: "通常为0，只有在多贴图情况下表，说明贴图的等级"
// 参数<2>: 颜色成份 SDT_INT, 参数说明: "必须为1、2、3或4"
// 参数<3>: 宽度 SDT_INT, 参数说明: "为范围大于0的数值，并且必须为2的幂数"
// 参数<4>: 高度 SDT_INT, 参数说明: "为范围大于0的数值，并且必须为2的幂数"
// 参数<5>: 格式 SDT_INT, 参数说明: "指出所读象素数据元素的格式。参数值可以为以下常量：\r\n\t6400、#单个颜色索引，每个元素为一个颜色索引号；\r\n\t6403、#红色分量，每个元素为一个红色分量的值；\r\n\t6404、#绿色分量，每个元素为一个绿色分量的值；\r\n\t6405、#蓝色分量，每个元素为一个蓝色分量的值；\r\n\t6406、#混合通道分量，每个元素为一个混合通道分量的值；\r\n\t6407、#红绿蓝分量，每个元素为一个红绿蓝分量的值；\r\n\t6408、#红绿蓝混合通道分量，每个元素为一个红绿蓝混合通道分量的值；\r\n\t6409、#亮度，每个元素为一个亮度分量的值；\r\n\t6410、#亮度和混合通道分量，每个元素为亮度和混合通道分量的值"
// 参数<6>: 图片像素 SDT_BIN, 参数说明: "一组包含纹理图片像素信息的数组，数组成员组成为[宽度][高度][颜色成份],否则纹理图片将发生变形。"
OPENGL_EXTERN_C void opengl_glTexImage2D_62_opengl(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[0].m_int;
    INT      arg2 = pArgInf[1].m_int;
    INT      arg3 = pArgInf[2].m_int;
    INT      arg4 = pArgInf[3].m_int;
    INT      arg5 = pArgInf[4].m_int;
    LPBYTE   arg6 = pArgInf[5].m_pBin;

}

