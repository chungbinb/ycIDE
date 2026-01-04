#include "..\include_internet_header.h"

// 调用格式: SDT_BOOL FTP文件改名, 命令说明: "更改FTP服务器上指定文件的名称。成功返回真，失败返回假"
// 参数<1>: 欲改名的FTP文件 SDT_TEXT, 参数说明: "本参数指定FTP服务器上欲改名文件的路径"
// 参数<2>: 新文件名称 SDT_TEXT, 参数说明: "本参数指定欲改为的文件名称"
INTERNET_EXTERN_C void internet_RenameFtpFile_13_internet(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR    arg1 = pArgInf[0].m_pText;
    LPSTR    arg2 = pArgInf[1].m_pText;

}

