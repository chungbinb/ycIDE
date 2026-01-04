#include "..\include_shell_header.h"

// 调用格式: SDT_TEXT 取特定目录, 命令说明: "本命令返回指定的Windows系统特定目录，所返回目录名以“\\”符号结束，如失败返回空文本。"
// 参数<1>: 欲获取目录类型 SDT_INT, 参数说明: "本参数指定欲获取目录的类型，可以为以下常量值之一： 1: #我的文档; 2: #我的收藏夹; 3: #系统桌面; 4: #系统字体; 5: #开始菜单组; 6: #程序菜单组; 7: #启动菜单组; 8: #程序数据目录; 9: #Windows安装目录; 10: #Windows系统目录; 11: #临时文件目录"
SHELL_EXTERN_C void shell_GetSpecialFolderPath_7_shell(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[0].m_int;

}

