#include "..\include_eapi_header.h"

// 调用格式: SDT_INT 打开打印机对话框, 命令说明: "打开打印机设置对话框。返回1：点击了确定按钮；2：点击了取消按钮。"
// 参数<1>: [对话框类型 SDT_INT], 参数说明: "要打开的对话框类型。可以为以下两个常量值之一：0、#接口常量.标准打印设置对话框；1.打印机MFC对话框，废弃，对于MFC静态编译会出现问题 2、#接口常量.打印属性对话框。默认值为0"
// 参数<2>: [&打印信息 MAKELONG(0x0B, 0)], 参数说明: "待取得的硬盘信息"
EAPI_EXTERN_C void eapi_OpenPrintSetDlg_88_eapi(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[0].m_int;
    void**   arg2 = pArgInf[1].m_ppCompoundData;

}

