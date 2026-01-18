#include "..\include_shellex_header.h"

// 调用格式: SDT_BOOL 设置文件拖放, 命令说明: "将组件设置成拖放状态"
// 参数<1>: 标签句柄 SDT_INT, 参数说明: "使用标签的反馈事件来操作拖放后的功能，标签反馈事件传递过来的参数中参数一为拖放的控件句柄，参数二第一次为拖放的文件个数，而后为拖放的文件名"
// 参数<2>: 控件句柄 SDT_INT, 参数说明: "需要设置成拖放状态的组件的句柄"
// 变长参数, 命令参数表中最后一个参数可以被重复添加
SHELLEX_EXTERN_C void shellex_DragFiles_8_shellex(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[0].m_int;
    INT      arg2 = pArgInf[1].m_int;

}

