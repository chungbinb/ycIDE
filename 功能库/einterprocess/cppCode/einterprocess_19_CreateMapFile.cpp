#include "..\include_einterprocess_header.h"

// 调用格式: SDT_BOOL (内存映射文件).创建映射文件, 命令说明: "创建内存映射文件。执行成功返回真，否则返回假"
// 参数<1>: 文件名称 SDT_TEXT, 参数说明: "欲创建内存映射文件的文件全路径名称"
// 参数<2>: 是否创建 SDT_BOOL, 参数说明: "是打开一个已经存在的文件还是创建一个新文件。真代表创建一个新文件，假代表打开一个已经存在的文件"
// 参数<3>: 文件大小 SDT_INT64, 参数说明: "欲创建内存映射文件的文件大小。如果是打开一个已经存在的文件，可以指定该值为0，表示创建的内存映射文件大小和文件本身大小是一致的"
// 参数<4>: [名称 SDT_TEXT], 参数说明: "创建内存映射文件的名称，该参数可以为空，表示创建一个没有名称的内存映射文件。其他进程可以通过该名称打开内存映射文件"
EINTERPROCESS_EXTERN_C void einterprocess_CreateMapFile_19_einterprocess(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR    arg1 = pArgInf[1].m_pText;
    BOOL     arg2 = pArgInf[2].m_bool;
    INT64    arg3 = pArgInf[3].m_int64;
    LPSTR    arg4 = pArgInf[4].m_pText;

}

