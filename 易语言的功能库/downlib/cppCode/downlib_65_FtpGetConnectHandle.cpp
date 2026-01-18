#include "..\include_downlib_header.h"

// 本命令被隐藏, 原始名字 = "取连接句柄"
// 调用格式: SDT_INT (FTP辅助对象).取连接句柄, 命令说明: "返回与FTP服务器的连接句柄。返回的句柄可以作为参数传给FTP上传对象或采用FTP的下载对象。这样做可以是FTP上传或下载操作基于同一个连接完成，成功返回连接句柄，失败返回-1"
// 无参数
DOWNLIB_EXTERN_C void downlib_FtpGetConnectHandle_65_downlib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{

}

