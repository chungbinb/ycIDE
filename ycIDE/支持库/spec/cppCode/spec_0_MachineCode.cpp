#include "..\include_spec_header.h"

// 本命令被隐藏, 原始名字 = "置入代码"
// 调用格式: _SDT_NULL 置入代码, 命令说明: "在编译后文件代码段中当前语句位置置入指定的机器指令码数据"
// 参数<1>: 代码数据 _SDT_ALL, 参数说明: "欲置入到代码段中的机器指令码数据，可以是字节集数据或二进制文件名文本"
SPEC_EXTERN_C void spec_MachineCode_0_spec(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    PVOID    arg1 = pArgInf[0].m_pByte;

}

