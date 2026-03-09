#include "..\include_spec_header.h"

// 调用格式: SDT_INT 申请内存, 命令说明: "向易语言运行时系统申请指定大小的内存区域。执行成功返回申请到的内存首地址，失败返回0。由本命令申请的内存必须通过“释放内存”命令释放。"
// 参数<1>: 欲申请的内存字节数 SDT_INT, 参数说明: NULL
// 参数<2>: [是否清零 SDT_BOOL], 参数说明: "如果本参数为“真”，则将申请到的内存区域全部以0填充。如果省略本参数，默认为“假”。"
SPEC_EXTERN_C void spec_AllocMem_9_spec(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[0].m_int;
    BOOL     arg2 = pArgInf[1].m_bool;

}

