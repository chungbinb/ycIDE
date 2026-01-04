#include "..\include_ogrelib_header.h"

// 调用格式: SDT_BOOL 保存模型, 命令说明: "保存模型到模型文件(.x)中。"
// 参数<1>: 模型文件 SDT_TEXT, 参数说明: "模型文件名。"
// 参数<2>: 模型 MAKELONG(0x1A, 0), 参数说明: "要保存的模型对象。"
// 参数<3>: [邻接信息 数组 SDT_INT], 参数说明: "模型邻接信息。"
// 参数<4>: 材质数据 数组 MAKELONG(0x0C, 0), 参数说明: "要保存到模型文件的材质数据。"
// 参数<5>: 材质数量 SDT_INT, 参数说明: "要保存到模型文件的材质数量。"
// 参数<6>: [格式 SDT_INT], 参数说明: "保存到模型文件时使用的格式。为以下值之一：0、二进制格式；1、文本格式；2、压缩格式。默认值为0。"
// 参数<7>: 纹理文件数组 数组 SDT_TEXT, 参数说明: "要保存到模型文件的纹理文件数组。"
OGRELIB_EXTERN_C void ogrelib_D3DXSaveMeshToX_349_ogrelib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR    arg1 = pArgInf[0].m_pText;
    void*    arg2 = pArgInf[1].m_pCompoundData;
    void*    arg3 = pArgInf[2].m_pAryData;
    void*    arg4 = pArgInf[3].m_pAryData;
    INT      arg5 = pArgInf[4].m_int;
    INT      arg6 = pArgInf[5].m_int;
    void*    arg7 = pArgInf[6].m_pAryData;

}

