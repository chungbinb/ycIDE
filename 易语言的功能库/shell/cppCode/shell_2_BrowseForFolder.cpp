#include "..\include_shell_header.h"

// 调用格式: SDT_TEXT 浏览文件夹, 命令说明: "打开一个对话框，浏览系统中存在的所有文件夹，返回被选定文件夹的路径名，如果未选中任何文件夹，则返回空文本。"
// 参数<1>: 标题 SDT_TEXT, 参数说明: "本参数提供显示在对话框上部的标题文本"
// 参数<2>: [是否包含文件 SDT_BOOL], 参数说明: "本参数指定是否在浏览框中同时列出文件名。如果被省略，默认值为假"
// 参数<3>: [窗口句柄 SDT_TEXT], 参数说明: "默认为当前进程活动句柄，也就是焦点所在句柄"
SHELL_EXTERN_C void shell_BrowseForFolder_2_shell(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR    arg1 = pArgInf[0].m_pText;
    BOOL     arg2 = pArgInf[1].m_bool;
    LPSTR    arg3 = pArgInf[2].m_pText;

}

