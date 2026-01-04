#include "..\include_javalib_header.h"

// 本命令被隐藏, 原始名字 = "连接"
// 调用格式: SDT_BOOL (Java虚拟机).连接, 命令说明: "将当前线程连接到Java虚拟机。试图连接已经连接的线程将不执行任何操作。本地线程不能同时连接到两个Java虚拟机上。成功返回“真”，失败返回“假”。"
// 无参数
JAVALIB_EXTERN_C void javalib_AttachCurrentThread_3_javalib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{

}

