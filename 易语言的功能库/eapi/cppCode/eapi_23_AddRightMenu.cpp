#include "..\include_eapi_header.h"

// 调用格式: SDT_BOOL 添加右键菜单, 命令说明: "向指定作用区域文件右键后的菜单内添加选项"
// 参数<1>: [作用的区域或扩展名 SDT_TEXT], 参数说明: "右键菜单在什么位置出现，例如指定:.txt,那么右键.txt文件时才会出现添加的菜单，“ * ”：所有地方 ；“D”：桌面右键；“URL”：文件夹右键；“SAVER”：文件右键；“.*”：指定后缀名右键，按照.*的格式写，不存在则创建（如.txt）,默认所有地方生效"
// 参数<2>: 右键菜单标题 SDT_TEXT, 参数说明: "右键后显示的标题，同时为类名，删除时指定此参数即可"
// 参数<3>: [点击后执行的命令行 SDT_TEXT], 参数说明: "点击右键菜单触发的命令，如在.txt文件添加\"用易运行\"并绑定命令 run e.exe 文件名 点击后就会指定运行命令行"
// 参数<4>: [快捷按键 SDT_TEXT], 参数说明: "指定右键后载弹种按下某键可以快捷调用此菜单，直接填A-Z中的一个就好"
// 参数<5>: [图标路径 SDT_TEXT], 参数说明: "指定图标路径,.ico图标路径/.exe程序路径，会自动读取图标"
// 参数<6>: [仅shift显示 SDT_BOOL], 参数说明: "是否在只按住SHIFT的情况下显示（只在一级菜单中有效），默认为假"
EAPI_EXTERN_C void eapi_AddRightMenu_23_eapi(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR    arg1 = pArgInf[0].m_pText;
    LPSTR    arg2 = pArgInf[1].m_pText;
    LPSTR    arg3 = pArgInf[2].m_pText;
    LPSTR    arg4 = pArgInf[3].m_pText;
    LPSTR    arg5 = pArgInf[4].m_pText;
    BOOL     arg6 = pArgInf[5].m_bool;

}

