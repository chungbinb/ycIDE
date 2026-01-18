#include "..\include_ogrelib_header.h"

// 调用格式: SDT_BOOL (模型).灵活顶点克隆模型, 命令说明: "用指定的灵活顶点克隆一个模型。"
// 参数<1>: 选项 MAKELONG(0x42, 0), 参数说明: "建立模型的选项。可以是一个或多个“模型创建标志”常量数据类型中的值。"
// 参数<2>: 灵活顶点 MAKELONG(0x3C, 0), 参数说明: "指定的灵活顶点格式。"
// 参数<3>: 设备 MAKELONG(0x09, 0), 参数说明: "输出模型在哪个设备中。"
// 参数<4>: 输出模型 MAKELONG(0x1A, 0), 参数说明: "输出的模型。"
OGRELIB_EXTERN_C void ogrelib_CloneMeshFVF_357_ogrelib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    void*    arg1 = pArgInf[1].m_pCompoundData;
    void*    arg2 = pArgInf[2].m_pCompoundData;
    void*    arg3 = pArgInf[3].m_pCompoundData;
    void*    arg4 = pArgInf[4].m_pCompoundData;

}

