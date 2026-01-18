#include "..\include_eapi_header.h"

// 调用格式: SDT_BOOL 设置窗口透明度, 命令说明: "成功返回真；失败返回假。"
// 参数<1>: 窗口句柄 SDT_INT, 参数说明: "要设置透明度的窗口句柄。"
// 参数<2>: [透明度 SDT_INT], 参数说明: "墙纸样式，可以为以下常量值之一：0、#接口常量.居中；1、#接口常量.平铺；2、#接口常量.拉伸。。"
// 参数<3>: [透明色 SDT_INT], 参数说明: "(#颜色)指定某颜色为完全透明(完全透明处不属于该窗口) 可空为不指定."
// 参数<4>: [鼠标穿透 SDT_BOOL], 参数说明: "设置鼠标穿透，默认为假"
EAPI_EXTERN_C void eapi_SetDiaphaneity_66_eapi(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[0].m_int;
    INT      arg2 = pArgInf[1].m_int;
    INT      arg3 = pArgInf[2].m_int;
    BOOL     arg4 = pArgInf[3].m_bool;

}

