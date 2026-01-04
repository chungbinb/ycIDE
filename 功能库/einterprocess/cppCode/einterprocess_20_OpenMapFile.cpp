#include "..\include_einterprocess_header.h"

// 调用格式: SDT_BOOL (内存映射文件).打开映射文件, 命令说明: "打开一个其他进程创建的内存映射文件。执行成功返回真，否则返回假"
// 参数<1>: 名称 SDT_TEXT, 参数说明: "其他进程创建的内存映射文件名称"
EINTERPROCESS_EXTERN_C void einterprocess_OpenMapFile_20_einterprocess(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR    arg1 = pArgInf[1].m_pText;

}

