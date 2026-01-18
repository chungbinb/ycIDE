#include "..\include_internet_header.h"

// 调用格式: SDT_TEXT 取用户帐号, 命令说明: "返回指定拨号连接配置所使用的用户帐号名称"
// 参数<1>: 拨号网络索引 SDT_INT, 参数说明: "指定获取哪一个拨号网络配置信息，索引值从 0 开始，最大值为“取拨号连接数”命令的返回值减一"
INTERNET_EXTERN_C void internet_GetUserNameW_21_internet(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[0].m_int;

}

