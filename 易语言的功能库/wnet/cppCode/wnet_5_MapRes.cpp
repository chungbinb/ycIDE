#include "..\include_wnet_header.h"

// 调用格式: SDT_TEXT (局域网操作).映射资源, 命令说明: "映射一个局域网内的共享资源到本地驱动器。成功返回映射到本地的驱动器名称（比如“X:”），失败返回空文本"
// 参数<1>: 欲映射的资源路径 SDT_TEXT, 参数说明: "该参数可以为局域网中的共享文件夹或共享打印机的路径，比如“\\\\Prog5\\共享测试”，“\\\\Prog5\\惠普打印机”"
// 参数<2>: [欲映射到的本地设备名称 SDT_TEXT], 参数说明: "比如“Y:”，“LPT1”。如果本参数省略，系统将自动选择一个可用的设备名称"
// 参数<3>: [用户名 SDT_TEXT], 参数说明: "访问指定的共享使用的用户名。如果本参数省略，将使用系统默认值"
// 参数<4>: [密码 SDT_TEXT], 参数说明: "访问指定的共享使用的密码。如果本参数省略，将使用系统默认值"
// 参数<5>: [是否提示错误 SDT_BOOL], 参数说明: "本参数指定当用户名或密码错误时，是否提示重新输入。如果提供的用户名或密码有误，且本参数为假，该命令将失败。如果本参数省略，默认值为“真”"
// 参数<6>: [是否自动重新连接 SDT_BOOL], 参数说明: "本参数指定是否在下次登陆计算机时自动重新连接该映射驱动器。如果本参数省略，默认值为“真”"
WNET_EXTERN_C void wnet_MapRes_5_wnet(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR    arg1 = pArgInf[1].m_pText;
    LPSTR    arg2 = pArgInf[2].m_pText;
    LPSTR    arg3 = pArgInf[3].m_pText;
    LPSTR    arg4 = pArgInf[4].m_pText;
    BOOL     arg5 = pArgInf[5].m_bool;
    BOOL     arg6 = pArgInf[6].m_bool;

}

