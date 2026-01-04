#include "..\include_eapi_header.h"

// 调用格式: SDT_BOOL 取硬盘信息, 命令说明: "取硬件出厂信息。Win95/98/Me中可能无效。特殊情况下也许只能获取部分硬盘信息，其它未被成功获取的部分，将被设置为空文本或0。本命令为初级命令。"
// 参数<1>: &硬盘信息 MAKELONG(0x07, 0), 参数说明: "待取得的硬盘信息"
// 参数<2>: [硬盘序号 SDT_INT], 参数说明: "可以被省略。待取得的硬盘信息的硬盘序号。可以为以下常量之一：0、#接口常量.磁盘0；1、#接口常量.磁盘1；2、#接口常量.磁盘2；3、#接口常量.磁盘3。"
EAPI_EXTERN_C void eapi_GetHDInfo_3_eapi(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    void**   arg1 = pArgInf[0].m_ppCompoundData;
    INT      arg2 = pArgInf[1].m_int;

}

