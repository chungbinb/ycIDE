#include "..\include_ogrelib_header.h"

// 调用格式: SDT_BOOL 从文件中载入模型, 命令说明: "载入存储在模型文件（.x）中的模型数据。本命令先创建一个模型对象，然后从.x文件中读取模型的几何信息。"
// 参数<1>: 模型文件 SDT_TEXT, 参数说明: "要载入的模型文件（.x）。"
// 参数<2>: 创建标志 SDT_INT, 参数说明: "指定模型的创建标志，为“模型创建标志”常量值之一或组合。"
// 参数<3>: 设备 MAKELONG(0x09, 0), 参数说明: "指定与模型相关联的设备。"
// 参数<4>: [&邻接信息 数组 SDT_INT], 参数说明: "返回模型的邻接信息。"
// 参数<5>: [&材质数据 数组 MAKELONG(0x0C, 0)], 参数说明: "返回模型的材质数据。"
// 参数<6>: [&纹理文件名 数组 SDT_TEXT], 参数说明: NULL
// 参数<7>: 模型 MAKELONG(0x1A, 0), 参数说明: "载入的模型对象。"
OGRELIB_EXTERN_C void ogrelib_D3DXLoadMeshFromX_346_ogrelib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR    arg1 = pArgInf[0].m_pText;
    INT      arg2 = pArgInf[1].m_int;
    void*    arg3 = pArgInf[2].m_pCompoundData;
    void**   arg4 = pArgInf[3].m_ppAryData;
    void**   arg5 = pArgInf[4].m_ppAryData;
    void**   arg6 = pArgInf[5].m_ppAryData;
    void*    arg7 = pArgInf[6].m_pCompoundData;

}

