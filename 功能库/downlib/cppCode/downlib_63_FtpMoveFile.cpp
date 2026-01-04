#include "..\include_downlib_header.h"

// 调用格式: SDT_BOOL (FTP辅助对象).移动文件, 命令说明: "移动文件或目录到新的位置"
// 参数<1>: 旧文件路径 SDT_TEXT, 参数说明: "旧文件路径"
// 参数<2>: 新文件路径 SDT_TEXT, 参数说明: "新文件路径"
DOWNLIB_EXTERN_C void downlib_FtpMoveFile_63_downlib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR    arg1 = pArgInf[1].m_pText;
    LPSTR    arg2 = pArgInf[2].m_pText;

}

