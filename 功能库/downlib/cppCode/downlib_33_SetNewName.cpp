#include "..\include_downlib_header.h"

// 调用格式: SDT_BOOL (下载对象).设新文件名, 命令说明: "本命令用来设置新的本地文件名。本命令必须是在处理“存在同名文件”这个消息的过程中使用。并且返回 #设置新名"
// 参数<1>: 新文件名 SDT_TEXT, 参数说明: "新的本地文件名"
DOWNLIB_EXTERN_C void downlib_SetNewName_33_downlib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR    arg1 = pArgInf[1].m_pText;

}

