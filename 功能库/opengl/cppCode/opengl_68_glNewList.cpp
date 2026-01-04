#include "..\include_opengl_header.h"

// 调用格式: _SDT_NULL 开始创建列表, 命令说明: NULL
// 参数<1>: 列表号 SDT_INT, 参数说明: "可以直接指定列表号或者使用由“生成列表号”取得的返回值，必须是大于0的整数。"
// 参数<2>: 列表模式 SDT_INT, 参数说明: "参数值可以为以下常量：\r\n\t4864、#编译列表； \r\n\t4865、#编译并且执行列表"
OPENGL_EXTERN_C void opengl_glNewList_68_opengl(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[0].m_int;
    INT      arg2 = pArgInf[1].m_int;

}

