#pragma once

#define __ISAPI_NAME(_index, _name) __LIB2_FNE_NAME_LEFT(__E_FNENAME)##_##_name##_##_index##_

// 传递函数名和索引, 拼接成 定义库名_名字_序号_定义库名, 比如 isapi_test_0_isapi
#define ISAPI_NAME(_index, _name) __LIB2_FNE_NAME_LEFT(__ISAPI_NAME(_index, _name))__LIB2_FNE_NAME_LEFT(__E_FNENAME)

// 传递函数名和索引, 拼接成 "定义库名_名字_序号_定义库名", 比如 "isapi_test_0_isapi"
#define ISAPI_NAME_STR(_index, _name) ______E_FNENAME(__ISAPI_NAME(_index, _name))

// 这个宏定义了所有的命令, 以后需要命令名数组, 声明命令等, 都可以使用这个宏
// 下面为_MAKE宏参数列表, 除了第一个成员外, 其他都是 CMD_INFO 这个结构的成员
//  0 _index            = 命令索引, 会根据这个索引生成命令名, 这个索引也能让你更直观的看到这个命令是在命令数组里的下标
//  1 _szName           = 命令的中文名, 需要加双引号, 这个会显示在易语言支持库命令列表里
//  2 _szEgName         = 命令的英文名, 不能加双引号, 这个会显示在易语言支持库命令列表英文名字里, 会根据这个英文名字生成c++实现功能函数名, 这个也是静态库导出的符号名
//  3 _szExplain        = 命令详细解释, 需要加双引号, 这个会显示在易语言支持库命令列表的详细解释里
//  4 _shtCategory      = 全局命令的所属类别, 从1开始, 对象成员命令的此值为-1
//  5 _wState           = 标记, CT_开头常量, _CMD_OS(__OS_WIN) 表示支持win系统, _CMD_OS(OS_ALL) 表示支持所有系统, 如果返回数组或者变长参数, 需要或上对应标志位
//  6 _dtRetValType     = 返回值类型, 使用前注意转换HIWORD为0的内部数据类型值到程序中使用的数据类型值
//  7 _wReserved        = 保留字段, 填0
//  8 _shtUserLevel     = 命令的用户学习难度级别, LVL_SIMPLE=初级命令, LVL_SECONDARY=中级命令, LVL_HIGH=高级命令
//  9 _shtBitmapIndex   = 指定图像索引, 从1开始, 0表示无, 显示在支持库列表里的图标
// 10 _shtBitmapCount   = 图像数目(用作动画)
// 11 _nArgCount        = 命令的参数数目
// 12 _pBeginArgInfo    = 参数起始地址
#define ISAPI_DEF(_MAKE) \
    _MAKE(  0, "构造函数", con, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED | CT_IS_OBJ_CONSTURCT_CMD, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_isapi_global_var + 0)\
    _MAKE(  1, "析构函数", des, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED | CT_IS_OBJ_FREE_CMD, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_isapi_global_var + 0)\
    _MAKE(  2, "初始化版本信息", InitVersionInfo, "本方法应该在GetExtensionVersion子程序中调用", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_isapi_global_var + 0)\
    _MAKE(  3, "初始化互联网扩展", InitHttp, "本方法应该在(HttpExtensionProc)子程序中调用", -1, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_isapi_global_var + 2)\
    _MAKE(  4, "写出文本", WriteText, "写出文本给客户端。文本一般是(HTML)文本。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_isapi_global_var + 3)\
    _MAKE(  5, "取服务器变量", GetServerVar, "取服务器变量。", -1, _CMD_OS(__OS_WIN), SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_isapi_global_var + 4)\
    _MAKE(  6, "获取传入数据方法", GetDataMethoed, "获取传入数据方法，成功返回“GET”或“POST”。", -1, _CMD_OS(__OS_WIN), SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_isapi_global_var + 0)\
    _MAKE(  7, "获取内容类型", GetContentType, "获取内容类型(ContentType)。", -1, _CMD_OS(__OS_WIN), SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_isapi_global_var + 0)\
    _MAKE(  8, "获取路径信息", GetPathInfo, "获取路径信息。如客户端传入 xxx.dll/111.txt。0: 相对路径为 /111.txt，1: 绝对路径为 c:\\wwwroot\\interput\\111.txt, c:\\wwwroot\\interput为服务器所在目录", -1, _CMD_OS(__OS_WIN), SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_isapi_global_var + 5)\
    _MAKE(  9, "获取客户端字符串", GetQueryString, "获取客户端传来的字符串，是客户端用获取(GET)方法传来的字符串。", -1, _CMD_OS(__OS_WIN), SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_isapi_global_var + 0)\
    _MAKE( 10, "读取文本", ReadText, "读取客户端用投递(POST)方法传入的文本。", -1, _CMD_OS(__OS_WIN), SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_isapi_global_var + 0)\
    _MAKE( 11, "解码文本", DecodeText, "将客户端用获取(GET)或用投递(POST)传上来的文本进行解码。", -1, _CMD_OS(__OS_WIN), SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_isapi_global_var + 6)\
    _MAKE( 12, "据关键字找文本", GetKeyField, "根据关键字找文本。如这样一个文本“key1=value1+key2=value2”根据“key”找“value”。", -1, _CMD_OS(__OS_WIN), SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_isapi_global_var + 8)\
    _MAKE( 13, "设置替换标志", SetReplaceFlag, "设置替换HTML模板的标志，如:<!--$站点$-->,“站点”前后的就是标志。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_isapi_global_var + 11)\
    _MAKE( 14, "获取替换标志", GetReplaceFlag, "获取“设置替换标志”方法设置的标志。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_isapi_global_var + 13)\
    _MAKE( 15, "模板替换", TemplateReplace, "对给定的HTML模板进行替换, 注如果有相同的变量只替换一次。调用此方法之前要调用“设置替换标志”设置标志", -1, _CMD_OS(__OS_WIN) | CT_ALLOW_APPEND_NEW_ARG, SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_isapi_global_var + 15)\
    _MAKE( 16, "设置存根", SetCookie, "设定存根(Cookie)。注本功能需要浏览器的支持。", -1, _CMD_OS(__OS_WIN) | CT_ALLOW_APPEND_NEW_ARG, SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 7, g_argumentInfo_isapi_global_var + 17)\
    _MAKE( 17, "获取存根", GetCookie, "获取存根(Cookie)的值。", -1, _CMD_OS(__OS_WIN), SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_isapi_global_var + 24)\
    _MAKE( 18, "会话开始", SessionStart, "开始一个新的会话(session)。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_isapi_global_var + 25)\
    _MAKE( 19, "会话结束", SessionEnd, "结束当前的会话。", -1, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_isapi_global_var + 0)\
    _MAKE( 20, "会话注册变量和值", SessionRegVarAndVal, "为当前会话注册变量和值，如果变量已注册则修改变量的值。", -1, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_isapi_global_var + 26)\
    _MAKE( 21, "会话变量是否注册", SessionIsReg, "检查当前会话中此变量是否注册。如果注册过了返回真，否则返回假。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_isapi_global_var + 28)\
    _MAKE( 22, "会话取消注册变量", SessionUnReg, "从当前会话中取消此变量。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_isapi_global_var + 29)\
    _MAKE( 23, "会话获取变量值", SessionGetVal, "从当前会话获取此变量的值。", -1, _CMD_OS(__OS_WIN), SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_isapi_global_var + 30)\
    _MAKE( 24, "会话轮询时间", SessionInTurnTime, "程序多长时间轮询一次，将失效的会话删掉。", -1, _CMD_OS(__OS_WIN), SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_isapi_global_var + 31)\
    _MAKE( 25, "编码文本", EncodeText, "将一段文本编码为URL格式的。", -1, _CMD_OS(__OS_WIN), SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_isapi_global_var + 32)\
    _MAKE( 26, "读取字节集", ReadBin, "读取客户端用投递(POST)方法传入的内容，并以字节集的方式返回。", -1, _CMD_OS(__OS_WIN), SDT_BIN, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_isapi_global_var + 34)\
    _MAKE( 27, "重定向", Redirect, "进行重定向，让浏览器访问一个指定的网页。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_isapi_global_var + 35)\
    _MAKE( 28, "调用扩展函数", ServerSupportFunction, "调用服务器提供的扩展函数，完成其他各种功能，具体视参数“欲调用的函数”而定", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 4, g_argumentInfo_isapi_global_var + 36)\
    _MAKE( 29, "写出字节集", WriteBinary, NULL, -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_HIGH, 0, 0, 1, g_argumentInfo_isapi_global_var + 40)\
    _MAKE( 30, "写出回复头", SendResponeHeaderEx, "等效于调用扩展函数HSE_REQ_SEND_RESPONSE_HEADER_EX", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_HIGH, 0, 0, 3, g_argumentInfo_isapi_global_var + 41)\
    _MAKE( 31, "无法识别的名字_31", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_isapi_global_var + 0)\
    _MAKE( 32, "无法识别的名字_32", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_isapi_global_var + 0)\
    _MAKE( 33, "无法识别的名字_33", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_isapi_global_var + 0)\
    _MAKE( 34, "无法识别的名字_34", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_isapi_global_var + 0)\
    _MAKE( 35, "无法识别的名字_35", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_isapi_global_var + 0)\
    _MAKE( 36, "无法识别的名字_36", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_isapi_global_var + 0)\
    _MAKE( 37, "无法识别的名字_37", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_isapi_global_var + 0)\
    _MAKE( 38, "无法识别的名字_38", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_isapi_global_var + 0)\
    _MAKE( 39, "无法识别的名字_39", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_isapi_global_var + 0)\
    _MAKE( 40, "无法识别的名字_40", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_isapi_global_var + 0)\
    _MAKE( 41, "无法识别的名字_41", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_isapi_global_var + 0)\
    _MAKE( 42, "无法识别的名字_42", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_isapi_global_var + 0)\
    _MAKE( 43, "无法识别的名字_43", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_isapi_global_var + 0)\
    _MAKE( 44, "无法识别的名字_44", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_isapi_global_var + 0)\
    _MAKE( 45, "无法识别的名字_45", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_isapi_global_var + 0)\
    _MAKE( 46, "无法识别的名字_46", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_isapi_global_var + 0)\
    _MAKE( 47, "无法识别的名字_47", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_isapi_global_var + 0)\
    _MAKE( 48, "无法识别的名字_48", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_isapi_global_var + 0)\
    _MAKE( 49, "无法识别的名字_49", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_isapi_global_var + 0)\
    _MAKE( 50, "无法识别的名字_50", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_isapi_global_var + 0)\
    _MAKE( 51, "无法识别的名字_51", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_isapi_global_var + 0)\
    _MAKE( 52, "无法识别的名字_52", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_isapi_global_var + 0)\
    _MAKE( 53, "无法识别的名字_53", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_isapi_global_var + 0)\
    _MAKE( 54, "无法识别的名字_54", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_isapi_global_var + 0)\
    _MAKE( 55, "无法识别的名字_55", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_isapi_global_var + 0)\
    _MAKE( 56, "无法识别的名字_56", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_isapi_global_var + 0)\
    _MAKE( 57, "无法识别的名字_57", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_isapi_global_var + 0)\
    _MAKE( 58, "无法识别的名字_58", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_isapi_global_var + 0)\
    _MAKE( 59, "构造函数", constructor, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED | CT_IS_OBJ_CONSTURCT_CMD, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_isapi_global_var + 0)\
    _MAKE( 60, "析构函数", destructor, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED | CT_IS_OBJ_FREE_CMD, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_isapi_global_var + 0)\
    _MAKE( 61, "备用拷贝构造函数", copyconstructor, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_isapi_global_var + 0)\
    _MAKE( 62, "初始化版本信息", FilterInitVersion, "在筛选器DLL的公开导出函数 GetFilterVersion() 中必须调用本方法", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_isapi_global_var + 44)\
    _MAKE( 63, "初始化互联网筛选器", FilterInitFilterProc, "在筛选器DLL的公开导出函数 HttpFilterProc() 中必须首先调用本方法", -1, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_isapi_global_var + 47)\
    _MAKE( 64, "取事件类型", FilterGetEventType, "返回当前的事件类型，返回值应该是枚举类型常量 #筛选器事件 的成员之一。", -1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_isapi_global_var + 0)\
    _MAKE( 65, "是否为安全端口", FilterIsSecurePort, NULL, -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_isapi_global_var + 0)\
    _MAKE( 66, "取服务器变量", FilterGetServerVariable, NULL, -1, _CMD_OS(__OS_WIN), SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_isapi_global_var + 50)\
    _MAKE( 67, "添加回复头", FilterAddResponseHeader, "建议在 #筛选器事件.发送回复 之前的筛选器事件中使用此命令。当“取事件类型()”返回值为 #筛选器事件.预处理头、#筛选器事件.发送回复、#筛选器事件.认证完成 的情况下，应使用“加HTTP头()”或“置HTTP头()”", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_isapi_global_var + 51)\
    _MAKE( 68, "批量添加回复头", FilterAddResponseHeaders, "建议在 #筛选器事件.发送回复 之前的筛选器事件中使用此命令。当“取事件类型()”返回值为 #筛选器事件.预处理头、#筛选器事件.发送回复、#筛选器事件.认证完成 的情况下，应使用“加HTTP头()”或“置HTTP头()”", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_isapi_global_var + 53)\
    _MAKE( 69, "向客户端写数据", FilterWriteClient, NULL, -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_isapi_global_var + 54)\
    _MAKE( 70, "申请内存", FilterAllocMem, "执行成功返回申请到的内存地址，否则返回假。申请到的内存会在连接结束时被自动释放。", -1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_isapi_global_var + 55)\
    _MAKE( 71, "调用扩展函数", FilterServerSupportFunction, "执行成功返回真，否则返回假", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 4, g_argumentInfo_isapi_global_var + 56)\
    _MAKE( 72, "取读写数据", FilterGetRawData, "仅当“取事件类型()”返回值为 #筛选器事件.读取数据 或 #筛选器事件.发送数据 的情况下才能调用此方法。执行成功返回真，否则返回假", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_isapi_global_var + 60)\
    _MAKE( 73, "HTTP状态码", FilterGetHttpStatus, "返回HTTP状态码，也可以通过参数设定新的HTTP状态码。仅当“取事件类型()”返回值为 #筛选器事件.预处理头 的情况下才能调用此方法。", -1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_isapi_global_var + 63)\
    _MAKE( 74, "取HTTP头", FilterGetHeader, "返回HTTP头中指定名称所对应的值。仅当“取事件类型()”返回值为 #筛选器事件.预处理头、#筛选器事件.发送回复、#筛选器事件.认证完成 的情况下才能调用此方法。", -1, _CMD_OS(__OS_WIN), SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_isapi_global_var + 64)\
    _MAKE( 75, "置HTTP头", FilterSetHeader, "设置HTTP头中指定名称所对应的值，或在参数“值”被省略或为空文本的情况下删除“名称”。仅当“取事件类型()”返回值为 #筛选器事件.预处理头、#筛选器事件.发送回复、#筛选器事件.认证完成 的情况下才能调用此方法。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_isapi_global_var + 65)\
    _MAKE( 76, "加HTTP头", FilterAddHeader, "添加指定名称和值到HTTP头中，或在参数“值”被省略或为空文本的情况下删除“名称”。仅当“取事件类型()”返回值为 #筛选器事件.预处理头、#筛选器事件.发送回复、#筛选器事件.认证完成 的情况下才能调用此方法。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_isapi_global_var + 67)\
    _MAKE( 77, "取认证信息", FilterGetAuthInfo, "获取客户端提交的用户名密码认证信息。仅当“取事件类型()”返回值为 #筛选器事件.认证 的情况下才能调用本方法。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_isapi_global_var + 69)\
    _MAKE( 78, "置认证信息", FilterSetAuthInfo, "修改客户端提交的用户名密码认证信息。仅当“取事件类型()”返回值为 #筛选器事件.认证 的情况下才能调用本方法。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_isapi_global_var + 71)\
    _MAKE( 79, "URL映射信息", FilterURLMap, "获取或修改URL映射信息，返回URL映射到的服务器文件名。仅当“取事件类型()”返回值为 #筛选器事件.映射URL 的情况下才能调用本方法。", -1, _CMD_OS(__OS_WIN), SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_isapi_global_var + 73)\
    _MAKE( 80, "取日志信息", FilterGetLogInfo, "获取日志信息，通过变量参数返回。仅当“取事件类型()”返回值为 #筛选器事件.记录日志 的情况下才能调用本方法。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 11, g_argumentInfo_isapi_global_var + 75)\
    _MAKE( 81, "置日志信息", FilterSetLogInfo, "修改日志信息。修改后的日志信息，将替换原有日志信息被IIS写入日志中。仅当“取事件类型()”返回值为 #筛选器事件.记录日志 的情况下才能调用本方法。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 11, g_argumentInfo_isapi_global_var + 86)\
    _MAKE( 82, "取拒绝访问信息", FilterGetAccessDenied, "获取拒绝访问信息。仅当“取事件类型()”返回值为 #筛选器事件.拒绝访问 的情况下才能调用本方法。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_isapi_global_var + 97)

