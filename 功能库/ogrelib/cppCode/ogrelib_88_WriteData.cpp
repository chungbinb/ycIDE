#include "..\include_ogrelib_header.h"

// 调用格式: SDT_BOOL (顶点缓冲).写入数据, 命令说明: "在当前资源中写入一块数据。"
// 参数<1>: 偏移 SDT_INT, 参数说明: "要写入数据的偏移，表示从哪儿开始写。单位为字节。注：如果要写入整个缓冲，请将本参数和下一个参数置0。"
// 参数<2>: 数据 SDT_BIN, 参数说明: "要写入的数据。"
OGRELIB_EXTERN_C void ogrelib_WriteData_88_ogrelib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;
    LPBYTE   arg2 = pArgInf[2].m_pBin;

}

