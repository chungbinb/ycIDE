#include "..\include_ogrelib_header.h"

// 调用格式: SDT_BOOL (设备).设置纹理, 命令说明: "为设备设置一层纹理。"
// 参数<1>: 纹理层 SDT_INT, 参数说明: "要指定的纹理层。"
// 参数<2>: 纹理 _SDT_ALL, 参数说明: "指定的纹理, 本参数可以是“纹理”、“基础纹理”、“立体纹理”、“立方体纹理”数据类型。"
OGRELIB_EXTERN_C void ogrelib_SetTexture_134_ogrelib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;
    PVOID    arg2 = pArgInf[2].m_pByte;

}

