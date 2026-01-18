#ifndef __E_STATIC_LIB
#include "include_isapi_header.h"

// 互联网扩展  下的方法索引
static INT s_dtCmdIndexisapi_InternetExtension_static_var_00[] = 
{
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 
    10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 
    20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 
    30, 
};
static int s_dtCmdIndexisapi_InternetExtension_static_var_count_00 = sizeof(s_dtCmdIndexisapi_InternetExtension_static_var_00) / sizeof(s_dtCmdIndexisapi_InternetExtension_static_var_00[0]);

// 互联网筛选器  下的方法索引
static INT s_dtCmdIndexisapi_InternetFilter_static_var_06[] = 
{
    59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 
    69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 
    79, 80, 81, 82, 
};
static int s_dtCmdIndexisapi_InternetFilter_static_var_count_06 = sizeof(s_dtCmdIndexisapi_InternetFilter_static_var_06) / sizeof(s_dtCmdIndexisapi_InternetFilter_static_var_06[0]);

// 互联网扩展  下的枚举常量成员数组
static LIB_DATA_TYPE_ELEMENT s_objEventisapi_InternetExtension_static_var_00[] = 
{
    /*000*/ {SDT_INT, 0, "互联网扩展句柄", NULL, NULL, LES_HIDED, (INT)0x00000000},
    /*001*/ {SDT_TEXT, 0, "前标志", NULL, NULL, LES_HIDED, (INT)0x00000000},
    /*002*/ {SDT_TEXT, 0, "后标志", NULL, NULL, LES_HIDED, (INT)0x00000000},

};
static int s_objEventisapi_InternetExtension_static_var_count_00 = sizeof(s_objEventisapi_InternetExtension_static_var_00) / sizeof(s_objEventisapi_InternetExtension_static_var_00[0]);

// 互联网扩展返回值  下的枚举常量成员数组
static LIB_DATA_TYPE_ELEMENT s_objEventisapi_InternetExtensionResult_static_var_01[] = 
{
    /*000*/ {SDT_INT, 0, "成功", NULL, NULL, LES_HAS_DEFAULT_VALUE, (INT)0x00000001},
    /*001*/ {SDT_INT, 0, "失败", NULL, NULL, LES_HAS_DEFAULT_VALUE, (INT)0x00000004},
    /*002*/ {SDT_INT, 0, "成功并保持连接", NULL, NULL, LES_HAS_DEFAULT_VALUE, (INT)0x00000002},
    /*003*/ {SDT_INT, 0, "PENDING", "HSE_STATUS_PENDING", NULL, LES_HAS_DEFAULT_VALUE, (INT)0x00000003},

};
static int s_objEventisapi_InternetExtensionResult_static_var_count_01 = sizeof(s_objEventisapi_InternetExtensionResult_static_var_01) / sizeof(s_objEventisapi_InternetExtensionResult_static_var_01[0]);

