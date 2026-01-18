#include "..\include_einterprocess_header.h"

// 调用格式: SDT_INT 创建命名管道, 命令说明: "命名管道可在不同进程之间，支持可靠的、单向或双向的数据通信。Win9x操作系统下不支持该命令。创建成功返回命名管道的句柄，否则返回0"
// 参数<1>: 名称 SDT_TEXT, 参数说明: "欲创建命名管道的名称"
EINTERPROCESS_EXTERN_C void einterprocess_CreateNamedPipeW_0_einterprocess(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR    arg1 = pArgInf[0].m_pText;

}

