#include "..\include_ogrelib_header.h"

// 调用格式: SDT_BOOL 创建模型, 命令说明: "创建一个空模型。"
// 参数<1>: 三角形数量 SDT_INT, 参数说明: "模型中的三角形数量，必须指定一个大于0的数。"
// 参数<2>: 顶点数量 SDT_INT, 参数说明: "顶点数量，必须指定一个大于0的数。"
// 参数<3>: 创建标志 SDT_INT, 参数说明: "指定模型的创建标志，为“模型创建标志”常量值之一或组合。"
// 参数<4>: 顶点成员数组 数组 MAKELONG(0x16, 0), 参数说明: "顶点成员数组。描述创建模型的顶点格式，该参数必须直接映射到灵活顶点格式。"
// 参数<5>: 设备 MAKELONG(0x09, 0), 参数说明: "与模型相关联的设备对象。"
// 参数<6>: 模型 MAKELONG(0x1A, 0), 参数说明: "要创建的模型对象。"
OGRELIB_EXTERN_C void ogrelib_D3DXCreateMesh_343_ogrelib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[0].m_int;
    INT      arg2 = pArgInf[1].m_int;
    INT      arg3 = pArgInf[2].m_int;
    void*    arg4 = pArgInf[3].m_pAryData;
    void*    arg5 = pArgInf[4].m_pCompoundData;
    void*    arg6 = pArgInf[5].m_pCompoundData;

}

