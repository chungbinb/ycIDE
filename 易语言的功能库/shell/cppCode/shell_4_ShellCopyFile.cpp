#include "..\include_shell_header.h"

// 调用格式: SDT_BOOL 进度复制文件, 命令说明: "本命令可以将一个或一批文件复制到另一个目录中，并同时显示一个复制进度对话框，成功返回真，失败或被用户中止复制操作返回假。"
// 参数<1>: 目的目录 SDT_TEXT, 参数说明: "本参数提供目的目录路径，如果该目录不存在，系统将自动询问是否创建"
// 参数<2>: 源文件 SDT_TEXT, 参数说明: "可以以数组方式同时提供多个文件或文件夹名，注意必须是全路径，支持通配符“*”和“?”"
SHELL_EXTERN_C void shell_ShellCopyFile_4_shell(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR    arg1 = pArgInf[0].m_pText;
    LPSTR    arg2 = pArgInf[1].m_pText;

}

