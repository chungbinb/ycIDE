#include "..\include_eapi_header.h"

// 调用格式: SDT_BOOL 取键盘指示灯状态, 命令说明: "取得三个键盘提示灯当前状态。开返回真；关返回假。本命令为初级命令。"
// 参数<1>: [指示灯类别 SDT_INT], 参数说明: "可以被省略。键盘指示灯类别，为以下常量值之一：0、#接口常量.数字锁定键盘灯； 1、#接口常量.大小写锁定键盘灯； 2、#接口常量.滚动锁定键盘灯"
EAPI_EXTERN_C void eapi_GetKeyboardLockState_0_eapi(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[0].m_int;

}

