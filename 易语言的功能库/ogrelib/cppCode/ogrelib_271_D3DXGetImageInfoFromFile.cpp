#include "..\include_ogrelib_header.h"

// 调用格式: SDT_BOOL 从文件中取图像, 命令说明: "从指定的文件中获取图像信息。"
// 参数<1>: 文件 SDT_TEXT, 参数说明: "指定的文件名。"
// 参数<2>: 图像信息 MAKELONG(0x28, 0), 参数说明: "要保存的图像信息。"
OGRELIB_EXTERN_C void ogrelib_D3DXGetImageInfoFromFile_271_ogrelib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR    arg1 = pArgInf[0].m_pText;
    void*    arg2 = pArgInf[1].m_pCompoundData;

}

