#include "..\include_downlib_header.h"

// 调用格式: SDT_BOOL (FTP上传对象).设新文件名, 命令说明: "本命令用来设置新的上传文件名"
// 参数<1>: 新文件名 SDT_TEXT, 参数说明: "上传到服务器的文件的新名"
DOWNLIB_EXTERN_C void downlib_SetNewName_46_downlib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR    arg1 = pArgInf[1].m_pText;

}

