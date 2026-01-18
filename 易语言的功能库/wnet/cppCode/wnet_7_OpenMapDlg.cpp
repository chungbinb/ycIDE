#include "..\include_wnet_header.h"

// 调用格式: SDT_TEXT (局域网操作).打开映射对话框, 命令说明: "打开系统“映射网络驱动器”对话框。如果通过该对话框成功映射了一个网络驱动器，返回该驱动器的名称，否则返回空文本"
// 参数<1>: 网络路径初始值 SDT_TEXT, 参数说明: "本参数指定出现在“映射网络驱动器”对话框中网络路径编辑框中的初始值"
// 参数<2>: 网络路径可否输入 SDT_BOOL, 参数说明: "本参数指定对话框中的网络路径编辑框是否可用"
// 参数<3>: 重新连接是否可选 SDT_BOOL, 参数说明: "本参数指定对话框中“登录时重新连接”复选框是否可用"
// 参数<4>: 是否自动连接 SDT_BOOL, 参数说明: "本参数指定对话框中“登录时重新连接”复选框是否被选中"
// 参数<5>: [父窗口句柄 SDT_INT], 参数说明: "本参数指定打开的对话框的父窗口"
WNET_EXTERN_C void wnet_OpenMapDlg_7_wnet(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR    arg1 = pArgInf[1].m_pText;
    BOOL     arg2 = pArgInf[2].m_bool;
    BOOL     arg3 = pArgInf[3].m_bool;
    BOOL     arg4 = pArgInf[4].m_bool;
    INT      arg5 = pArgInf[5].m_int;

}

