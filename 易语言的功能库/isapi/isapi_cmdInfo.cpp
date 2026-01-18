#if !defined(__E_STATIC_LIB)
#include "include_isapi_header.h"

//TODO 静态库需要的部分,会记录所有的函数名到数组里,静态编译的时候需要取回命令名
static ARG_INFO g_argumentInfo_isapi_global_var[] = 
{
    // 1=参数名称, 2=参数详细解释, 3=指定图像索引,从1开始,0表示无, 4=图像数目(用作动画)
    // 5=参数类型, 6=参数默认值,   7=参数标志 AS_ 开头常量
    // AS_HAS_DEFAULT_VALUE         有默认值,倒数第二个参数是默认值
    // AS_DEFAULT_VALUE_IS_EMPTY    默认值为空,有可空标志
    // AS_RECEIVE_VAR               只能传递变量,相当于传引用,传递过来的肯定不是数组
    // AS_RECEIVE_VAR_ARRAY         传递过来的肯定是数组变量引用
    // AS_RECEIVE_VAR_OR_ARRAY      传递变量或者数组变量引用
    // AS_RECEIVE_ARRAY_DATA        传递数组
    // AS_RECEIVE_ALL_TYPE_DATA     传递数组/非数组
    // AS_RECEIVE_VAR_OR_OTHER      可以传递 变量/数据/返回值数据



    /*000*/ {"版本信息", "要求必须是 GetExtensionVersion 函数的参数", 0, 0, SDT_INT, 0, NULL},
    /*001*/ {"描述信息", "用于描述此扩展程序的文本，可能会被显示在IIS管理器中", 0, 0, SDT_TEXT, 0, NULL},

    /*002*/ {"扩展程序上下文", "要求必须是 HttpExtensionProc 函数的参数", 0, 0, SDT_INT, 0, NULL},

    /*003*/ {"文本", "本参数表示要写的文本。", 0, 0, SDT_TEXT, 0, NULL},

    /*004*/ {"变量类型", "要取的变量类型，可以为“服务器变量类型”中的常量值。", 0, 0, SDT_INT, 0, NULL},

    /*005*/ {"路径类型", "路径类型: 0为相对路径，1为绝对路径", 0, 0, SDT_INT, 0, NULL},

    /*006*/ {"文本", "本参数表示要解码的文本", 0, 0, SDT_TEXT, 0, NULL},
    /*007*/ {"连接符", "连接各文本的字符，本字符将被替换为空格，默认为 +", 0, 0, SDT_BYTE, 43, AS_HAS_DEFAULT_VALUE},

    /*008*/ {"文本", "本参数表示要查找的文本", 0, 0, SDT_TEXT, 0, NULL},
    /*009*/ {"关键字", "关键字名字", 0, 0, SDT_TEXT, 0, NULL},
    /*010*/ {"分割字符", "分割的字符。黙认为'&'。 ", 0, 0, SDT_BYTE, 38, AS_HAS_DEFAULT_VALUE},

    /*011*/ {"前标志", "本参数表示变量前面的标志。", 0, 0, SDT_TEXT, 9480312, AS_HAS_DEFAULT_VALUE},
    /*012*/ {"后标志", "本参数表示变量后面的标志。", 0, 0, SDT_TEXT, 9480268, AS_HAS_DEFAULT_VALUE},

    /*013*/ {"前标志", "本参数表示变量前面的标志。", 0, 0, SDT_TEXT, 0, AS_RECEIVE_VAR},
    /*014*/ {"后标志", "本参数表示变量后面的标志。", 0, 0, SDT_TEXT, 0, AS_RECEIVE_VAR},

    /*015*/ {"要替换的模板", "本参数表示要替换的模板。", 0, 0, SDT_TEXT, 0, AS_RECEIVE_VAR},
    /*016*/ {"变量名称替换数据", "本参数的每一对参数值顺序指定欲被替换的变量名称及其相应的替换值。", 0, 0, SDT_TEXT, 0, NULL},

    /*017*/ {"名称", "本参数表示存根(Cookie)的名称。", 0, 0, SDT_TEXT, 0, NULL},
    /*018*/ {"路径", "限定浏览器发送存根(Cookie)的路径，如果此参数指定则只有在浏览此路径下的网页时存根(Cookie)才会被发送。注：发送路径时请注意路径的大小写要和(Windows)目录的一致。", 0, 0, SDT_TEXT, 9479860, AS_HAS_DEFAULT_VALUE},
    /*019*/ {"网站", "限定了存根(Cookie)发送的网站。", 0, 0, SDT_TEXT, 0, AS_DEFAULT_VALUE_IS_EMPTY},
    /*020*/ {"安全", "本参数为真表示传递中就实行了加密算法，如果你正在使用安全接口层，那么你就可以使用这个属性。安全接口层在IIS中设置。", 0, 0, SDT_BOOL, 0, AS_HAS_DEFAULT_VALUE},
    /*021*/ {"失效日期", "本参数表示到到日期后存根(Cookie)就失效。", 0, 0, SDT_DATE_TIME, 0, NULL},
    /*022*/ {"变量分割符", "本参数表示如果有多个变量的话它们之间用什么符号分割。黙认为'&'。", 0, 0, SDT_BYTE, 38, AS_HAS_DEFAULT_VALUE},
    /*023*/ {"变量名称和值", "本参数的每一对参数值顺序指定变量名称及其相应的值。", 0, 0, SDT_TEXT, 0, NULL},

    /*024*/ {"名称", "本参数表示存根(Cookie)的名称。是“设置存根”方法设置的名称。", 0, 0, SDT_TEXT, 0, NULL},

    /*025*/ {"超时", "本参数表示此会话过多长时间失效。单位为秒。", 0, 0, SDT_INT, 0, NULL},

    /*026*/ {"变量名", "本参数表示要注册或修改变量名。", 0, 0, SDT_TEXT, 0, NULL},
    /*027*/ {"值", "本参数表示赋予变量的值。", 0, 0, SDT_TEXT, 0, NULL},

    /*028*/ {"变量名", "本参数表示查询的变量名。", 0, 0, SDT_TEXT, 0, NULL},

    /*029*/ {"变量名", "本参数表示要取消的变量名。", 0, 0, SDT_TEXT, 0, NULL},

    /*030*/ {"变量名", "本参数表示要取值的变量名。", 0, 0, SDT_TEXT, 0, NULL},

    /*031*/ {"时间", "本参数表示要轮询的时间，单位为秒。注：如果本参数设的时间过长可能会使“会话超时”不准，如果本参数设的时间过短对性能有影响。默认为2分钟。", 0, 0, SDT_INT, 0, NULL},

    /*032*/ {"文本", "本参数表示要编码的文本。", 0, 0, SDT_TEXT, 0, NULL},
    /*033*/ {"连接符", "本参数表示要将空格替换为哪个字符，默认为'+'。", 0, 0, SDT_BYTE, 43, AS_HAS_DEFAULT_VALUE},

    /*034*/ {"大小", "本参数表示期望返回字节集的最大长度。如果实际数据比本长度大，就按本长度返回；如果实际数据比本长度小，就按实际长度返回。", 0, 0, SDT_INT, 0, NULL},

    /*035*/ {"地址", "本参数表示要重定向的地址，所有HEAD的其它信息如Content-Type，可以写在地址的后面，注意加回车换行。", 0, 0, SDT_TEXT, 0, NULL},

    /*036*/ {"欲调用的函数", "应为枚举型常量 #互联网扩展函数.xxx 成员之一", 0, 0, SDT_INT, 0, NULL},
    /*037*/ {"缓冲区地址", NULL, 0, 0, SDT_INT, 0, NULL},
    /*038*/ {"缓冲区长度", "返回后此参数值可能会被改写", 0, 0, SDT_INT, 0, AS_RECEIVE_VAR},
    /*039*/ {"数据类型", "仅在调用部分扩展函数时用到", 0, 0, SDT_INT, 0, AS_DEFAULT_VALUE_IS_EMPTY | AS_RECEIVE_VAR},

    /*040*/ {"数据", NULL, 0, 0, SDT_BIN, 0, NULL},

    /*041*/ {"HTTP状态", "如果被省略，默认为空文本", 0, 0, SDT_TEXT, 0, AS_DEFAULT_VALUE_IS_EMPTY},
    /*042*/ {"HTTP头", "如果被省略，默认为空文本。通常应以两个 #换行符 结尾。", 0, 0, SDT_TEXT, 0, AS_DEFAULT_VALUE_IS_EMPTY},
    /*043*/ {"是否保持连接", "如果被省略，默认为假", 0, 0, SDT_BOOL, 0, AS_DEFAULT_VALUE_IS_EMPTY},

    /*044*/ {"版本信息", "必须是 GetFilterVersion() 函数的参数", 0, 0, SDT_INT, 0, NULL},
    /*045*/ {"筛选器标记", "此标记用于指定本筛选器欲处理的事件类型、端口、优先级等。仅当相应条件（事件/端口）满足时IIS服务器才会调用 HttpFilterProc() 函数。参数值可以是枚举类型常量 #筛选器事件 中的一个或多个，可以是 #筛选器端口 中的一个，也可以是 #筛选器优先级 中的一个，或其数值之和。例如：#筛选器事件.预处理头 + #筛选器事件.结束请求 + #筛选器优先级.默认 + #筛选器端口.非安全端口", 0, 0, SDT_INT, 0, NULL},
    /*046*/ {"筛选器描述", "可以是任意文本，但长度不得超过256字节（128个汉字）。此文本可被IIS服务器获取并显示。", 0, 0, SDT_TEXT, 0, NULL},

    /*047*/ {"筛选器上下文", "必须是 HttpFilterProc() 函数的第一个参数", 0, 0, SDT_INT, 0, NULL},
    /*048*/ {"通知类型", "必须是 HttpFilterProc() 函数的第二个参数", 0, 0, SDT_INT, 0, NULL},
    /*049*/ {"通知数据", "必须是 HttpFilterProc() 函数的第三个参数", 0, 0, SDT_INT, 0, NULL},

    /*050*/ {"服务器变量名称", NULL, 0, 0, SDT_TEXT, 0, NULL},

    /*051*/ {"名称", NULL, 0, 0, SDT_TEXT, 0, NULL},
    /*052*/ {"值", NULL, 0, 0, SDT_TEXT, 0, NULL},

    /*053*/ {"回复头文本", "要求格式如下：“名称1:值1” + #换行符 + “名称2:值2” + #换行符 ……", 0, 0, SDT_TEXT, 0, NULL},

    /*054*/ {"欲写出的数据", NULL, 0, 0, SDT_BIN, 0, NULL},

    /*055*/ {"欲申请的内存长度", NULL, 0, 0, SDT_INT, 0, NULL},

    /*056*/ {"欲调用的函数", "可以为枚举类型常量 #筛选器扩展函数 的成员之一", 0, 0, SDT_INT, 0, NULL},
    /*057*/ {"参数1", "视调用的函数不同，此参数含义也不相同", 0, 0, SDT_INT, 0, NULL},
    /*058*/ {"参数2", "视调用的函数不同，此参数含义也不相同", 0, 0, SDT_INT, 0, NULL},
    /*059*/ {"参数3", "视调用的函数不同，此参数含义也不相同", 0, 0, SDT_INT, 0, NULL},

    /*060*/ {"数据缓冲区地址", NULL, 0, 0, SDT_INT, 0, AS_DEFAULT_VALUE_IS_EMPTY | AS_RECEIVE_VAR},
    /*061*/ {"数据长度", NULL, 0, 0, SDT_INT, 0, AS_DEFAULT_VALUE_IS_EMPTY | AS_RECEIVE_VAR},
    /*062*/ {"缓冲区长度", NULL, 0, 0, SDT_INT, 0, AS_DEFAULT_VALUE_IS_EMPTY | AS_RECEIVE_VAR},

    /*063*/ {"HTTP状态码", "如果此参数未被省略，参数值将被设定为新的HTTP状态码", 0, 0, SDT_INT, 0, AS_DEFAULT_VALUE_IS_EMPTY},

    /*064*/ {"名称", "普通名称必须以\":\"结尾。另有一些特殊名称可供使用：\"method\", \"url\", \"version\", \"status\"，其中前三个在 #筛选器事件.预处理头 和 #筛选器事件.认证完成 事件中有效，最后一个\"status\"在 #筛选器事件.发送回复 事件中有效。", 0, 0, SDT_TEXT, 0, NULL},

    /*065*/ {"名称", "普通名称必须以\":\"结尾。另有一些特殊名称可供使用：\"method\", \"url\", \"version\", \"status\"，其中前三个在 #筛选器事件.预处理头 和 #筛选器事件.认证完成 事件中有效，最后一个\"status\"在 #筛选器事件.发送回复 事件中有效。", 0, 0, SDT_TEXT, 0, NULL},
    /*066*/ {"值", "如果本参数被省略或为空文本，表示从HTTP头中删除对应的名称", 0, 0, SDT_TEXT, 0, AS_DEFAULT_VALUE_IS_EMPTY},

    /*067*/ {"名称", "普通名称必须以\":\"结尾。另有一些特殊名称可供使用：\"method\", \"url\", \"version\", \"status\"，其中前三个在 #筛选器事件.预处理头 和 #筛选器事件.认证完成 事件中有效，最后一个\"status\"在 #筛选器事件.发送回复 事件中有效。", 0, 0, SDT_TEXT, 0, NULL},
    /*068*/ {"值", "如果本参数被省略或为空文本，表示从HTTP头中删除对应的名称", 0, 0, SDT_TEXT, 0, AS_DEFAULT_VALUE_IS_EMPTY},

    /*069*/ {"用户名", NULL, 0, 0, SDT_TEXT, 0, AS_DEFAULT_VALUE_IS_EMPTY | AS_RECEIVE_VAR},
    /*070*/ {"密码", NULL, 0, 0, SDT_TEXT, 0, AS_DEFAULT_VALUE_IS_EMPTY | AS_RECEIVE_VAR},

    /*071*/ {"用户名", "如果文本太长可能会被截断。省略此参数表示不修改用户名。", 0, 0, SDT_TEXT, 0, AS_DEFAULT_VALUE_IS_EMPTY},
    /*072*/ {"密码", "如果文本太长可能会被截断。省略此参数表示不修改密码。", 0, 0, SDT_TEXT, 0, AS_DEFAULT_VALUE_IS_EMPTY},

    /*073*/ {"URL", NULL, 0, 0, SDT_TEXT, 0, NULL},
    /*074*/ {"服务器文件", "如果提供了此参数，表示修改URL映射结果，要求参数是服务器中的一个完整路径文件名。如果此参数被省略，表示不修改URL映射结果。", 0, 0, SDT_TEXT, 0, AS_DEFAULT_VALUE_IS_EMPTY},

    /*075*/ {"主机名", "The client's host name", 0, 0, SDT_TEXT, 0, AS_DEFAULT_VALUE_IS_EMPTY | AS_RECEIVE_VAR},
    /*076*/ {"用户名", "The client's user name", 0, 0, SDT_TEXT, 0, AS_DEFAULT_VALUE_IS_EMPTY | AS_RECEIVE_VAR},
    /*077*/ {"服务器名", "The name of the server to which the client is connected", 0, 0, SDT_TEXT, 0, AS_DEFAULT_VALUE_IS_EMPTY | AS_RECEIVE_VAR},
    /*078*/ {"操作类型", "The HTTP operation type", 0, 0, SDT_TEXT, 0, AS_DEFAULT_VALUE_IS_EMPTY | AS_RECEIVE_VAR},
    /*079*/ {"目标", "The target of the HTTP command", 0, 0, SDT_TEXT, 0, AS_DEFAULT_VALUE_IS_EMPTY | AS_RECEIVE_VAR},
    /*080*/ {"参数", "The parameters passed to the HTTP command", 0, 0, SDT_TEXT, 0, AS_DEFAULT_VALUE_IS_EMPTY | AS_RECEIVE_VAR},
    /*081*/ {"HTTP状态码", "The HTTP return status", 0, 0, SDT_INT, 0, AS_DEFAULT_VALUE_IS_EMPTY | AS_RECEIVE_VAR},
    /*082*/ {"Win32错误码", "The Win32 error code", 0, 0, SDT_INT, 0, AS_DEFAULT_VALUE_IS_EMPTY | AS_RECEIVE_VAR},
    /*083*/ {"发送字节数", "The number of bytes sent from the server to the client", 0, 0, SDT_INT, 0, AS_DEFAULT_VALUE_IS_EMPTY | AS_RECEIVE_VAR},
    /*084*/ {"接收字节数", "The number of bytes received by the server from the client", 0, 0, SDT_INT, 0, AS_DEFAULT_VALUE_IS_EMPTY | AS_RECEIVE_VAR},
    /*085*/ {"处理时间(ms)", "单位毫秒。The time in milliseconds required to process the client request", 0, 0, SDT_INT, 0, AS_DEFAULT_VALUE_IS_EMPTY | AS_RECEIVE_VAR},

    /*086*/ {"主机名", "The client's host name", 0, 0, SDT_TEXT, 0, AS_DEFAULT_VALUE_IS_EMPTY},
    /*087*/ {"用户名", "The client's user name", 0, 0, SDT_TEXT, 0, AS_DEFAULT_VALUE_IS_EMPTY},
    /*088*/ {"服务器名", "The name of the server to which the client is connected", 0, 0, SDT_TEXT, 0, AS_DEFAULT_VALUE_IS_EMPTY},
    /*089*/ {"操作类型", "The HTTP operation type", 0, 0, SDT_TEXT, 0, AS_DEFAULT_VALUE_IS_EMPTY},
    /*090*/ {"目标", "The target of the HTTP command", 0, 0, SDT_TEXT, 0, AS_DEFAULT_VALUE_IS_EMPTY},
    /*091*/ {"参数", "The parameters passed to the HTTP command", 0, 0, SDT_TEXT, 0, AS_DEFAULT_VALUE_IS_EMPTY},
    /*092*/ {"HTTP状态码", "The HTTP return status", 0, 0, SDT_INT, 0, AS_DEFAULT_VALUE_IS_EMPTY},
    /*093*/ {"Win32错误码", "The Win32 error code", 0, 0, SDT_INT, 0, AS_DEFAULT_VALUE_IS_EMPTY},
    /*094*/ {"发送字节数", "The number of bytes sent from the server to the client", 0, 0, SDT_INT, 0, AS_DEFAULT_VALUE_IS_EMPTY},
    /*095*/ {"接收字节数", "The number of bytes received by the server from the client", 0, 0, SDT_INT, 0, AS_DEFAULT_VALUE_IS_EMPTY},
    /*096*/ {"处理时间(ms)", "单位毫秒。The time in milliseconds required to process the client request", 0, 0, SDT_INT, 0, AS_DEFAULT_VALUE_IS_EMPTY},

    /*097*/ {"URL", NULL, 0, 0, SDT_TEXT, 0, AS_DEFAULT_VALUE_IS_EMPTY | AS_RECEIVE_VAR},
    /*098*/ {"服务器文件", NULL, 0, 0, SDT_TEXT, 0, AS_DEFAULT_VALUE_IS_EMPTY | AS_RECEIVE_VAR},
    /*099*/ {"拒绝理由", "应为枚举类型常量 #筛选器拒绝理由.xxx 成员之一", 0, 0, SDT_INT, 0, AS_DEFAULT_VALUE_IS_EMPTY | AS_RECEIVE_VAR},

};

