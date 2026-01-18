#include "..\include_ogrelib_header.h"

// 调用格式: SDT_BOOL (设备).置对话框模式, 命令说明: "设置是否在全屏模式应用程序中使用GDI对话框。"
// 参数<1>: 启用对话框 SDT_BOOL, 参数说明: "真为启用对话框，假为禁用对话框。"
OGRELIB_EXTERN_C void ogrelib_SetDialogBoxMode_164_ogrelib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    BOOL     arg1 = pArgInf[1].m_bool;

}

