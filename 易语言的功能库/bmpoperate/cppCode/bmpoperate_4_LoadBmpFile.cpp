#include "..\include_bmpoperate_header.h"

// 调用格式: SDT_BOOL (位图).载入文件, 命令说明: "从bmp文件载入位图数据，成功返回真，失败返回假。如果失败，随后立即调用本对象的“取错误信息”命令将取得一个非空文本，该文本为失败的原因。"
// 参数<1>: 位图文件名 SDT_TEXT, 参数说明: NULL
BMPOPERATE_EXTERN_C void bmpoperate_LoadBmpFile_4_bmpoperate(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR    arg1 = pArgInf[1].m_pText;

}

