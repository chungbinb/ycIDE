#include "..\include_ogrelib_header.h"

// 调用格式: SDT_INT (基础纹理).置精细级别, 命令说明: "设置一个受托管资源的精细级别，精细级别表示多纹理级别应该加载到显存几个级别。还表示精细级别是多少。例如：纹理被建立为256*256,这时如果设置为0表示最大层是256*256,如果设置为1表示最大层是128*128。"
// 参数<1>: 精细级别 SDT_INT, 参数说明: "要设置的新的精细级别。"
OGRELIB_EXTERN_C void ogrelib_SetLOD_24_ogrelib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;

}

