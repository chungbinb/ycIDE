#include "..\include_opengl_header.h"

// 调用格式: _SDT_NULL 清除缓存, 命令说明: "指定清除的一项或几项缓存。"
// 参数<1>: 缓存 SDT_INT, 参数说明: "可以使用下列参数值的位或后的数值。参数值可以为以下常量：\r\n\t256、#深度缓存位，将深度缓存恢复到默认值；\r\n\t512、#累积缓存位，将累积缓存恢复到默认值；\r\n\t1024、#模板缓存位，将模板缓存恢复到默认值；\r\n\t16384、#颜色缓存位，将颜色缓存恢复到默认值"
OPENGL_EXTERN_C void opengl_glClear_42_opengl(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[0].m_int;

}