// 服务器变量类型  下的枚举常量成员数组
static LIB_DATA_TYPE_ELEMENT s_objEventisapi_ServerVariable_static_var_02[] = 
{
    /*000*/ {SDT_INT, 0, "超文本传输协议头", "ALL_HTTP", "包含有在表单(HTTP_NAME：value)下的非标准头标的完整列表 ", LES_HAS_DEFAULT_VALUE, (INT)0x00000000},
    /*001*/ {SDT_INT, 0, "非超文本传输协议头", "ALL_RAW", "除了当它被接收的时候没有HTTP前缀和其他的格式，其他的和“所有超文本传输协议头”一样 。", LES_HAS_DEFAULT_VALUE, (INT)0x00000001},
    /*002*/ {SDT_INT, 0, "数据库逻辑路径", "APPL_MD_PATH", "包含有数据库的逻辑路径。", LES_HAS_DEFAULT_VALUE, (INT)0x00000002},
    /*003*/ {SDT_INT, 0, "数据库物理路径", "APPL_PHYSICAL_PATH", "包含有数据库的物理路径。", LES_HAS_DEFAULT_VALUE, (INT)0x00000003},
    /*004*/ {SDT_INT, 0, "身份验证口令", "AUTH_PASSWORD", "如果使用最基本的身份验证时所输入的口令。", LES_HAS_DEFAULT_VALUE, (INT)0x00000004},
    /*005*/ {SDT_INT, 0, "身份验证类型", "AUTH_TYPE", "使用的安全身份验证类型。", LES_HAS_DEFAULT_VALUE, (INT)0x00000005},
    /*006*/ {SDT_INT, 0, "身份验证用户名", "AUTH_USER", "已经过身份验证的用户名。", LES_HAS_DEFAULT_VALUE, (INT)0x00000006},
    /*007*/ {SDT_INT, 0, "客户认证存根", "CERT_COOKIE", "包含客户认证的唯一标识符", LES_HAS_DEFAULT_VALUE, (INT)0x00000007},
    /*008*/ {SDT_INT, 0, "客户认证标志", "CERT_FLAGS", "如果客户认证存在，则设置第一个标志位，如果证书可信，则设置第二个标志位 ", LES_HAS_DEFAULT_VALUE, (INT)0x00000008},
    /*009*/ {SDT_INT, 0, "客户认证发放机构", "CERT_ISSUER", "客户认证发放机构", LES_HAS_DEFAULT_VALUE, (INT)0x00000009},
    /*010*/ {SDT_INT, 0, "安全码位数", "CERT_KEYSIZE", "在安全套接字层(SSL)安全码中的位数", LES_HAS_DEFAULT_VALUE, (INT)0x0000000A},
    /*011*/ {SDT_INT, 0, "服务器安全码位数", "CERT_SECRETKEYSIZE", "在服务器中的安全套接字层(SSL)安全码中的位数", LES_HAS_DEFAULT_VALUE, (INT)0x0000000B},
    /*012*/ {SDT_INT, 0, "客户认证序列号", "CERT_SERIALNUMBER", "客户认证的序列号", LES_HAS_DEFAULT_VALUE, (INT)0x0000000C},
    /*013*/ {SDT_INT, 0, "服务器证书", "CERT_SERVER_ISSUER", "发放服务器认证的证书", LES_HAS_DEFAULT_VALUE, (INT)0x0000000D},
    /*014*/ {SDT_INT, 0, "服务器认证主题", "CERT_SERVER_SUBJECT", "服务器认证的主题域", LES_HAS_DEFAULT_VALUE, (INT)0x0000000E},
    /*015*/ {SDT_INT, 0, "客户认证主题", "CERT_SUBJECT", "客户认证的主题域", LES_HAS_DEFAULT_VALUE, (INT)0x0000000F},
    /*016*/ {SDT_INT, 0, "内容长度", "CONTENT_LENGTH", "访问者向服务器发出请求时报告的字节数", LES_HAS_DEFAULT_VALUE, (INT)0x00000010},
    /*017*/ {SDT_INT, 0, "内容类型", "CONTENT_TYPE", "请求的类型", LES_HAS_DEFAULT_VALUE, (INT)0x00000011},
    /*018*/ {SDT_INT, 0, "帐号", "LOGON_USER", "用户登陆到windows的帐号", LES_HAS_DEFAULT_VALUE, (INT)0x0000001D},
    /*019*/ {SDT_INT, 0, "是否安全请求", "HTTPS", "如果是一个安全请求，则包含字符串真(on)，否则的话包含字符串假(off) ", LES_HAS_DEFAULT_VALUE, (INT)0x00000015},
    /*020*/ {SDT_INT, 0, "请求位数", "HTTPS_KEYSIZE", "在请求中使用的安全套接字层(SSL)的位数", LES_HAS_DEFAULT_VALUE, (INT)0x00000016},
    /*021*/ {SDT_INT, 0, "请求服务端位数", "HTTPS_SECRETKEYSIZE", "在服务器端使用的安全套接字层(SSL)位数", LES_HAS_DEFAULT_VALUE, (INT)0x00000017},
    /*022*/ {SDT_INT, 0, "请求服务器证书", "HTTPS_SERVER_ISSUER", "请求中使用的服务器认证的证书", LES_HAS_DEFAULT_VALUE, (INT)0x00000018},
    /*023*/ {SDT_INT, 0, "请求服务器主题", "HTTPS_SERVER_SUBJECT", "服务器认证的主题域", LES_HAS_DEFAULT_VALUE, (INT)0x00000019},
    /*024*/ {SDT_INT, 0, "标识号", "INSTANCE_ID", "互联网服务器的实例标识号", LES_HAS_DEFAULT_VALUE, (INT)0x0000001A},
    /*025*/ {SDT_INT, 0, "服务元路径", "INSTANCE_META_PATH", "互联网服务器实例的元数据库路径", LES_HAS_DEFAULT_VALUE, (INT)0x0000001B},
    /*026*/ {SDT_INT, 0, "路径信息", "PATH_INFO", "在域名根目录外的到被请求页面的路径", LES_HAS_DEFAULT_VALUE, (INT)0x0000001E},
    /*027*/ {SDT_INT, 0, "请求物理路径", "PATH_TRANSLATED", "请求的物理路径", LES_HAS_DEFAULT_VALUE, (INT)0x0000001F},
    /*028*/ {SDT_INT, 0, "字符串", "QUERY_STRING", "通过使用获取(GET)方法提交的任何数据，或是通过一个链接中的问号后面的数据 ", LES_HAS_DEFAULT_VALUE, (INT)0x00000020},
    /*029*/ {SDT_INT, 0, "客户端IP", "REMOTE_ADDR", "发出请求的机器的IP地址", LES_HAS_DEFAULT_VALUE, (INT)0x00000021},
    /*030*/ {SDT_INT, 0, "客户端域", "REMOTE_HOST", "如果存在，为请求者的转换名，否则的话，为包含这个IP地址的域", LES_HAS_DEFAULT_VALUE, (INT)0x00000022},
    /*031*/ {SDT_INT, 0, "客户端用户名", "REMOTE_USER", "客户端发送的用户名 ", LES_HAS_DEFAULT_VALUE, (INT)0x00000024},
    /*032*/ {SDT_INT, 0, "请求方法", "REQUEST_METHOD", "使用的请求方法，即投递(POST)还是获取(GET)", LES_HAS_DEFAULT_VALUE, (INT)0x00000025},
    /*033*/ {SDT_INT, 0, "脚本名", "SCRIPT_NAME", "被请求的脚本的名字", LES_HAS_DEFAULT_VALUE, (INT)0x00000026},
    /*034*/ {SDT_INT, 0, "服务器名", "SERVER_NAME", "服务器主机名", LES_HAS_DEFAULT_VALUE, (INT)0x00000027},
    /*035*/ {SDT_INT, 0, "服务器端口", "SERVER_PORT", "服务器使用的端口号 ", LES_HAS_DEFAULT_VALUE, (INT)0x00000028},
    /*036*/ {SDT_INT, 0, "安全连接", "SERVER_PORT_SECURE", "如果请求是通过安全连接发送的，那么这个域中包含值1，否则的话包含0 ", LES_HAS_DEFAULT_VALUE, (INT)0x00000029},
    /*037*/ {SDT_INT, 0, "协议版本", "SERVER_PROTOCOL", "使用的协议的版本号，即HTTP/11 ", LES_HAS_DEFAULT_VALUE, (INT)0x0000002A},
    /*038*/ {SDT_INT, 0, "服务器软件信息", "SERVER_SOFTWARE", "在服务器上运行的Web服务器软件的名称和版本 ", LES_HAS_DEFAULT_VALUE, (INT)0x0000002B},
    /*039*/ {SDT_INT, 0, "地址", "URL", "被请求的页面的地址(URL)", LES_HAS_DEFAULT_VALUE, (INT)0x0000002C},

};
static int s_objEventisapi_ServerVariable_static_var_count_02 = sizeof(s_objEventisapi_ServerVariable_static_var_02) / sizeof(s_objEventisapi_ServerVariable_static_var_02[0]);

