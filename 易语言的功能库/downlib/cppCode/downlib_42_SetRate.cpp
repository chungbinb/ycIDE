#include "..\include_downlib_header.h"

// 调用格式: SDT_BOOL (FTP上传对象).限制速度, 命令说明: "限制上传速度。"
// 参数<1>: 速度 SDT_INT, 参数说明: "指定上传速度"
DOWNLIB_EXTERN_C void downlib_SetRate_42_downlib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;

}

