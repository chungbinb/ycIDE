#include "..\include_downlib_header.h"

// 调用格式: SDT_BOOL (FTP辅助对象).置现行目录, 命令说明: "设置FTP服务器上的当前目录，设置后可以在其他FTP命令中使用相对路径来指定文件。成功返回真，失败返回假"
// 参数<1>: FTP目录路径 SDT_TEXT, 参数说明: "FTP目录路径"
DOWNLIB_EXTERN_C void downlib_FtpSetCurrentDir_60_downlib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR    arg1 = pArgInf[1].m_pText;

}

