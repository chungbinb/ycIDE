#include "..\include_ogrelib_header.h"

// 调用格式: SDT_BOOL (模型).读顶点数据, 命令说明: "从模型中读出顶点缓冲数据。"
// 参数<1>: [大小 SDT_INT], 参数说明: "要读数据的大小。单位为字节。"
// 参数<2>: &数据 SDT_BIN, 参数说明: "属性数据。"
OGRELIB_EXTERN_C void ogrelib_ReadVertexBuffer_375_ogrelib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;
    LPBYTE*  arg2 = pArgInf[2].m_ppBin;

}

