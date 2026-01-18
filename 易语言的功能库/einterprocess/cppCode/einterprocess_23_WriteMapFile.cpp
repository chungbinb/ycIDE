#include "..\include_einterprocess_header.h"

// 调用格式: SDT_BOOL (内存映射文件).写数据, 命令说明: "向已经创建或打开的内存映射文件中写入数据，成功写入返回真，失败返回假。注意：使用该命令时一定要注意不能超过映射文件的边界，否则会造成非法的内存访问，意外退出"
// 参数<1>: 起始位置 SDT_INT64, 参数说明: "向内存映射文件中写入的数据的起始位置。起始位置的值从0开始"
// 参数<2>: 数据 SDT_BIN, 参数说明: "欲写入的数据。注意写数据时，数据的长度不能超过映射文件的边界"
EINTERPROCESS_EXTERN_C void einterprocess_WriteMapFile_23_einterprocess(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT64    arg1 = pArgInf[1].m_int64;
    LPBYTE   arg2 = pArgInf[2].m_pBin;

}