// 互联网扩展函数  下的枚举常量成员数组
static LIB_DATA_TYPE_ELEMENT s_objEventisapi_ServerSupportFunction_static_var_03[] = 
{
    /*000*/ {SDT_INT, 0, "HSE_REQ_SEND_URL_REDIRECT_RESP", NULL, "HSE_REQ_SEND_URL_REDIRECT_RESP", LES_HAS_DEFAULT_VALUE, (INT)0x00000001},
    /*001*/ {SDT_INT, 0, "HSE_REQ_SEND_URL", NULL, "HSE_REQ_SEND_URL", LES_HAS_DEFAULT_VALUE, (INT)0x00000002},
    /*002*/ {SDT_INT, 0, "HSE_REQ_SEND_RESPONSE_HEADER", NULL, "HSE_REQ_SEND_RESPONSE_HEADER", LES_HAS_DEFAULT_VALUE, (INT)0x00000003},
    /*003*/ {SDT_INT, 0, "HSE_REQ_DONE_WITH_SESSION", NULL, "HSE_REQ_DONE_WITH_SESSION", LES_HAS_DEFAULT_VALUE, (INT)0x00000004},
    /*004*/ {SDT_INT, 0, "HSE_REQ_MAP_URL_TO_PATH", NULL, "HSE_REQ_MAP_URL_TO_PATH", LES_HAS_DEFAULT_VALUE, (INT)0x000003E9},
    /*005*/ {SDT_INT, 0, "HSE_REQ_GET_SSPI_INFO", NULL, "HSE_REQ_GET_SSPI_INFO", LES_HAS_DEFAULT_VALUE, (INT)0x000003EA},
    /*006*/ {SDT_INT, 0, "HSE_APPEND_LOG_PARAMETER", NULL, "HSE_APPEND_LOG_PARAMETER", LES_HAS_DEFAULT_VALUE, (INT)0x000003EB},
    /*007*/ {SDT_INT, 0, "HSE_REQ_IO_COMPLETION", NULL, "HSE_REQ_IO_COMPLETION", LES_HAS_DEFAULT_VALUE, (INT)0x000003ED},
    /*008*/ {SDT_INT, 0, "HSE_REQ_TRANSMIT_FILE", NULL, "HSE_REQ_TRANSMIT_FILE", LES_HAS_DEFAULT_VALUE, (INT)0x000003EE},
    /*009*/ {SDT_INT, 0, "HSE_REQ_REFRESH_ISAPI_ACL", NULL, "HSE_REQ_REFRESH_ISAPI_ACL", LES_HAS_DEFAULT_VALUE, (INT)0x000003EF},
    /*010*/ {SDT_INT, 0, "HSE_REQ_IS_KEEP_CONN", NULL, "HSE_REQ_IS_KEEP_CONN", LES_HAS_DEFAULT_VALUE, (INT)0x000003F0},
    /*011*/ {SDT_INT, 0, "HSE_REQ_ASYNC_READ_CLIENT", NULL, "HSE_REQ_ASYNC_READ_CLIENT", LES_HAS_DEFAULT_VALUE, (INT)0x000003F2},
    /*012*/ {SDT_INT, 0, "HSE_REQ_GET_IMPERSONATION_TOKEN", NULL, "HSE_REQ_GET_IMPERSONATION_TOKEN", LES_HAS_DEFAULT_VALUE, (INT)0x000003F3},
    /*013*/ {SDT_INT, 0, "HSE_REQ_MAP_URL_TO_PATH_EX", NULL, "HSE_REQ_MAP_URL_TO_PATH_EX", LES_HAS_DEFAULT_VALUE, (INT)0x000003F4},
    /*014*/ {SDT_INT, 0, "HSE_REQ_ABORTIVE_CLOSE", NULL, "HSE_REQ_ABORTIVE_CLOSE", LES_HAS_DEFAULT_VALUE, (INT)0x000003F6},
    /*015*/ {SDT_INT, 0, "HSE_REQ_GET_CERT_INFO_EX", NULL, "HSE_REQ_GET_CERT_INFO_EX", LES_HAS_DEFAULT_VALUE, (INT)0x000003F7},
    /*016*/ {SDT_INT, 0, "HSE_REQ_SEND_RESPONSE_HEADER_EX", NULL, "HSE_REQ_SEND_RESPONSE_HEADER_EX", LES_HAS_DEFAULT_VALUE, (INT)0x000003F8},
    /*017*/ {SDT_INT, 0, "HSE_REQ_CLOSE_CONNECTION", NULL, "HSE_REQ_CLOSE_CONNECTION", LES_HAS_DEFAULT_VALUE, (INT)0x000003F9},
    /*018*/ {SDT_INT, 0, "HSE_REQ_IS_CONNECTED", NULL, "HSE_REQ_IS_CONNECTED", LES_HAS_DEFAULT_VALUE, (INT)0x000003FA},
    /*019*/ {SDT_INT, 0, "HSE_REQ_MAP_UNICODE_URL_TO_PATH", NULL, "HSE_REQ_MAP_UNICODE_URL_TO_PATH", LES_HAS_DEFAULT_VALUE, (INT)0x000003FF},
    /*020*/ {SDT_INT, 0, "HSE_REQ_MAP_UNICODE_URL_TO_PATH_EX", NULL, "HSE_REQ_MAP_UNICODE_URL_TO_PATH_EX", LES_HAS_DEFAULT_VALUE, (INT)0x00000400},
    /*021*/ {SDT_INT, 0, "HSE_REQ_EXEC_UNICODE_URL", NULL, "HSE_REQ_EXEC_UNICODE_URL", LES_HAS_DEFAULT_VALUE, (INT)0x00000401},
    /*022*/ {SDT_INT, 0, "HSE_REQ_EXEC_URL", NULL, "HSE_REQ_EXEC_URL", LES_HAS_DEFAULT_VALUE, (INT)0x00000402},
    /*023*/ {SDT_INT, 0, "HSE_REQ_GET_EXEC_URL_STATUS", NULL, "HSE_REQ_GET_EXEC_URL_STATUS", LES_HAS_DEFAULT_VALUE, (INT)0x00000403},
    /*024*/ {SDT_INT, 0, "HSE_REQ_SEND_CUSTOM_ERROR", NULL, "HSE_REQ_SEND_CUSTOM_ERROR", LES_HAS_DEFAULT_VALUE, (INT)0x00000404},
    /*025*/ {SDT_INT, 0, "HSE_REQ_IS_IN_PROCESS", NULL, "HSE_REQ_IS_IN_PROCESS", LES_HAS_DEFAULT_VALUE, (INT)0x00000406},
    /*026*/ {SDT_INT, 0, "HSE_REQ_REPORT_UNHEALTHY", NULL, "HSE_REQ_REPORT_UNHEALTHY", LES_HAS_DEFAULT_VALUE, (INT)0x00000408},
    /*027*/ {SDT_INT, 0, "HSE_REQ_NORMALIZE_URL", NULL, "HSE_REQ_NORMALIZE_URL", LES_HAS_DEFAULT_VALUE, (INT)0x00000409},
    /*028*/ {SDT_INT, 0, "HSE_REQ_VECTOR_SEND", NULL, "HSE_REQ_VECTOR_SEND", LES_HAS_DEFAULT_VALUE, (INT)0x0000040D},
    /*029*/ {SDT_INT, 0, "HSE_REQ_GET_ANONYMOUS_TOKEN", NULL, "HSE_REQ_GET_ANONYMOUS_TOKEN", LES_HAS_DEFAULT_VALUE, (INT)0x0000040E},
    /*030*/ {SDT_INT, 0, "HSE_REQ_GET_CACHE_INVALIDATION_CALLBACK", NULL, "HSE_REQ_GET_CACHE_INVALIDATION_CALLBACK", LES_HAS_DEFAULT_VALUE, (INT)0x00000410},
    /*031*/ {SDT_INT, 0, "HSE_REQ_GET_UNICODE_ANONYMOUS_TOKEN", NULL, "HSE_REQ_GET_UNICODE_ANONYMOUS_TOKEN", LES_HAS_DEFAULT_VALUE, (INT)0x00000411},
    /*032*/ {SDT_INT, 0, "HSE_REQ_GET_TRACE_INFO", NULL, "HSE_REQ_GET_TRACE_INFO", LES_HAS_DEFAULT_VALUE, (INT)0x00000412},

};
static int s_objEventisapi_ServerSupportFunction_static_var_count_03 = sizeof(s_objEventisapi_ServerSupportFunction_static_var_03) / sizeof(s_objEventisapi_ServerSupportFunction_static_var_03[0]);

