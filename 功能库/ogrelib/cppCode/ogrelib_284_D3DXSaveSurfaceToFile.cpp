#include "..\include_ogrelib_header.h"

// 调用格式: SDT_BOOL 保存页面, 命令说明: "将页面保存为图像格式的文件。"
// 参数<1>: 目标文件 SDT_TEXT, 参数说明: "要保存的目标文件名。"
// 参数<2>: 文件格式 MAKELONG(0x41, 0), 参数说明: "目标文件格式。"
// 参数<3>: 页面 MAKELONG(0x08, 0), 参数说明: "页面。"
// 参数<4>: 区域 MAKELONG(0x01, 0), 参数说明: "页面的区域，注：将结构全部填为0，表示整个页面。"
OGRELIB_EXTERN_C void ogrelib_D3DXSaveSurfaceToFile_284_ogrelib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR    arg1 = pArgInf[0].m_pText;
    void*    arg2 = pArgInf[1].m_pCompoundData;
    void*    arg3 = pArgInf[2].m_pCompoundData;
    void*    arg4 = pArgInf[3].m_pCompoundData;

}

