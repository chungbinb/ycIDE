#include "..\include_internet_header.h"

// 调用格式: SDT_BOOL FTP删除文件, 命令说明: "删除FTP服务器上的指定文件。成功返回真，失败返回假"
// 参数<1>: 欲删除的FTP文件 SDT_TEXT, 参数说明: "本参数指定在FTP服务器上欲删除文件的路径"
INTERNET_EXTERN_C void internet_DeleteFtpFile_12_internet(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR    arg1 = pArgInf[0].m_pText;

}

