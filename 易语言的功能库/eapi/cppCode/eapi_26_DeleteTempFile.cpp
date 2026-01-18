#include "..\include_eapi_header.h"

// 调用格式: SDT_BOOL 删除临时文件, 命令说明: "删除指定位置的临时文件。成功返回真；失败返回假。"
// 参数<1>: [临时文件类型 SDT_INT], 参数说明: "可以被省略。临时文件类型，可以为以下常量值之一：0、#接口常量.系统临时文件；1、#接口常量.IE临时文件；2、#接口常量.回收站；3、#接口常量.存根文件。"
EAPI_EXTERN_C void eapi_DeleteTempFile_26_eapi(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[0].m_int;

}

