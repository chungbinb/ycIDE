#include "..\include_bmpoperate_header.h"

// 调用格式: SDT_BOOL (位图).映射文件, 命令说明: "和“载入文件”类似。二者的区别是：“载入文件”方法会一次性把整个文件读入内存中，而本方法则会使用Windows的文件映射来实现“边使用边读取”的效果。注意：使用本方法后，被映射的文件将被本对象打开并占有直到本对象被销毁"
// 参数<1>: 位图文件名 SDT_TEXT, 参数说明: NULL
// 参数<2>: DC句柄 SDT_INT, 参数说明: NULL
BMPOPERATE_EXTERN_C void bmpoperate_MapFile_33_bmpoperate(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR    arg1 = pArgInf[1].m_pText;
    INT      arg2 = pArgInf[2].m_int;

}

