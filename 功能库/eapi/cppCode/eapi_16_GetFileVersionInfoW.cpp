#include "..\include_eapi_header.h"

// 调用格式: SDT_BOOL 取文件版本信息, 命令说明: "取文件信息版本。"
// 参数<1>: 文件名 SDT_TEXT, 参数说明: "要取得版本信息的文件名。"
// 参数<2>: &文件版本信息 MAKELONG(0x05, 0), 参数说明: "待取得的文件版本信息。"
EAPI_EXTERN_C void eapi_GetFileVersionInfoW_16_eapi(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR    arg1 = pArgInf[0].m_pText;
    void**   arg2 = pArgInf[1].m_ppCompoundData;

}

