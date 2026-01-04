#include "..\include_eapi_header.h"

// 调用格式: SDT_TEXT []取驱动器列表, 命令说明: "取当前指定类型的驱动器列表。成功返回驱动器列表数组；失败返回空。本命令为初级命令。"
// 参数<1>: [驱动器类型 SDT_INT], 参数说明: "可以被省略。驱动器的类型，可以为以下常量值之一：0、#接口常量.所有驱动器；1、#接口常量.硬盘驱动器；2、#接口常量.光盘驱动器；3、#接口常量.可移动驱动器；4、#接口常量.网络驱动器。"
EAPI_EXTERN_C void eapi_GetDrivesList_5_eapi(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[0].m_int;

}

