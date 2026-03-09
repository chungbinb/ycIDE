#include "..\include_spec_header.h"

// 调用格式: SDT_INT 取变量数据地址, 命令说明: "取指定变量中数据的内存地址。返回值为0表示没有取到变量地址。对文本型或字节集型变量而言，如果其长度为0，将返回0；对数组变量而言，如果其成员数为0，也将返回0。对于文本型或字节集型变量，返回值为文本数据或字节集数据的地址。其它情况下，本命令与“取变量地址”具有相同的返回值。注意：一旦变量超出其作用域范围，或文本型/字节集型变量被重新赋值，或数组变量被重新赋值，先前取到的变量地址将变成无效地址，再对该地址进行读写操作很有可能导致程序崩溃。"
// 参数<1>: &变量 数组/非数组 _SDT_ALL, 参数说明: NULL
SPEC_EXTERN_C void spec_GetVarDataAddr_3_spec(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    void**   arg1 = pArgInf[0].m_ppAryData;

}

