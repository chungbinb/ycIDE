#include "..\include_bmpoperate_header.h"

// 调用格式: MAKELONG(0x01, 0) (位图).镜像, 命令说明: "将该位图做水平/垂直镜像，并返回镜像后的新位图。失败返回空位图。如果失败，随后立即调用本对象的“取错误信息”命令将取得一个非空文本，该文本为失败的原因。"
// 参数<1>: 是否水平镜像 SDT_BOOL, 参数说明: NULL
// 参数<2>: 是否垂直镜像 SDT_BOOL, 参数说明: NULL
BMPOPERATE_EXTERN_C void bmpoperate_Mirror_25_bmpoperate(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    BOOL     arg1 = pArgInf[1].m_bool;
    BOOL     arg2 = pArgInf[2].m_bool;

}

