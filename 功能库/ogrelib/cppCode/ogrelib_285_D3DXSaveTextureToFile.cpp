#include "..\include_ogrelib_header.h"

// 调用格式: SDT_BOOL 保存纹理, 命令说明: "将纹理保存为图像格式的文件。"
// 参数<1>: 目标文件 SDT_TEXT, 参数说明: "要保存的目标文件名。"
// 参数<2>: 文件格式 MAKELONG(0x41, 0), 参数说明: "目标文件格式。"
// 参数<3>: 纹理 MAKELONG(0x10, 0), 参数说明: "纹理。"
OGRELIB_EXTERN_C void ogrelib_D3DXSaveTextureToFile_285_ogrelib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR    arg1 = pArgInf[0].m_pText;
    void*    arg2 = pArgInf[1].m_pCompoundData;
    void*    arg3 = pArgInf[2].m_pCompoundData;

}

