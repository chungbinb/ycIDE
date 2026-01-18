#include "..\include_internet_header.h"

// 调用格式: SDT_BOOL FTP文件下载, 命令说明: "将FTP服务器上的指定文件下载到本地，成功返回真，失败返回假"
// 参数<1>: 欲下载的FTP文件 SDT_TEXT, 参数说明: "本参数指定在FTP服务器上欲下载文件的路径"
// 参数<2>: 本地文件名 SDT_TEXT, 参数说明: "本参数指定被下载文件的本地保存位置"
// 参数<3>: [是否使用ASCII码方式 SDT_BOOL], 参数说明: "指定是否使用ASCII编码方式传递文件数据。如果本参数被省略，默认值为假，即使用二进制方式传送"
INTERNET_EXTERN_C void internet_GetFtpFile_10_internet(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR    arg1 = pArgInf[0].m_pText;
    LPSTR    arg2 = pArgInf[1].m_pText;
    BOOL     arg3 = pArgInf[2].m_bool;

}