// 互联网筛选器  下的枚举常量成员数组
static LIB_DATA_TYPE_ELEMENT s_objEventisapi_InternetFilter_static_var_06[] = 
{
    /*000*/ {SDT_INT, 0, "pFilterDataStruct", NULL, NULL, LES_HIDED, (INT)0x00000000},

};
static int s_objEventisapi_InternetFilter_static_var_count_06 = sizeof(s_objEventisapi_InternetFilter_static_var_06) / sizeof(s_objEventisapi_InternetFilter_static_var_06[0]);

// 筛选器事件  下的枚举常量成员数组
static LIB_DATA_TYPE_ELEMENT s_objEventisapi_FilterNotificationType_static_var_07[] = 
{
    /*000*/ {SDT_INT, 0, "读取数据", "ReadRawData", "此事件发生在从客户端读取数据时。每次请求(Request)可能多次读取数据。IIS5只能在全局筛选器中使用此事件，IIS6以兼容IIS5模式运行时才支持此事件，到IIS7之后就不再支持了。SF_NOTIFY_READ_RAW_DATA", LES_HAS_DEFAULT_VALUE, (INT)0x00008000},
    /*001*/ {SDT_INT, 0, "预处理头", "PreProcessHeaders", "此事件发生在预处理头之后、处理头内容之前。(Occurs immediately after IIS has pre-processed headers, but before IIS has begun to process header content.) SF_NOTIFY_PREPROC_HEADERS", LES_HAS_DEFAULT_VALUE, (INT)0x00004000},
    /*002*/ {SDT_INT, 0, "映射URL", "MapURL", "此事件发生在服务器把URL映射到真实路径之后，可能会发生多次。SF_NOTIFY_URL_MAP", LES_HAS_DEFAULT_VALUE, (INT)0x00001000},
    /*003*/ {SDT_INT, 0, "认证", "Authentication", "此事件发生在服务器认证(Authenticate)客户之前。SF_NOTIFY_AUTHENTICATION", LES_HAS_DEFAULT_VALUE, (INT)0x00002000},
    /*004*/ {SDT_INT, 0, "认证完成", "AuthenticationComplete", "此事件发生在认证完成后。SF_NOTIFY_AUTH_COMPLETE", LES_HAS_DEFAULT_VALUE, (INT)0x04000000},
    /*005*/ {SDT_INT, 0, "拒绝访问", "AccessDenied", "此事件发生在服务器“决定拒绝访问”之后、向客户端发送回复(Response)之前。SF_NOTIFY_ACCESS_DENIED", LES_HAS_DEFAULT_VALUE, (INT)0x00000800},
    /*006*/ {SDT_INT, 0, "发送回复", "SendResponse", "此事件发生在服务器向客户端发送回复(Response)之前，此时已接收并解析完毕客户端的请求(Request)但还没有发送任何数据（包括HTTP/HTTPS头）。SF_NOTIFY_SEND_RESPONSE", LES_HAS_DEFAULT_VALUE, (INT)0x00000040},
    /*007*/ {SDT_INT, 0, "发送数据", "SendRawData", "此事件发生在向客户端发送数据时。每次请求(Request)可能多次发送数据。SF_NOTIFY_SEND_RAW_DATA", LES_HAS_DEFAULT_VALUE, (INT)0x00000400},
    /*008*/ {SDT_INT, 0, "结束请求", "EndRequest", "此事件发生在请求(Request)被处理完毕（回复完毕）之后。SF_NOTIFY_END_OF_REQUEST", LES_HAS_DEFAULT_VALUE, (INT)0x00000080},
    /*009*/ {SDT_INT, 0, "记录日志", "Log", "此事件发生在请求(Request)被处理完毕之后、服务器记录日志之前。SF_NOTIFY_LOG", LES_HAS_DEFAULT_VALUE, (INT)0x00000200},
    /*010*/ {SDT_INT, 0, "结束会话", "EndSession", "此事件发生在会话(Session)结束之后。SF_NOTIFY_END_OF_NET_SESSION", LES_HAS_DEFAULT_VALUE, (INT)0x00000100},

};
static int s_objEventisapi_FilterNotificationType_static_var_count_07 = sizeof(s_objEventisapi_FilterNotificationType_static_var_07) / sizeof(s_objEventisapi_FilterNotificationType_static_var_07[0]);

