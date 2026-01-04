#include "..\include_eapi_header.h"

// 调用格式: SDT_INT 安装字体, 命令说明: "在Windows系统中添加一种字体资源。添加完毕后，该字体即可由任何Windows应用程序调用。成功返回1,失败返回0，如加载内存资源则返回安装临时字体的句柄，字体资源进程之间不可共享，可通过删除字体提前删除资源；"
// 参数<1>: 字体文件名或字体资源 _SDT_ALL, 参数说明: "字体资源文件的文件名。可以是.FON，.FNT，.TTF或.FOT文件。或者字体资源"
EAPI_EXTERN_C void eapi_AddFont_29_eapi(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    PVOID    arg1 = pArgInf[0].m_pByte;

}

