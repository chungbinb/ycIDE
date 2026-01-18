#include "..\include_esslayer_header.h"

// 调用格式: SDT_INT (保密服务器).取消息代码, 命令说明: "取得触发处理函数的消息代码。该方法必须在服务程序的“处理函数”中使用。返回1表示有客户连接到服务程序，返回2表示有客户主动或意外断开与服务程序的连接，返回3表示有客户发送数据到达服务程序，返回-1表示出错"
// 无参数
ESSLAYER_EXTERN_C void esslayer_ServerGetMsgCode_5_esslayer(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{

}

