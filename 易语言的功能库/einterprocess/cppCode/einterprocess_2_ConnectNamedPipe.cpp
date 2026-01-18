#include "..\include_einterprocess_header.h"

// 调用格式: SDT_INT 连接命名管道, 命令说明: "连接一个已经创建完毕并在监听的命名管道。连接成功返回管道的句柄，失败返回0"
// 参数<1>: 命名管道名称 SDT_TEXT, 参数说明: "欲连接命名管道的名称"
// 参数<2>: 超时时间 SDT_INT, 参数说明: "连接命名管道时的等待时间，-1为无限等待，其他为具体的等待时间，单位是毫秒"
EINTERPROCESS_EXTERN_C void einterprocess_ConnectNamedPipe_2_einterprocess(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR    arg1 = pArgInf[0].m_pText;
    INT      arg2 = pArgInf[1].m_int;

}