// 筛选器端口  下的枚举常量成员数组
static LIB_DATA_TYPE_ELEMENT s_objEventisapi_FilterSecurePort_static_var_08[] = 
{
    /*000*/ {SDT_INT, 0, "安全端口", "SecurePortOnly", "表示此筛选器仅对安全端口有效", LES_HAS_DEFAULT_VALUE, (INT)0x00000001},
    /*001*/ {SDT_INT, 0, "非安全端口", "NonsecurePortOnly", "表示此筛选器仅对非安全端口有效", LES_HAS_DEFAULT_VALUE, (INT)0x00000002},

};
static int s_objEventisapi_FilterSecurePort_static_var_count_08 = sizeof(s_objEventisapi_FilterSecurePort_static_var_08) / sizeof(s_objEventisapi_FilterSecurePort_static_var_08[0]);

// 筛选器优先级  下的枚举常量成员数组
static LIB_DATA_TYPE_ELEMENT s_objEventisapi_FilterPriority_static_var_09[] = 
{
    /*000*/ {SDT_INT, 0, "默认", "Default", "系统默认优先级", LES_HAS_DEFAULT_VALUE, (INT)0x00020000},
    /*001*/ {SDT_INT, 0, "较低", "Low", NULL, LES_HAS_DEFAULT_VALUE, (INT)0x00020000},
    /*002*/ {SDT_INT, 0, "普通", "Medium", "优先级不高也不低", LES_HAS_DEFAULT_VALUE, (INT)0x00040000},
    /*003*/ {SDT_INT, 0, "较高", "High", NULL, LES_HAS_DEFAULT_VALUE, (INT)0x00080000},

};
static int s_objEventisapi_FilterPriority_static_var_count_09 = sizeof(s_objEventisapi_FilterPriority_static_var_09) / sizeof(s_objEventisapi_FilterPriority_static_var_09[0]);

