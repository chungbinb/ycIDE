#include "..\include_ewizard_header.h"

// 调用格式: SDT_TEXT 写出程序, 命令说明: "根据已有的模板程序修改记录对模板程序进行实际修改。如果修改成功返回空文本，否则返回具体的错误信息。修改成功后的结果在向导程序退出后将被易系统自动载入编辑。无论写出成功或者失败，本命令退出时都会自动清除所有模板程序修改信息"
// 参数<1>: 模板程序数据 SDT_BIN, 参数说明: "此数据为用作模板程序的完整易源程序"
// 参数<2>: [是否关闭源程序 SDT_BOOL], 参数说明: "如果本参数为真，则当执行写出程序时，先将现行易语言编辑环境中的源程序关闭，然后将所写出程序作为新的完整程序载入。如果本参数为假，当执行写出程序时，会将所生成程序与现行易语言编辑环境中的源程序合并（合并前“_启动窗口”及其窗口程序集、“启动子程序”代码将被抛弃）。如果本参数被省略，默认值为真"
EWIZARD_EXTERN_C void ewizard_WritePrg_13_ewizard(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPBYTE   arg1 = pArgInf[0].m_pBin;
    BOOL     arg2 = pArgInf[1].m_bool;

}

