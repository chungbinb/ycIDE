#include "..\include_downlib_header.h"

// 调用格式: SDT_BOOL (FTP辅助对象).删除目录, 命令说明: "删除FTP服务器上的指定目录。成功返回真，失败返回假"
// 参数<1>: FTP目录路径 SDT_TEXT, 参数说明: "FTP目录路径"
DOWNLIB_EXTERN_C void downlib_FtpDelDir_59_downlib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR    arg1 = pArgInf[1].m_pText;

}

