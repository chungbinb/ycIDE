#include "..\include_shell_header.h"

// 调用格式: SDT_BOOL 关闭系统, 命令说明: "本命令可以执行关机、重启、注销等Windows系统操作。成功返回真，失败返回假。"
// 参数<1>: 关闭方式 SDT_INT, 参数说明: "本参数可以为以下常量值之一： 1: #关机; 2: #重启; 3: #注销; 4: #休眠 5: #冬眠。 注意在Windows95/98/ME下冬眠等同于休眠"
// 参数<2>: [是否强制执行 SDT_BOOL], 参数说明: "本参数指定是否不等待当前正在运行的所有软件正常结束而立即执行指定的操作。如果本参数被省略，默认值为假"
SHELL_EXTERN_C void shell_ExitWindows_8_shell(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[0].m_int;
    BOOL     arg2 = pArgInf[1].m_bool;

}

