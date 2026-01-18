#include "..\include_downlib_header.h"

// 调用格式: SDT_TEXT (下载对象).取本地文件路径, 命令说明: "获得待下载文件保存在本地的路径。本命令在收到”存在同名文件“消息时使用可以获得已经存在的同名文件的路径。本命令在收到“下载上传开始”消息后使用可以获得当前下载的文件保存在本地的准确路径"
// 无参数
DOWNLIB_EXTERN_C void downlib_GetFilePath_32_downlib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{

}

