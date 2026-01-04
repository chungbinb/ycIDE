#include "..\include_eapi_header.h"

// 调用格式: _SDT_ALL 截取屏幕区域, 命令说明: "将选定的屏幕区域拷贝到位图文件、剪贴板或字节集中。如拷贝到位图文件或剪贴板，则成功返回真；失败返回假。如拷贝到字节集，则成功返回位图字节集；失败返回空。"
// 参数<1>: [屏幕区域 MAKELONG(0x02, 0)], 参数说明: "要截取的屏幕区域，为空则截取整个屏幕。"
// 参数<2>: [截取到 SDT_INT], 参数说明: "要截取屏幕区域到何处，可以为以下常量值之一：0、#接口常量.到文件； 1、#接口常量.到剪贴板； 2、#接口常量.到字节集。"
// 参数<3>: [屏幕区域 SDT_TEXT], 参数说明: NULL
EAPI_EXTERN_C void eapi_GetScreenBitma_74_eapi(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    void*    arg1 = pArgInf[0].m_pCompoundData;
    INT      arg2 = pArgInf[1].m_int;
    LPSTR    arg3 = pArgInf[2].m_pText;

}

