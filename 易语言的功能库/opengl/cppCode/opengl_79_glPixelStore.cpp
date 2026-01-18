#include "..\include_opengl_header.h"

// 调用格式: _SDT_NULL 设置像素存储字节数, 命令说明: "设置位图像素数据在计算机内存中存储的方式"
// 参数<1>: 存储的字节数 SDT_INT, 参数说明: "只能是1、2、4或8，其他数值无效。"
OPENGL_EXTERN_C void opengl_glPixelStore_79_opengl(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[0].m_int;

}

