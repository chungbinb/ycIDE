#pragma once

#define __EAPI_NAME(_index, _name) __LIB2_FNE_NAME_LEFT(__E_FNENAME)##_##_name##_##_index##_

// 传递函数名和索引, 拼接成 定义库名_名字_序号_定义库名, 比如 eapi_test_0_eapi
#define EAPI_NAME(_index, _name) __LIB2_FNE_NAME_LEFT(__EAPI_NAME(_index, _name))__LIB2_FNE_NAME_LEFT(__E_FNENAME)

// 传递函数名和索引, 拼接成 "定义库名_名字_序号_定义库名", 比如 "eapi_test_0_eapi"
#define EAPI_NAME_STR(_index, _name) ______E_FNENAME(__EAPI_NAME(_index, _name))

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
#define EAPI_DEF(_MAKE) \
    _MAKE(  0, "取键盘指示灯状态", GetKeyboardLockState, "取得三个键盘提示灯当前状态。开返回真；关返回假。本命令为初级命令。", 1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_eapi_global_var + 0)\
    _MAKE(  1, "模拟按键", SimulateKey, "模拟键盘的输入。", 1, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_eapi_global_var + 1)\
    _MAKE(  2, "模拟鼠标点击", SimulateMouse, "模拟鼠标点击。", 1, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_eapi_global_var + 4)\
    _MAKE(  3, "取硬盘信息", GetHDInfo, "取硬件出厂信息。Win95/98/Me中可能无效。特殊情况下也许只能获取部分硬盘信息，其它未被成功获取的部分，将被设置为空文本或0。本命令为初级命令。", 1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_eapi_global_var + 7)\
    _MAKE(  4, "取驱动器数量", GetDrivesNum, "取当前指定类型的驱动器数量", 1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_eapi_global_var + 9)\
    _MAKE(  5, "取驱动器列表", GetDrivesList, "取当前指定类型的驱动器列表。成功返回驱动器列表数组；失败返回空。本命令为初级命令。", 1, _CMD_OS(__OS_WIN) | CT_RETRUN_ARY_TYPE_DATA, SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_eapi_global_var + 9)\
    _MAKE(  6, "弹出光驱", PopupCdrom, "弹出光驱门,成功仅代表命令执行成功。", 1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_eapi_global_var + 0)\
    _MAKE(  7, "关闭光驱", CloseCdrom, "关闭光驱门，成功仅代表命令执行成功。", 1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_eapi_global_var + 0)\
    _MAKE(  8, "取光驱盘符", GetCdrom, "取光驱盘符。失败返回空。", 1, _CMD_OS(__OS_WIN), SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_eapi_global_var + 0)\
    _MAKE(  9, "光驱中是否有盘", IsDiskInside, "光驱中是否有盘。有盘返回真；否则返回假。", 1, _CMD_OS(__OS_WIN), SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_eapi_global_var + 0)\
    _MAKE( 10, "取系统进程列表", IsDiskInside, "取所有正在运行的程序列表。成功返回程序列表数组；失败返回空。本命令为初级命令。", 1, _CMD_OS(__OS_WIN) | CT_RETRUN_ARY_TYPE_DATA, MAKELONG(0x09, 0), 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_eapi_global_var + 0)\
    _MAKE( 11, "终止进程", SimulateKey, "模拟键盘的输入。", 1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_eapi_global_var + 10)\
    _MAKE( 12, "取正在使用DLL列表", GetDllList, "取所有正在使用的DLL列表。成功返回DLL列表数组；失败返回空。", 1, _CMD_OS(__OS_WIN) | CT_RETRUN_ARY_TYPE_DATA, SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_eapi_global_var + 11)\
    _MAKE( 13, "取没有响应程序列表", GetHungProgramList, "取没有响应程序列表。成功返回程序标题数组，失败返回空。本命令为初级命令。", 1, _CMD_OS(__OS_WIN) | CT_RETRUN_ARY_TYPE_DATA, SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_eapi_global_var + 0)\
    _MAKE( 14, "取系统信息", GetSystemInfo, "取系统信息。", 1, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_eapi_global_var + 12)\
    _MAKE( 15, "取BIOS信息", GetBiosInfo, "取BIOS信息。", 1, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_eapi_global_var + 13)\
    _MAKE( 16, "取文件版本信息", GetFileVersionInfoW, "取文件信息版本。", 1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_eapi_global_var + 14)\
    _MAKE( 17, "取CPU信息", GetCpuInfo, "取CPU信息。", 1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_eapi_global_var + 16)\
    _MAKE( 18, "取CPU占用率", GetCpuUsges, "取当前CPU占用率。成功返回CPU占用率；失败返回-1。Win98系统下使用本方法取得的数值可能会有误。本命令为初级命令。", 1, _CMD_OS(__OS_WIN), SDT_DOUBLE, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_eapi_global_var + 0)\
    _MAKE( 19, "取内存容量信息", GetMemoryInfo, "取内存容量信息，单位为M（兆）。", 1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_eapi_global_var + 17)\
    _MAKE( 20, "取声卡名称", GetAudioCard, "取系统声卡名称，失败返回空", 1, _CMD_OS(__OS_WIN), SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_eapi_global_var + 0)\
    _MAKE( 21, "打开屏幕", OpenMonitor, "打开屏幕", 1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_eapi_global_var + 0)\
    _MAKE( 22, "关闭屏幕", CloseMonitor, "关闭屏幕", 1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_eapi_global_var + 0)\
    _MAKE( 23, "添加右键菜单", AddRightMenu, "向指定作用区域文件右键后的菜单内添加选项", 1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 6, g_argumentInfo_eapi_global_var + 18)\
    _MAKE( 24, "删除右键菜单", DeleteRightMenu, "删除指定标题或作用区域右键菜单", 1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_eapi_global_var + 24)\
    _MAKE( 25, "设置自动运行", SetAutoRun, "设置程序在开机时自动运行。本命令为初级命令", 1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_eapi_global_var + 26)\
    _MAKE( 26, "删除临时文件", DeleteTempFile, "删除指定位置的临时文件。成功返回真；失败返回假。", 1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_eapi_global_var + 29)\
    _MAKE( 27, "清除历史记录", ClearHistory, "清除指定类型的历史记录。成功返回真；失败返回假。", 1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_eapi_global_var + 30)\
    _MAKE( 28, "取系统字体列表", GetFontList, "取系统所有字体的列表。成功返回字体数组；失败返回空。", 1, _CMD_OS(__OS_WIN) | CT_RETRUN_ARY_TYPE_DATA, SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_eapi_global_var + 0)\
    _MAKE( 29, "安装字体", AddFont, "在Windows系统中添加一种字体资源。添加完毕后，该字体即可由任何Windows应用程序调用。成功返回1,失败返回0，如加载内存资源则返回安装临时字体的句柄，字体资源进程之间不可共享，可通过删除字体提前删除资源；", 1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_eapi_global_var + 31)\
    _MAKE( 30, "删除字体", RemoveFont, "在从Windows系统中删除一种字体资源。提供临时字体的句柄，则会立即卸载临时字体，如删除系统的字体目前正由其他应用程序使用，则并不将其立即删除。成功返回真；失败返回假。", 1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_eapi_global_var + 32)\
    _MAKE( 31, "取图片宽度", GetImageWidth, "取得指定图片的宽度。成功返回图片宽度；失败返回0。本命令为初级命令。", 1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_eapi_global_var + 33)\
    _MAKE( 32, "取图片高度", GetImageHeight, "取得指定图片的宽度。成功返回图片宽度；失败返回0。本命令为初级命令。", 1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_eapi_global_var + 33)\
    _MAKE( 33, "提取资源文件图标", GetIconFromResource, "从指定的资源文件中提取图标，并保存为ICO、BMP文件或复制到剪贴板。成功返回图标数量；失败返回0。", 1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 4, g_argumentInfo_eapi_global_var + 34)\
    _MAKE( 34, "取IE版本号", GetIEVersion, "取得IE的版本号。成功返回版本号；", 1, _CMD_OS(__OS_WIN), SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_eapi_global_var + 0)\
    _MAKE( 35, "添加IE工具条按钮", AddButtonToIE, "向IE工具栏添加自己的按钮。", 1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 5, g_argumentInfo_eapi_global_var + 38)\
    _MAKE( 36, "删除IE工具条按钮", DeleteButtonFromIE, "向IE工具栏添加自己的按钮。", 1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_eapi_global_var + 43)\
    _MAKE( 37, "创建程序组", CreateProgramGroup, "在指定的位置的特殊位置创建特殊位置文件夹。成功返回真，失败返回假。", 1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_eapi_global_var + 44)\
    _MAKE( 38, "删除程序组", DeleteProgramGroupGroup, "删除指定的特殊位置建立过的程序组。成功返回真，失败返回假。", 1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_eapi_global_var + 46)\
    _MAKE( 39, "创建程序项", CreateProgramItem, "在指定的特殊位置位置程序组文件夹中创建快捷方式。", 1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 4, g_argumentInfo_eapi_global_var + 48)\
    _MAKE( 40, "删除程序项", DeleteProgramItem, "删除指定的特殊位置程序组文件夹中的文件夹快捷方式。成功返回真，失败返回假。", 1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_eapi_global_var + 52)\
    _MAKE( 41, "取快捷方式目标", GetShortCutTarget, "获取指定快捷方式（.lnk）的目标和参数。成功返回目标，失败返回空。只有第一个参数为输入参数，后面的其它参数均为输出参数（可省略）。", 1, _CMD_OS(__OS_WIN), SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 8, g_argumentInfo_eapi_global_var + 55)\
    _MAKE( 42, "取网卡信息列表", GetApapterList, "取得本地计算机的网卡信息列表。失败返回空", 2, _CMD_OS(__OS_WIN) | CT_RETRUN_ARY_TYPE_DATA, MAKELONG(0x08, 0), 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_eapi_global_var + 0)\
    _MAKE( 43, "取本机网卡名", GetLocalAdapterName, "取得本地计算机的网卡信息列表。失败返回空", 2, _CMD_OS(__OS_WIN), SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_eapi_global_var + 63)\
    _MAKE( 44, "取本机网卡物理地址", GetLocalMac, "取得本地计算机指定序号的网卡物理地址。失败返回空。", 2, _CMD_OS(__OS_WIN), SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_eapi_global_var + 63)\
    _MAKE( 45, "取远程网卡物理地址", GetRemoteMac, "根据指定的IP地址取远程机器网卡的物理地址。。失败返回空。", 2, _CMD_OS(__OS_WIN), SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_eapi_global_var + 64)\
    _MAKE( 46, "取远程机器名", GetRemoteName, "根据指定的IP地址取远程机器名称。失败返回空", 2, _CMD_OS(__OS_WIN), SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_eapi_global_var + 64)\
    _MAKE( 47, "取IP地址", GetRemoteName, NULL, 2, _CMD_OS(__OS_WIN), SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_eapi_global_var + 64)\
    _MAKE( 48, "撰写邮件", RunEmailAddr, "调用系统默认邮箱工具并置目标邮箱为参数", 2, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_eapi_global_var + 65)\
    _MAKE( 49, "取网络类型列表", GetNetList, "取所有的网络类型。成功返回程序列表数组；失败返回空", 2, _CMD_OS(__OS_WIN) | CT_RETRUN_ARY_TYPE_DATA, SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_eapi_global_var + 0)\
    _MAKE( 50, "取网络工作组列表", GetGroupList, "取局域网中指定网络类型的工作组列表。成功返回列表数组；失败返回空。", 2, _CMD_OS(__OS_WIN) | CT_RETRUN_ARY_TYPE_DATA, SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_eapi_global_var + 66)\
    _MAKE( 51, "取网络计算机列表", GetComputerList, "取局域网中指定工作组的计算机列表。成功返回列表数组；失败返回空。", 2, _CMD_OS(__OS_WIN) | CT_RETRUN_ARY_TYPE_DATA, SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_eapi_global_var + 67)\
    _MAKE( 52, "是否联网", IsConnectToInternet, "取局域网中指定工作组的计算机列表。成功返回列表数组；失败返回空。", 2, _CMD_OS(__OS_WIN) | CT_RETRUN_ARY_TYPE_DATA, SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_eapi_global_var + 0)\
    _MAKE( 53, "是否存在网络", IsLoginNet, "如果取是否联网推荐使用命令\"是否联网()\"检测机器是否存在网络，\"if a network is present\"，参见MSDN中对GetSystemMetrics()参数SM_NETWORK的说明）。", 2, _CMD_OS(__OS_WIN) | CT_RETRUN_ARY_TYPE_DATA, SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_eapi_global_var + 0)\
    _MAKE( 54, "端口检测", CheckPort, NULL, 2, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_eapi_global_var + 68)\
    _MAKE( 55, "打开特殊系统窗口", OpenSysWindow, "打开特殊的系统窗口，如控制面板。", 3, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_eapi_global_var + 70)\
    _MAKE( 56, "打开指定网址", OpenURL, "用系统默认浏览器打开指定的网址。", 3, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_eapi_global_var + 72)\
    _MAKE( 57, "隐藏桌面图标", HideDesktopIcon, NULL, 3, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_eapi_global_var + 0)\
    _MAKE( 58, "显示桌面图标", ShowDesktopIcon, NULL, 3, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_eapi_global_var + 0)\
    _MAKE( 59, "隐藏任务栏", HideTaskBar, NULL, 3, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_eapi_global_var + 0)\
    _MAKE( 60, "显示任务栏", ShowTaskBar, NULL, 3, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_eapi_global_var + 0)\
    _MAKE( 61, "隐藏系统时钟", HideClock, NULL, 3, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_eapi_global_var + 0)\
    _MAKE( 62, "显示系统时钟", ShowClock, NULL, 3, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_eapi_global_var + 0)\
    _MAKE( 63, "隐藏开始按钮", HideStartButton, NULL, 3, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_eapi_global_var + 0)\
    _MAKE( 64, "显示开始按钮", ShowStartButton, NULL, 3, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_eapi_global_var + 0)\
    _MAKE( 65, "设置桌面墙纸", SetDeskWallPaper, "成功返回真；失败返回假。", 3, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_eapi_global_var + 73)\
    _MAKE( 66, "设置窗口透明度", SetDiaphaneity, "成功返回真；失败返回假。", 3, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 4, g_argumentInfo_eapi_global_var + 75)\
    _MAKE( 67, "取显示模式列表", GetVideoList, "取系统支持的显示模式列表(Win95/98/Me下无法取得刷新频率)。成功返回显示模式信息数组，失败返回空。", 3, _CMD_OS(__OS_WIN) | CT_RETRUN_ARY_TYPE_DATA, MAKELONG(0x0A, 0), 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_eapi_global_var + 0)\
    _MAKE( 68, "取当前显示模式", GetCurVideo, "取系统当前的显示模式(Win95/98/Me下无法取得刷新频率)。", 3, _CMD_OS(__OS_WIN), MAKELONG(0x0A, 0), 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_eapi_global_var + 0)\
    _MAKE( 69, "设置屏幕分辨率", SetResolveRatio, NULL, 3, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 4, g_argumentInfo_eapi_global_var + 79)\
    _MAKE( 70, "屏幕单位转换", ChangeUnit, NULL, 3, _CMD_OS(__OS_WIN), SDT_DOUBLE, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_eapi_global_var + 83)\
    _MAKE( 71, "取当前鼠标处颜色值", GetPointRGB, "取当前鼠标所在位置的RGB值。", 3, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_eapi_global_var + 86)\
    _MAKE( 72, "捕获鼠标", SetCapture, "将鼠标捕获设置到指定的窗口。在鼠标按钮按下的时候，这个窗口会为当前应用程序或整个系统接收所有鼠标输入。返回值为之前拥有鼠标捕获的窗口的句柄，如没有这样的窗口则返回空。", 3, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_eapi_global_var + 89)\
    _MAKE( 73, "释放鼠标", ReleaseCapture, "为当前的应用程序释放鼠标捕获。成功返回真、失败返回假。", 3, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_eapi_global_var + 0)\
    _MAKE( 74, "截取屏幕区域", GetScreenBitma, "将选定的屏幕区域拷贝到位图文件、剪贴板或字节集中。如拷贝到位图文件或剪贴板，则成功返回真；失败返回假。如拷贝到字节集，则成功返回位图字节集；失败返回空。", 3, _CMD_OS(__OS_WIN), _SDT_ALL, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_eapi_global_var + 90)\
    _MAKE( 75, "取所有窗口列表", GetAllWindowsList, "取当前打开的所有窗口列表。成功返回窗口句柄数组，失败返回空。", 3, _CMD_OS(__OS_WIN) | CT_RETRUN_ARY_TYPE_DATA, SDT_INT, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_eapi_global_var + 0)\
    _MAKE( 76, "取窗口标题", GetWindowTextW, "取指定窗口的标题。成功返回窗口标题，失败返回空。", 3, _CMD_OS(__OS_WIN), SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_eapi_global_var + 89)\
    _MAKE( 77, "取窗口类名", GetClassNameW, "取指定窗口的类名。成功返回窗口类名，失败返回空。", 3, _CMD_OS(__OS_WIN), SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_eapi_global_var + 89)\
    _MAKE( 78, "取鼠标所在窗口句柄", GetHwndFromPoint, "取鼠标所在窗口句柄。", 3, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_eapi_global_var + 0)\
    _MAKE( 79, "映射网络驱动器", NetAddConnection, "将指定的网络资源映射到本地驱动器。成功返回真；失败返回假。", 2, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_eapi_global_var + 93)\
    _MAKE( 80, "发送局域网消息", NetSendMessage, "发送消息到指定的机器（即Windows信使服务，只有启动了Messenger服务的机器才能收到该命令发送的消息）。成功返回真；失败返回假。", 2, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_eapi_global_var + 95)\
    _MAKE( 81, "取网络共享资源列表", GetShareResourceList, "取局域网中指定计算机的共享资源列表。成功返回列表数组；失败返回空。", 2, _CMD_OS(__OS_WIN) | CT_RETRUN_ARY_TYPE_DATA, SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_eapi_global_var + 98)\
    _MAKE( 82, "取消网络驱动器映射", CancelNetConnection, "取消指定的网络驱动器映射。成功返回真；", 2, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_eapi_global_var + 100)\
    _MAKE( 83, "取消自动运行", CancelAutoRun, "取消程序的自动运行", 1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_eapi_global_var + 101)\
    _MAKE( 84, "格式化字符串", sprintf, "将一个格式化的字符串输出到一个目的字符串中。给定unicode会返回unicode，此命令与c中sprintf相同，请遵守格式化规则，不把ansi和unicode混用", 1, _CMD_OS(__OS_WIN) | CT_ALLOW_APPEND_NEW_ARG, _SDT_ALL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_eapi_global_var + 102)\
    _MAKE( 85, "取打印机列表", GetPrinterList, "枚举系统打印机列表。", 1, _CMD_OS(__OS_WIN) | CT_RETRUN_ARY_TYPE_DATA, SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_eapi_global_var + 0)\
    _MAKE( 86, "取默认打印机", GetDefaultPrinterW, "取系统默认打印机。成功返回默认打印机名称，失败返回空。", 1, _CMD_OS(__OS_WIN), SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_eapi_global_var + 0)\
    _MAKE( 87, "设置默认打印机", SetDefaultPrinterW, "设置系统的默认打印机。", 1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_eapi_global_var + 104)\
    _MAKE( 88, "打开打印机对话框", OpenPrintSetDlg, "打开打印机设置对话框。返回1：点击了确定按钮；2：点击了取消按钮。", 1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_eapi_global_var + 105)\
    _MAKE( 89, "取屏幕DPI", GetMoniterDPI, "返回指定窗口句柄所处显示器的当前DPI(缩放比例). hWnd: 提供所欲检查窗口句柄,为NULL表示使用桌面窗口(主显示器).", 3, _CMD_OS(__OS_WIN), SDT_DOUBLE, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_eapi_global_var + 107)\
    _MAKE( 90, "隐藏鼠标", HideCursor, "隐藏鼠标光标.注意: 每显示一次,内部计数会加一,每隐藏一次,内部计数会减一,只有当内部计数值大于等于0时,鼠标光标才会被显示.", 3, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_eapi_global_var + 0)\
    _MAKE( 91, "显示鼠标", ShowCursor, "显示鼠标光标.注意: 每显示一次,内部计数会加一,每隐藏一次,内部计数会减一,只有当内部计数值大于等于0时,鼠标光标才会被显示.", 3, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_eapi_global_var + 0)\
    _MAKE( 92, "取图片格式", GetPictureFormat, "获取指定图片格式，失败返回空", 1, _CMD_OS(__OS_WIN), SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_eapi_global_var + 108)\
    _MAKE( 93, "取硬盘编号", GetDiskNumber, "获取硬盘的编号（出场带的）", 1, _CMD_OS(__OS_WIN), SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_eapi_global_var + 0)\
    _MAKE( 94, "蜂鸣", MessageBeep, "通过计算机媒体设备或者喇叭发出一个声音。", 1, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_eapi_global_var + 109)\
    _MAKE( 95, "圆角化窗口", RoundedWindow, " 圆角化置顶窗口", 3, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_eapi_global_var + 110)\
    _MAKE( 96, "窗口置顶", SetForegroundWindow, " 置顶置顶窗口", 3, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_eapi_global_var + 113)\
    _MAKE( 97, "程序提权", UpPrivilegeValue, "提升本程序的权限", 1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_eapi_global_var + 0)

