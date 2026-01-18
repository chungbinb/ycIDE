#include "..\include_downlib_header.h"

// 调用格式: SDT_BOOL (FTP辅助对象).删除文件, 命令说明: "删除FTP服务器上的指定文件。成功返回真，失败返回假"
// 参数<1>: 欲删除的FTP文件 SDT_TEXT, 参数说明: "本参数指定在FTP服务器上欲删除文件的路径"
DOWNLIB_EXTERN_C void downlib_FtpDelFile_56_downlib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR    arg1 = pArgInf[1].m_pText;

}

