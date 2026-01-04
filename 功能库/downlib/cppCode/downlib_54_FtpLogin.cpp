#include "..\include_downlib_header.h"

// 调用格式: SDT_BOOL (FTP辅助对象).连接FTP服务器, 命令说明: "连接到互联网上指定地址处的FTP服务器。成功返回真，失败返回假"
// 参数<1>: FTP服务器地址 SDT_TEXT, 参数说明: "本参数指定FTP服务器在互联网上的地址，格式为 ftp://域名或ip/"
// 参数<2>: 用户名 SDT_TEXT, 参数说明: "本参数提供用作通过FTP服务器用户验证的用户名称，如果被省略，默认值为匿名用户名“anonymous”"
// 参数<3>: 密码 SDT_TEXT, 参数说明: "本参数提供用作通过FTP服务器用户验证的密码，如果被省略，默认值为空文本"
// 参数<4>: 端口 SDT_INT, 参数说明: "本参数提供欲连接到FTP服务器上的端口号，如果被省略，默认值为标准端口 21"
// 参数<5>: [日志回调函数 SDT_SUB_PTR], 参数说明: "注册一个回调函数，系统产生日志时会调用此函数通知用户,如果为0则不能接收日志，默认为0。回调函数需要有5个参数，第一个参数（整数型），线程信息，第二个参数（文本型），消息产生的时间。第三个参数（文本型），消息的正文。第四个参数（整数型），消息类型，参见”传送常量“第五个参数（整数型），对象信息，可以通过“取对象信息”获得下载或上传对象的信息与本参数比较，从而确定本消息属于哪个对象"
// 参数<6>: [超时 SDT_INT], 参数说明: "登录过程中数据交互的超时时间,单位为毫秒，如果忽略则默认为2000"
DOWNLIB_EXTERN_C void downlib_FtpLogin_54_downlib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR    arg1 = pArgInf[1].m_pText;
    LPSTR    arg2 = pArgInf[2].m_pText;
    LPSTR    arg3 = pArgInf[3].m_pText;
    INT      arg4 = pArgInf[4].m_int;
    DWORD    arg5 = pArgInf[5].m_dwSubCodeAdr;
    INT      arg6 = pArgInf[6].m_int;

}

