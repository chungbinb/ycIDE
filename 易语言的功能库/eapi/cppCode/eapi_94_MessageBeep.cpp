#include "..\include_eapi_header.h"

// 调用格式: _SDT_NULL 蜂鸣, 命令说明: "通过计算机媒体设备或者喇叭发出一个声音。"
// 参数<1>: [ 默认类型 SDT_INT], 参数说明: " 默认: 0,标准: 1,拒绝 :2,询问 3,感叹 48,特别 64"
EAPI_EXTERN_C void eapi_MessageBeep_94_eapi(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[0].m_int;

}

