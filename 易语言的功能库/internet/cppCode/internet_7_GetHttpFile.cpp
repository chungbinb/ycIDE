#include "..\include_internet_header.h"

// 调用格式: SDT_BIN HTTP读文件, 命令说明: "获取互联网上指定HTTP地址处的网页或数据文件，返回文件内容字节集"
// 参数<1>: HTTP文件地址 SDT_TEXT, 参数说明: "本参数指定欲读取文件在互联网上的HTTP地址（即URL）"
INTERNET_EXTERN_C void internet_GetHttpFile_7_internet(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR    arg1 = pArgInf[0].m_pText;

}