// 筛选器返回值  下的枚举常量成员数组
static LIB_DATA_TYPE_ELEMENT s_objEventisapi_FilterResult_static_var_10[] = 
{
    /*000*/ {SDT_INT, 0, "已完成", "Finished", "表示已完成，服务器应断开会话连接(the server should disconnect the session)? SF_STATUS_REQ_FINISHED", LES_HAS_DEFAULT_VALUE, (INT)0x08000000},
    /*001*/ {SDT_INT, 0, "已完成_保持连接", "Finished_KeepConnection", "表示此筛选器不被支持(this filter is not supported)? SF_STATUS_REQ_FINISHED_KEEP_CONN", LES_HAS_DEFAULT_VALUE, (INT)0x08000001},
    /*002*/ {SDT_INT, 0, "已处理_其他筛选器继续", "NextNotification", "表示此事件已处理，后面的筛选器继续处理。HttpFilterProc()多数情况下通常返回此常量值。SF_STATUS_REQ_NEXT_NOTIFICATION", LES_HAS_DEFAULT_VALUE, (INT)0x08000002},
    /*003*/ {SDT_INT, 0, "已处理_其他筛选器走开", "HandledNotification", "表示此事件已处理，不必再让其他筛选器处理。SF_STATUS_REQ_HANDLED_NOTIFICATION", LES_HAS_DEFAULT_VALUE, (INT)0x08000003},
    /*004*/ {SDT_INT, 0, "已错误", "ErrorOccurs", "表示已发生错误。返回此数值前应该调用Win32 API函数SetLastError()设定相应的错误号，以便IIS服务器做针对性处理。SF_STATUS_REQ_ERROR", LES_HAS_DEFAULT_VALUE, (INT)0x08000004},
    /*005*/ {SDT_INT, 0, "继续读取数据", NULL, "表示要求继续读取数据。The filter is an opaque stream filter (encrypted/compressed HTTP requests) and the session parameters are being negotiated. This is valid only for raw-read notification. This notification indicates that the full request has not yet been received; the Web server should issue another read and notify the filter with the additional data read. SF_STATUS_REQ_READ_NEXT", LES_HAS_DEFAULT_VALUE, (INT)0x08000005},

};
static int s_objEventisapi_FilterResult_static_var_count_10 = sizeof(s_objEventisapi_FilterResult_static_var_10) / sizeof(s_objEventisapi_FilterResult_static_var_10[0]);

