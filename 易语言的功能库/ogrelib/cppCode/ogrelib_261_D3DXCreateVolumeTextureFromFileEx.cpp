#include "..\include_ogrelib_header.h"

// 调用格式: SDT_BOOL 从文件创建立体纹理扩展, 命令说明: "从指定的文件创建立体纹理。可以指定一些扩展参数。"
// 参数<1>: 设备 MAKELONG(0x09, 0), 参数说明: "指定与立体纹理相关联的设备。"
// 参数<2>: 文件名 SDT_TEXT, 参数说明: "指定文件名。"
// 参数<3>: 宽度 SDT_INT, 参数说明: "立体纹理的宽度（以像素为单位）。如果参数值为0或-1，尺寸将从文件中取得。"
// 参数<4>: 高度 SDT_INT, 参数说明: "立体纹理的高度（以像素为单位）。如果参数值为0或-1，尺寸将从文件中取得。"
// 参数<5>: 深度 SDT_INT, 参数说明: "立体纹理的深度（以像素为单位）。如果参数值为0或-1，尺寸将从文件中取得。"
// 参数<6>: 渐进级数 SDT_INT, 参数说明: "指定渐进级数。如果参数值为0或-1，将创建一个完整的渐进映射链。"
// 参数<7>: 用途 MAKELONG(0x37, 0), 参数说明: "为“资源用途”常量值之一。"
// 参数<8>: 像素格式 MAKELONG(0x35, 0), 参数说明: "要取得的像素格式。"
// 参数<9>: 内存池类型 MAKELONG(0x39, 0), 参数说明: "指定纹理的放置位置。为“内存池类型”常量值之一。"
// 参数<10>: 纹理过滤器 MAKELONG(0x40, 0), 参数说明: "指定图像如何过滤。为“纹理通道过滤”常量值之一或组合。值为-1时相当于：#纹理通道过滤.由点生成目标点 + #纹理通道过滤.用抖动运算产生。"
// 参数<11>: 渐进过滤器 MAKELONG(0x40, 0), 参数说明: "指定图像如何过滤。为“纹理通道过滤”常量值之一或组合。值为-1时等于：#纹理通道过滤.由区域生成目标点。"
// 参数<12>: 关键色 SDT_INT, 参数说明: "表示被替换为Alpha = 0的透明色。设置为0，表示禁用透明色。"
// 参数<13>: &源图像文件信息 MAKELONG(0x28, 0), 参数说明: NULL
// 参数<14>: &调色板 MAKELONG(0x1F, 0), 参数说明: "返回256色调色板单元。"
// 参数<15>: &立体纹理 MAKELONG(0x11, 0), 参数说明: "要创建的立体纹理对象。"
OGRELIB_EXTERN_C void ogrelib_D3DXCreateVolumeTextureFromFileEx_261_ogrelib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    void*    arg1 = pArgInf[0].m_pCompoundData;
    LPSTR    arg2 = pArgInf[1].m_pText;
    INT      arg3 = pArgInf[2].m_int;
    INT      arg4 = pArgInf[3].m_int;
    INT      arg5 = pArgInf[4].m_int;
    INT      arg6 = pArgInf[5].m_int;
    void*    arg7 = pArgInf[6].m_pCompoundData;
    void*    arg8 = pArgInf[7].m_pCompoundData;
    void*    arg9 = pArgInf[8].m_pCompoundData;
    void*    arg10 = pArgInf[9].m_pCompoundData;
    void*    arg11 = pArgInf[10].m_pCompoundData;
    INT      arg12 = pArgInf[11].m_int;
    void**   arg13 = pArgInf[12].m_ppCompoundData;
    void**   arg14 = pArgInf[13].m_ppCompoundData;
    void**   arg15 = pArgInf[14].m_ppCompoundData;

}

