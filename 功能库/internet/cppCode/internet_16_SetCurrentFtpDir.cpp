#include "..\include_internet_header.h"

// 调用格式: SDT_BOOL FTP置现行目录, 命令说明: "设置FTP服务器上的当前目录，设置后可以在其他FTP命令中使用相对路径来指定文件。成功返回真，失败返回假"
// 参数<1>: FTP目录路径 SDT_TEXT, 参数说明: NULL
INTERNET_EXTERN_C void internet_SetCurrentFtpDir_16_internet(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR    arg1 = pArgInf[0].m_pText;

}

