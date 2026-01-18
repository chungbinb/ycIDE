#include "..\include_shellex_header.h"

// 本命令被隐藏, 原始名字 = "设置树型框拖放"
// 调用格式: SDT_BOOL 设置树型框拖放, 命令说明: "将组件设置拖放"
// 参数<1>: 窗口句柄 SDT_INT, 参数说明: "树型框父窗口句柄"
// 参数<2>: 树型框句柄 SDT_INT, 参数说明: "需要设置成拖放状态的树型框的句柄"
// 参数<3>: 标签句柄 SDT_INT, 参数说明: "使用标签的反馈事件来操作拖放后的功能，标签反馈事件传递过来的参数中参数一为拖放的控件句柄，参数二第一次为拖放的文件个数，而后为拖放的文件名"
SHELLEX_EXTERN_C void shellex_DragTree_9_shellex(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[0].m_int;
    INT      arg2 = pArgInf[1].m_int;
    INT      arg3 = pArgInf[2].m_int;

}

