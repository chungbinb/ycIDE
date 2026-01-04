#include "..\include_eapi_header.h"

// 调用格式: SDT_INT 提取资源文件图标, 命令说明: "从指定的资源文件中提取图标，并保存为ICO、BMP文件或复制到剪贴板。成功返回图标数量；失败返回0。"
// 参数<1>: 资源文件名 SDT_TEXT, 参数说明: "要提取图标的资源文件名。扩展名为EXE、DLL、ICL、ICO。"
// 参数<2>: [是否提取到剪辑板 SDT_INT], 参数说明: "0为假其余为真，兼容之前易库,默认为假"
// 参数<3>: [保存文件名 SDT_TEXT], 参数说明: "如需要提取到文件则填写提取的图标要保存的文件名。扩展名为ICO、BMP。，默认为空"
// 参数<4>: [图标序号 SDT_INT], 参数说明: "当资源文件中包含多个图标时，表示要提取图标的序号。"
EAPI_EXTERN_C void eapi_GetIconFromResource_33_eapi(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR    arg1 = pArgInf[0].m_pText;
    INT      arg2 = pArgInf[1].m_int;
    LPSTR    arg3 = pArgInf[2].m_pText;
    INT      arg4 = pArgInf[3].m_int;

}

