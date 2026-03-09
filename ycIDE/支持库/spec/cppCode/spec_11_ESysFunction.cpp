#include "..\include_spec_header.h"

// 调用格式: SDT_INT 调用易系统功能, 命令说明: "具体的功能号及其所对应的参数值和返回值，请参考易语言支持库开发手册。"
// 参数<1>: 功能号 SDT_INT, 参数说明: "可以为易语言支持库开发手册中定义的以“NES_”,“NRS_”,“NAS_”开头的常量之一。"
// 参数<2>: [参数一 SDT_INT], 参数说明: "如果本参数被省略，默认为0。"
// 参数<3>: [参数二 SDT_INT], 参数说明: "如果本参数被省略，默认为0。"
SPEC_EXTERN_C void spec_ESysFunction_11_spec(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[0].m_int;
    INT      arg2 = pArgInf[1].m_int;
    INT      arg3 = pArgInf[2].m_int;

}