// 筛选器扩展函数  下的枚举常量成员数组
static LIB_DATA_TYPE_ELEMENT s_objEventisapi_FilterServerSupportFunction_static_var_11[] = 
{
    /*000*/ {SDT_INT, 0, "SF_REQ_SEND_RESPONSE_HEADER", NULL, "SF_REQ_SEND_RESPONSE_HEADER", LES_HAS_DEFAULT_VALUE, (INT)0x00000000},
    /*001*/ {SDT_INT, 0, "SF_REQ_ADD_HEADERS_ON_DENIAL", NULL, "SF_REQ_ADD_HEADERS_ON_DENIAL", LES_HAS_DEFAULT_VALUE, (INT)0x00000001},
    /*002*/ {SDT_INT, 0, "SF_REQ_SET_NEXT_READ_SIZE", NULL, "SF_REQ_SET_NEXT_READ_SIZE", LES_HAS_DEFAULT_VALUE, (INT)0x00000002},
    /*003*/ {SDT_INT, 0, "SF_REQ_SET_PROXY_INFO", NULL, "SF_REQ_SET_PROXY_INFO", LES_HAS_DEFAULT_VALUE, (INT)0x00000003},
    /*004*/ {SDT_INT, 0, "SF_REQ_GET_CONNID", NULL, "SF_REQ_GET_CONNID", LES_HAS_DEFAULT_VALUE, (INT)0x00000004},
    /*005*/ {SDT_INT, 0, "SF_REQ_SET_CERTIFICATE_INFO", NULL, "SF_REQ_SET_CERTIFICATE_INFO", LES_HAS_DEFAULT_VALUE, (INT)0x00000005},
    /*006*/ {SDT_INT, 0, "SF_REQ_GET_PROPERTY", NULL, "SF_REQ_GET_PROPERTY", LES_HAS_DEFAULT_VALUE, (INT)0x00000006},
    /*007*/ {SDT_INT, 0, "SF_REQ_NORMALIZE_URL", NULL, "SF_REQ_NORMALIZE_URL", LES_HAS_DEFAULT_VALUE, (INT)0x00000007},
    /*008*/ {SDT_INT, 0, "SF_REQ_DISABLE_NOTIFICATIONS", NULL, "SF_REQ_DISABLE_NOTIFICATIONS", LES_HAS_DEFAULT_VALUE, (INT)0x00000008},

};
static int s_objEventisapi_FilterServerSupportFunction_static_var_count_11 = sizeof(s_objEventisapi_FilterServerSupportFunction_static_var_11) / sizeof(s_objEventisapi_FilterServerSupportFunction_static_var_11[0]);

// 筛选器拒绝理由  下的枚举常量成员数组
static LIB_DATA_TYPE_ELEMENT s_objEventisapi_FilterDeniedReason_static_var_12[] = 
{
    /*000*/ {SDT_INT, 0, "登录", "DeniedLogon", "The client could not be logged on. SF_DENIED_LOGON", LES_HAS_DEFAULT_VALUE, (INT)0x00000001},
    /*001*/ {SDT_INT, 0, "访问控制列表", "DeniedResource", "A Windows DACL denied the resource. SF_DENIED_RESOURCE", LES_HAS_DEFAULT_VALUE, (INT)0x00000002},
    /*002*/ {SDT_INT, 0, "筛选器", "DeniedFilter", "An ISAPI filter denied the request. SF_DENIED_FILTER", LES_HAS_DEFAULT_VALUE, (INT)0x00000004},
    /*003*/ {SDT_INT, 0, "扩展程序或CGI", "DeniedApplication", "An ISAPI extension or CGI application denied the request. SF_DENIED_APPLICATION", LES_HAS_DEFAULT_VALUE, (INT)0x00000008},
    /*004*/ {SDT_INT, 0, "服务器配置", "DeniedConfig", "The server configuration denied the request. For example, disabling anonymous requests on the server would generate this filter notification when a user without credentials tried to make a request to the server. SF_DENIED_BY_CONFIG", LES_HAS_DEFAULT_VALUE, (INT)0x00010000},

};
static int s_objEventisapi_FilterDeniedReason_static_var_count_12 = sizeof(s_objEventisapi_FilterDeniedReason_static_var_12) / sizeof(s_objEventisapi_FilterDeniedReason_static_var_12[0]);

