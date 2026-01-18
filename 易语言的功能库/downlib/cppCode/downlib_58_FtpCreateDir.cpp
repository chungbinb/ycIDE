#include "..\include_downlib_header.h"

// 调用格式: SDT_BOOL (FTP辅助对象).创建目录, 命令说明: "在FTP服务器上指定位置处创建新的目录。成功返回真，失败返回假"
// 参数<1>: FTP目录路径 SDT_TEXT, 参数说明: "FTP目录路径"
DOWNLIB_EXTERN_C void downlib_FtpCreateDir_58_downlib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR    arg1 = pArgInf[1].m_pText;

}

