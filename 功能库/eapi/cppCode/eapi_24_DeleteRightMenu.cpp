#include "..\include_eapi_header.h"

// 调用格式: SDT_BOOL 删除右键菜单, 命令说明: "删除指定标题或作用区域右键菜单"
// 参数<1>: [作用的区域或扩展名 SDT_TEXT], 参数说明: "右键菜单在什么位置出现，例如指定:.txt,那么右键.txt文件时才会出现添加的菜单，“ * ”：所有地方 ；“D”：桌面右键；“URL”：文件夹右键；“SAVER”：文件右键；“.*”：指定后缀名右键，按照.*的格式写，不存在则创建（如.txt）,默认所有地方生效"
// 参数<2>: 右键菜单标题 SDT_TEXT, 参数说明: "右键后显示的标题，同时为类名，删除时指定此参数即可"
EAPI_EXTERN_C void eapi_DeleteRightMenu_24_eapi(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR    arg1 = pArgInf[0].m_pText;
    LPSTR    arg2 = pArgInf[1].m_pText;

}

