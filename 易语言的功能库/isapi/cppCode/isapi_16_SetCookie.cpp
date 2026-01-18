#include "..\include_isapi_header.h"

// 调用格式: SDT_BOOL (互联网扩展).设置存根, 命令说明: "设定存根(Cookie)。注本功能需要浏览器的支持。"
// 参数<1>: 名称 SDT_TEXT, 参数说明: "本参数表示存根(Cookie)的名称。"
// 参数<2>: 路径 SDT_TEXT, 参数说明: "限定浏览器发送存根(Cookie)的路径，如果此参数指定则只有在浏览此路径下的网页时存根(Cookie)才会被发送。注：发送路径时请注意路径的大小写要和(Windows)目录的一致。"
// 参数<3>: [网站 SDT_TEXT], 参数说明: "限定了存根(Cookie)发送的网站。"
// 参数<4>: 安全 SDT_BOOL, 参数说明: "本参数为真表示传递中就实行了加密算法，如果你正在使用安全接口层，那么你就可以使用这个属性。安全接口层在IIS中设置。"
// 参数<5>: 失效日期 SDT_DATE_TIME, 参数说明: "本参数表示到到日期后存根(Cookie)就失效。"
// 参数<6>: 变量分割符 SDT_BYTE, 参数说明: "本参数表示如果有多个变量的话它们之间用什么符号分割。黙认为'&'。"
// 参数<7>: 变量名称和值 SDT_TEXT, 参数说明: "本参数的每一对参数值顺序指定变量名称及其相应的值。"
// 变长参数, 命令参数表中最后一个参数可以被重复添加
ISAPI_EXTERN_C void isapi_SetCookie_16_isapi(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR    arg1 = pArgInf[1].m_pText;
    LPSTR    arg2 = pArgInf[2].m_pText;
    LPSTR    arg3 = pArgInf[3].m_pText;
    BOOL     arg4 = pArgInf[4].m_bool;
    DATE     arg5 = pArgInf[5].m_date;
    BYTE     arg6 = pArgInf[6].m_byte;
    LPSTR    arg7 = pArgInf[7].m_pText;

}

