#include "..\include_ogrelib_header.h"

// 调用格式: SDT_BOOL (顶点缓冲).读出数据, 命令说明: "从当前资源中读出一块数据。"
// 参数<1>: 偏移 SDT_INT, 参数说明: "要写入数据的偏移，表示从哪儿开始读。单位为字节。"
// 参数<2>: 大小 SDT_BIN, 参数说明: "要读入数据的大小。注：此参数不能指定为0。"
// 参数<3>: &数据 SDT_BIN, 参数说明: "返回的数据。"
OGRELIB_EXTERN_C void ogrelib_ReadData_89_ogrelib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;
    LPBYTE   arg2 = pArgInf[2].m_pBin;
    LPBYTE*  arg3 = pArgInf[3].m_ppBin;

}

