#include "..\include_spec_header.h"

// 调用格式: SDT_INT 取变量地址, 命令说明: "返回指定变量的内存地址（在内联汇编代码中可以在执行本命令后立即检查EAX寄存器获得），如欲修改该地址处内容（尤其是文本型、字节集型或复合数据类型变量），请谨慎操作，否则很可能造成内存垃圾或程序崩溃"
// 参数<1>: &欲获取其地址的变量 数组/非数组 _SDT_ALL, 参数说明: NULL
SPEC_EXTERN_C void spec_GetVarAddress_1_spec(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    void**   arg1 = pArgInf[0].m_ppAryData;

}