LIB_DATA_TYPE_INFO g_DataType_isapi_global_var[] = 
{
    //1=中文名字,2=英文名字,3=详细解释,4=命令数量,5=索引值,6=标志 LDT_
    //类型为窗口或菜单组件时有效 7=资源ID,8=事件数量,9=组件事件数组,10=属性数  11=属性数组 12=组件交互子程序
    //不为窗口、菜单组件或为枚举数据类型时才有效 13=成员数量,14=成员数据数组
    
/*000*/ { "互联网扩展", "InternetExtension", "请在 GetExtensionVersion 中调用“互联网扩展.初始化版本信息()”，在 HttpExtensionProc 中首先调用“互联网扩展.初始化互联网扩展()”",
            s_dtCmdIndexisapi_InternetExtension_static_var_count_00, s_dtCmdIndexisapi_InternetExtension_static_var_00, _DT_OS(__OS_WIN),
            0, NULL, NULL,
            NULL, NULL,
            NULL,
            s_objEventisapi_InternetExtension_static_var_count_00, s_objEventisapi_InternetExtension_static_var_00
        },
/*001*/ { "互联网扩展返回值", "InternetExtensionResult", "本数据类型包括了互联网扩展返回值的常量",
            NULL, NULL, _DT_OS(__OS_WIN) | LDT_ENUM,
            0, NULL, NULL,
            NULL, NULL,
            NULL,
            s_objEventisapi_InternetExtensionResult_static_var_count_01, s_objEventisapi_InternetExtensionResult_static_var_01
        },
/*002*/ { "服务器变量类型", "ServerVariable", "本数据类型包括了服务器变量类型的常量",
            NULL, NULL, _DT_OS(__OS_WIN) | LDT_ENUM,
            0, NULL, NULL,
            NULL, NULL,
            NULL,
            s_objEventisapi_ServerVariable_static_var_count_02, s_objEventisapi_ServerVariable_static_var_02
        },
/*003*/ { "互联网扩展函数", "ServerSupportFunction", "参见：互联网扩展.调用扩展函数()。各函数的功能及其参数含义，请参考ISAPI文档 http://msdn.microsoft.com/en-us/library/ms525975(v=vs.90).aspx。注意，有些扩展函数是IIS后来添加的，可能不被较旧版本支持。",
            NULL, NULL, _DT_OS(__OS_WIN) | LDT_ENUM,
            0, NULL, NULL,
            NULL, NULL,
            NULL,
            s_objEventisapi_ServerSupportFunction_static_var_count_03, s_objEventisapi_ServerSupportFunction_static_var_03
        },
/*004*/ { NULL, NULL, NULL,
            NULL, NULL, _DT_OS(__OS_WIN) | LDT_IS_HIDED,
            0, NULL, NULL,
            NULL, NULL,
            NULL,
            NULL, NULL
        },
/*005*/ { NULL, NULL, NULL,
            NULL, NULL, _DT_OS(__OS_WIN) | LDT_IS_HIDED,
            0, NULL, NULL,
            NULL, NULL,
            NULL,
            NULL, NULL
        },
/*006*/ { "互联网筛选器", "InternetFilter", "请在 GetFilterVersion 中调用“互联网筛选器.初始化版本信息()”，在 HttpFilterProc 中首先调用“互联网筛选器.初始化互联网筛选器()”",
            s_dtCmdIndexisapi_InternetFilter_static_var_count_06, s_dtCmdIndexisapi_InternetFilter_static_var_06, _DT_OS(__OS_WIN),
            0, NULL, NULL,
            NULL, NULL,
            NULL,
            s_objEventisapi_InternetFilter_static_var_count_06, s_objEventisapi_InternetFilter_static_var_06
        },
/*007*/ { "筛选器事件", "FilterNotificationType", "用于指定本筛选器欲处理的事件类型，或正在处理的事件类型。参见：互联网筛选器.初始化版本信息(), 互联网筛选器.取事件()",
            NULL, NULL, _DT_OS(__OS_WIN) | LDT_ENUM,
            0, NULL, NULL,
            NULL, NULL,
            NULL,
            s_objEventisapi_FilterNotificationType_static_var_count_07, s_objEventisapi_FilterNotificationType_static_var_07
        },
/*008*/ { "筛选器端口", "FilterSecurePort", "用于指定本筛选器欲处理的端口类型，参见：互联网筛选器.初始化版本信息()",
            NULL, NULL, _DT_OS(__OS_WIN) | LDT_ENUM,
            0, NULL, NULL,
            NULL, NULL,
            NULL,
            s_objEventisapi_FilterSecurePort_static_var_count_08, s_objEventisapi_FilterSecurePort_static_var_08
        },
/*009*/ { "筛选器优先级", "FilterPriority", "用于指定本筛选器的优先级，参见：互联网筛选器.初始化版本信息()",
            NULL, NULL, _DT_OS(__OS_WIN) | LDT_ENUM,
            0, NULL, NULL,
            NULL, NULL,
            NULL,
            s_objEventisapi_FilterPriority_static_var_count_09, s_objEventisapi_FilterPriority_static_var_09
        },
/*010*/ { "筛选器返回值", "FilterResult", "指示本筛选器对当前事件的执行结果，用作 HttpFilterProc() 的返回值",
            NULL, NULL, _DT_OS(__OS_WIN) | LDT_ENUM,
            0, NULL, NULL,
            NULL, NULL,
            NULL,
            s_objEventisapi_FilterResult_static_var_count_10, s_objEventisapi_FilterResult_static_var_10
        },
/*011*/ { "筛选器扩展函数", "FilterServerSupportFunction", "服务器提供的扩展函数(ServerSupportFunction)，不同的函数对参数有不同的要求，详见MSDN等文档。参见：互联网筛选器.调用扩展函数()。注意，有些扩展函数是IIS后来添加的，可能不被较旧版本支持。",
            NULL, NULL, _DT_OS(__OS_WIN) | LDT_ENUM,
            0, NULL, NULL,
            NULL, NULL,
            NULL,
            s_objEventisapi_FilterServerSupportFunction_static_var_count_11, s_objEventisapi_FilterServerSupportFunction_static_var_11
        },
/*012*/ { "筛选器拒绝理由", "FilterDeniedReason", "参见：互联网筛选器.取拒绝访问信息() 第三个参数",
            NULL, NULL, _DT_OS(__OS_WIN) | LDT_ENUM,
            0, NULL, NULL,
            NULL, NULL,
            NULL,
            s_objEventisapi_FilterDeniedReason_static_var_count_12, s_objEventisapi_FilterDeniedReason_static_var_12
        },
};
int g_DataType_isapi_global_var_count = sizeof(g_DataType_isapi_global_var) / sizeof(g_DataType_isapi_global_var[0]);

#endif

