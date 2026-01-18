#include "..\include_shell_header.h"

// 调用格式: SDT_BOOL 删除到回收站, 命令说明: "本命令可以将一个或一批文件删除到Windows系统回收站中，成功返回真，失败或被用户中止删除操作返回假。"
// 参数<1>: [删除选项 SDT_INT], 参数说明: "本参数提供删除操作选项，可以为以下常量值之一或之和： 1: #不询问删除; 2: #不提示错误; 4: #不显示进度。如果被省略，默认值为 0 "
// 参数<2>: 欲删除文件 SDT_TEXT, 参数说明: "本参数提供显示在对话框上部的标题文本"
SHELL_EXTERN_C void shell_DeleteIntoRecycleBin_3_shell(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[0].m_int;
    LPSTR    arg2 = pArgInf[1].m_pText;

}

