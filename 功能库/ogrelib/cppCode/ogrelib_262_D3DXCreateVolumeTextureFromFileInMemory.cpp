#include "..\include_ogrelib_header.h"

// 调用格式: SDT_BOOL 从内存文件创建立体纹理, 命令说明: "从内存文件创建立体纹理。"
// 参数<1>: 设备 MAKELONG(0x09, 0), 参数说明: "指定与立体纹理相关联的设备。"
// 参数<2>: 源数据 SDT_BIN, 参数说明: "要创建立体纹理的内存文件数据。"
// 参数<3>: &立体纹理 MAKELONG(0x11, 0), 参数说明: "要创建的立体纹理对象。"
OGRELIB_EXTERN_C void ogrelib_D3DXCreateVolumeTextureFromFileInMemory_262_ogrelib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    void*    arg1 = pArgInf[0].m_pCompoundData;
    LPBYTE   arg2 = pArgInf[1].m_pBin;
    void**   arg3 = pArgInf[2].m_ppCompoundData;

}

