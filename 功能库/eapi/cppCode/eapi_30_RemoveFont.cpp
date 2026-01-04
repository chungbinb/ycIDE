#include "..\include_eapi_header.h"

// 调用格式: SDT_BOOL 删除字体, 命令说明: "在从Windows系统中删除一种字体资源。提供临时字体的句柄，则会立即卸载临时字体，如删除系统的字体目前正由其他应用程序使用，则并不将其立即删除。成功返回真；失败返回假。"
// 参数<1>: 字体名或字体句柄 _SDT_ALL, 参数说明: "字体资源文件的文件名。可以是.FON，.FNT，.TTF或.FOT文件。或者字体资源"
EAPI_EXTERN_C void eapi_RemoveFont_30_eapi(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    PVOID    arg1 = pArgInf[0].m_pByte;

}