#ifdef _DEBUG    // 这里是为了确认参数序号是否正确, 成员数比最后一个序号大1就是正确
const int dbg_cmd_arg_count__ = sizeof(g_argumentInfo_isapi_global_var) / sizeof(g_argumentInfo_isapi_global_var[0]);
#endif


#define ISAPI_DEF_CMDINFO(_index, _szName, _szEgName, _szExplain, _shtCategory, _wState, _dtRetValType, _wReserved, _shtUserLevel, _shtBitmapIndex, _shtBitmapCount, _nArgCount, _pBeginArgInfo) \
    { _szName, ______E_FNENAME(_szEgName), _szExplain, _shtCategory, _wState, _dtRetValType, _wReserved, _shtUserLevel, _shtBitmapIndex, _shtBitmapCount, _nArgCount, _pBeginArgInfo },


// 易语言每个命令的定义, 会把信息显示在支持库列表里, 这里每个成员都是详细的描述一个命令的信息
CMD_INFO g_cmdInfo_isapi_global_var[] = 
{
    ISAPI_DEF(ISAPI_DEF_CMDINFO)
};

int g_cmdInfo_isapi_global_var_count = sizeof(g_cmdInfo_isapi_global_var) / sizeof(g_cmdInfo_isapi_global_var[0]);

#endif

