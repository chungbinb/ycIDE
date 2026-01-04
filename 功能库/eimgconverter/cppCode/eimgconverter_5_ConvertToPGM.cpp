#include "..\include_eimgconverter_header.h"

// 调用格式: SDT_INT 转换到PGM, 命令说明: "将本库支持读取的图像文件转换为PGM文件，目前支持文本模式和二进制模式。成功返回 1 ，失败返回一个小于等于零的数值，具体为： 0: 未知错误； -1: 文件不存在； -2: 系统资源不足； -3: 该文件格式不被支持； -4: 编码时出错； -6: 编码时系统资源不足； -7: 编码参数错误 目前可以读取如下源格式的图像并转换到PGM格式：标准JPEG（全部格式）、BMP（目前仅支持24位BMP）、TIFF（单页图像所有格式）、PNG（32位及32位以下）、PCX（8位和24位，RLE压缩）、PCD（取第三幅）"
// 参数<1>: 源文件名 SDT_TEXT, 参数说明: "可以是本支持库支持的任意文件格式，请提供文件全名(包含路径)"
// 参数<2>: 目标文件名 SDT_TEXT, 参数说明: "参数值指定转换后保存到的目标文件名"
// 参数<3>: 保存模式 SDT_BOOL, 参数说明: "参数值为真则保存为二进制模式，为假则保存为文本模式，默认参数值为真"
EIMGCONVERTER_EXTERN_C void eimgconverter_ConvertToPGM_5_eimgconverter(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR    arg1 = pArgInf[0].m_pText;
    LPSTR    arg2 = pArgInf[1].m_pText;
    BOOL     arg3 = pArgInf[2].m_bool;

}

