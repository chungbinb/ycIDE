#include "..\include_einterprocess_header.h"

// 调用格式: SDT_BOOL (邮槽服务器).创建, 命令说明: "创建成功返回真，否则返回假"
// 参数<1>: 邮槽名称 SDT_TEXT, 参数说明: "欲在服务器上创建的邮槽名称，注意在Win9x下使用，系统最大允许名称的长度为8位，多余的会被抛弃"
EINTERPROCESS_EXTERN_C void einterprocess_CreateMailslotServer_9_einterprocess(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR    arg1 = pArgInf[1].m_pText;

}

