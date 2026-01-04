#include "..\include_ogrelib_header.h"

// 调用格式: SDT_BOOL 从文件创建纹理, 命令说明: "从指定的文件创建纹理。"
// 参数<1>: 设备 MAKELONG(0x09, 0), 参数说明: "指定与纹理相关联的设备。"
// 参数<2>: 文件名 SDT_TEXT, 参数说明: "指定文件名。"
// 参数<3>: 纹理 MAKELONG(0x0F, 0), 参数说明: "要创建的纹理对象。"
OGRELIB_EXTERN_C void ogrelib_D3DXCreateTextureFromFile_255_ogrelib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    void*    arg1 = pArgInf[0].m_pCompoundData;
    LPSTR    arg2 = pArgInf[1].m_pText;
    void*    arg3 = pArgInf[2].m_pCompoundData;

}

