#include "..\include_einterprocess_header.h"

// 调用格式: SDT_BOOL (内存映射文件).读数据, 命令说明: "从已经创建或打开的内存映射文件中读出数据，成功返回真，失败返回假。注意：使用该命令时一定要注意不能超过映射文件的边界，否则会造成非法的内存访问，意外退出"
// 参数<1>: 起始位置 SDT_INT64, 参数说明: "从已经映射到内存的内存映射文件中读出的数据的起始位置。起始位置的值从0开始"
// 参数<2>: 长度 SDT_INT, 参数说明: "从已经映射到内存的内存映射文件中读出的数据的长度"
// 参数<3>: &数据 SDT_BIN, 参数说明: "从内存映射文件中读出的数据，作为方法的返回值"
EINTERPROCESS_EXTERN_C void einterprocess_ReadMapFile_22_einterprocess(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT64    arg1 = pArgInf[1].m_int64;
    INT      arg2 = pArgInf[2].m_int;
    LPBYTE*  arg3 = pArgInf[3].m_ppBin;

}

