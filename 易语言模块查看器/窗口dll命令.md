.版本 2

.DLL命令 api_SHChangeNotify, , , "SHChangeNotify", , 刷新图标_
    .参数 a, 整数型, , 134217728
    .参数 b, 整数型, , 0
    .参数 c, 整数型, , 0
    .参数 d, 整数型, , 0

.DLL命令 api_GetMenu, 整数型, "user32", "GetMenu", , 取菜单_
    .参数 窗口句柄, 整数型

.DLL命令 api_GetMenuItemRect, 整数型, "user32", "GetMenuItemRect", , 取菜单条目尺寸_
    .参数 窗口句柄, 整数型
    .参数 菜单句柄, 整数型
    .参数 位置, 整数型
    .参数 矩形位置, 矩形, 传址

.DLL命令 api_GetWindow, 整数型, "user32", "GetWindow", , 获得一个窗口的句柄，该窗口与某源窗口有特定的关系
    .参数 hwnd, 整数型, , 源窗口
    .参数 wCmd, 整数型, , 指定结果窗口与源窗口的关系

.DLL命令 api_GetParent, 整数型, , "GetParent"
    .参数 hWnd, 整数型

.DLL命令 api_SetWindowLongA, 整数型, , "SetWindowLongA"
    .参数 hWnd, 整数型
    .参数 nIndex, 整数型
    .参数 dwNewLong, 子程序指针

.DLL命令 api_SetWindowLongA_拖放, 子程序指针, "user32", "SetWindowLongA"
    .参数 hwnd, 整数型
    .参数 nIndex, 整数型
    .参数 dwNewLong, 子程序指针

.DLL命令 api_GetWindowLongA, 整数型, , "GetWindowLongA"
    .参数 hWnd, 整数型
    .参数 nIndex, 整数型

.DLL命令 api_CallWindowProcA, 整数型, , "CallWindowProcA"
    .参数 lpPrevWndFunc, 整数型, , 可能为一个 WNDPROC 子程序指针。
    .参数 hWnd, 整数型
    .参数 Msg, 整数型
    .参数 wParam, 整数型
    .参数 lParam, 整数型

.DLL命令 api_GetWindowRect, 逻辑型, , "GetWindowRect", , 取窗口矩形_
    .参数 hWnd, 整数型
    .参数 lpRect, 矩形, , 未知类型：LPRECT。

.DLL命令 api_GetClientRect, 逻辑型, , "GetClientRect"
    .参数 hWnd, 整数型
    .参数 lpRect, 矩形, , 未知类型：LPRECT。

.DLL命令 api_EnumChildWindows, 逻辑型, , "EnumChildWindows"
    .参数 hWndParent, 整数型
    .参数 lpEnumFunc, 子程序指针, , 可能为一个 WNDENUMPROC 子程序指针。
    .参数 lParam, 整数型

.DLL命令 api_IsWindowVisible, 逻辑型, , "IsWindowVisible"
    .参数 hWnd, 整数型

.DLL命令 api_CreateThread, 整数型, , "CreateThread", , 生成线程成功，CreateThread函数返回新线程的句柄。否则返回NULL。线程执行完毕后自动退出（除非进入的是循环）。
    .参数 lpThreadAttributes, 整数型, , 线程的安全属性，缺省为0
    .参数 dwStackSize, 整数型, , 线程的堆栈大小，为0时与进程相同
    .参数 lpStartAddress, 子程序指针, , 线程函数的起始地址
    .参数 lpParameter, 整数型, , 传递线程的上下文,可以传递一个整数型参数。
    .参数 dwCreationFlags, 整数型, , 0：创建线程后立即启动，相反的是标志位CREATE_SUSPENDED，这样您需要稍后显示地让该线程运行（可设为－1）
    .参数 lpThreadId, 整数型, 传址, 内核给新生成的线程分配的线程ID

.DLL命令 api_TerminateThread, , , "TerminateThread", , 强行中止线程。
    .参数 hThread, 整数型, , CreateThread函数返回的线程句柄
    .参数 dwExitCode, 整数型

.DLL命令 api_ResumeThread, , , "ResumeThread", , 执行挂起的线程，执行完毕后线程自动退出
    .参数 hThread, 整数型, , CreateThread函数返回的线程句柄

.DLL命令 api_CallWindowProcA_拖放1, 整数型, "user32", "CallWindowProcA"
    .参数 lpPrevWndFunc, 子程序指针
    .参数 hWnd, 整数型
    .参数 Msg, 整数型
    .参数 wParam, 整数型
    .参数 lParam, 整数型

.DLL命令 api_CallWindowProcA_拖放2, 整数型, "user32", "CallWindowProcA"
    .参数 lpPrevWndFunc, 子程序指针
    .参数 总文件数量, 整数型
    .参数 当前是第几个文件, 整数型
    .参数 当前文件名, 文本型
    .参数 当前文件后缀, 文本型

.DLL命令 api_DragAcceptFiles, 整数型, "shell32.dll", "DragAcceptFiles"
    .参数 hwnd, 整数型
    .参数 fAccept, 整数型

.DLL命令 api_DragQueryFile, 整数型, "shell32.dll", "DragQueryFileA"
    .参数 hDrop, 整数型
    .参数 iFile, 整数型
    .参数 lpszFile, 文本型
    .参数 cch, 整数型

.DLL命令 api_DragFinish, 整数型, "shell32.dll", "DragFinish"
    .参数 hDrop, 整数型

.DLL命令 api_SendMessageA_文本, 整数型, "user32", "SendMessageA"
    .参数 窗口句柄, 整数型, , 要接收消息的那个窗口的句柄
    .参数 信息值, 整数型, , 消息的标识符
    .参数 参数一, 整数型, , 具体取决于消息
    .参数 参数二, 文本型, , 具体取决于消息;

.DLL命令 注册表_枚举子项, 整数型, , "RegEnumKeyA", , advapi32.dll
    .参数 hKey, 整数型
    .参数 dwIndex, 整数型
    .参数 lpName, 文本型, 传址
    .参数 cbName, 整数型

.DLL命令 注册表_枚举值, 整数型, , "RegEnumValueA", , advapi32.dll零（ERROR_SUCCESS）表示成功。其他任何值都代表一个错误代码
    .参数 句柄, 整数型, , 一个已打开项的句柄，或者指定一个标准项名
    .参数 表项索引, 整数型, , 欲获取值的索引。注意第一个值的索引编号为零
    .参数 子键名, 文本型, , 用于装载位于指定索引处值名的一个缓冲区
    .参数 子键长度, 整数型, 传址, 用于装载lpValueName缓冲区长度的一个变量。一旦返回，它会设为实际载入缓冲区的字符数量
    .参数 lpReserved, 整数型, , 未用设为零
    .参数 键值类型, 整数型, 传址, 值的类型，用于装载值的类型代码的变量
    .参数 键值数据, 字节集, 传址, 用于装载值数据的一个缓冲区
    .参数 键值缓寸, 整数型, 传址, 用于装载lpData缓冲区长度的一个变量。一旦返回，它会设为实际载入缓冲区的字符数量

.DLL命令 注册表_取值, 整数型, , "RegQueryValueExA", , advapi32.dll获取一个项的设置值  零（ERROR_SUCCESS）表示成功。其他任何值都代表一个错误代码
    .参数 hKey, 整数型, , 一个已打开项的句柄，或者指定一个标准项名
    .参数 lpValueName, 文本型, , 要获取值的名字
    .参数 lpReserved, 整数型, , 未用，设为零
    .参数 lpType, 整数型, 传址, 用于装载取回数据类型的一个变量
    .参数 lpData, 字节集, 传址, 用于装载指定值的一个缓冲区
    .参数 lpcbData, 整数型, 传址, 用于装载lpData缓冲区长度的一个变量。一旦返回，它会设为实际装载到缓冲区的字节数;

.DLL命令 注册表_删除值, 整数型, , "RegDeleteValueA", , advapi32.dll删除指定项下方的一个值  零（ERROR_SUCCESS）表示成功。其他任何值都代表一个错误代码
    .参数 hKey, 整数型, , 一个已打开项的句柄，或标准项名之一
    .参数 lpValueName, 文本型, , 要删除的值名。可设为vbNull或一个空串，表示删除那个项的默认值;

.DLL命令 注册表_创建项, 整数型, , "RegCreateKeyA", , advapi32.dll在指定的项下创建一个新项。如指定的项已经存在，那么函数会打开现有的项  零（ERROR_SUCCESS）表示成功。其他任何值都代表一个错误代码
    .参数 hKey, 整数型, , 要打开项的句柄，或者一个标准项名
    .参数 lpSubKey, 文本型, , 欲创建的新子项。可同时创建多个项，只需用反斜杠将它们分隔开即可。例如level1\level2\newkey
    .参数 phkResult, 整数型, 传址, 指定一个变量，用于装载新子项的句柄;

.DLL命令 注册表_打开项, 整数型, , "RegOpenKeyA", , advapi32.dll打开一个现有的注册表项  零（ERROR_SUCCESS）表示成功。其他任何值都代表一个错误代码
    .参数 hKey, 整数型, , 一个已打开项的句柄，或指定一个标准项名
    .参数 lpSubKey, 文本型, , 要打开的项名
    .参数 phkResult, 整数型, 传址, 指定一个变量，用于装载（保存）打开注册表项的一个句柄;

.DLL命令 注册表_删除项, 整数型, , "RegDeleteKeyA", , advapi32.dll删除现有项下方一个指定的子项  零（ERROR_SUCCESS）表示成功。其他任何值都代表一个错误代码
    .参数 hKey, 整数型, , 一个已打开项的句柄，或者标准项名之一
    .参数 lpSubKey, 文本型, , 要删除项的名字。这个项的所有子项也会删除;

.DLL命令 注册表_取项信息, 整数型, , "RegQueryInfoKeyA", , advapi32.dll获取与一个项有关的信息  零（ERROR_SUCCESS）表示成功。其他任何值都代表一个错误代码。如一个缓冲区的长度不够，不能容下返回的数据，则函数会返回ERROR_MORE_DATA
    .参数 hKey, 整数型, , 一个已打开项的句柄，或指定一个标准项名
    .参数 lpClass, 整数型, , 指定一个字串，用于装载这个注册表项的类名
    .参数 lpcbClass, 整数型, , 指定一个变量，用于装载lpClass缓冲区的长度。一旦返回，它会设为实际装载到缓冲区的字节数量
    .参数 lpReserved, 整数型, , 未用，设为零
    .参数 lpcSubKeys, 整数型, 传址, 用于装载（保存）这个项的子项数量的一个变量
    .参数 lpcbMaxSubKeyLen, 整数型, , 指定一个变量，用于装载这个项最长一个子项的长度。注意这个长度不包括空中止字符
    .参数 lpcbMaxClassLen, 整数型, , 指定一个变量，用于装载这个项之子项的最长一个类名的长度。注意这个长度不包括空中止字符
    .参数 lpcValues, 整数型, , 用于装载这个项的设置值数量的一个变量
    .参数 lpcbMaxValueNameLen, 整数型, , 指定一个变量，用于装载这个项之子项的最长一个值名的长度。注意这个长度不包括空中止字符
    .参数 lpcbMaxValueLen, 整数型, , 指定一个变量，用于装载容下这个项最长一个值数据所需的缓冲区长度
    .参数 lpcbSecurityDescriptor, 整数型, , 装载值安全描述符长度的一个变量
    .参数 lpftLastWriteTime, 整数型, , FILETIME，指定一个结构，用于容纳该项的上一次修改时间;

.DLL命令 注册表_关闭项, 整数型, , "RegCloseKey", , advapi32.dll关闭系统注册表中的一个项（或键）  零（ERROR_SUCCESS）表示成功。其他任何值都代表一个错误代码
    .参数 hKey, 整数型, , 要关闭的项;

.DLL命令 api_keybd_event, 整数型, "user32.dll", "keybd_event", ,  模拟键盘行动
    .参数 键代码, 整数型, , bVk,欲模拟的虚拟键码
    .参数 键扫描码, 整数型, , bScan,键的OEM扫描码
    .参数 标志值, 整数型, , dwFlags,标志常数
    .参数 特定值, 整数型, , dwExtraInfo,通常不用的一个值

.DLL命令 api_PostMessageA_整数型, 整数型, "user32", "PostMessageA"
    .参数 窗口句柄, 整数型, , 要接收消息的那个窗口的句柄
    .参数 信息值, 整数型, , 消息的标识符
    .参数 参数一, 整数型, , 具体取决于消息
    .参数 参数二, 整数型, , 具体取决于消息;

.DLL命令 api_PostMessageA_文本型, 整数型, "user32", "PostMessageA"
    .参数 窗口句柄, 整数型, , 要接收消息的那个窗口的句柄
    .参数 信息值, 整数型, , 消息的标识符
    .参数 参数一, 整数型, , 具体取决于消息
    .参数 参数二, 文本型, , 具体取决于消息;

.DLL命令 api_GetCursorPos, 逻辑型, , "GetCursorPos", , user32.dll_取鼠标位置
    .参数 光标位置, 坐标, 传址

.DLL命令 api_FindWindowExA, 整数型, "user32", "FindWindowExA"
    .参数 窗口句柄1, 整数型
    .参数 窗口句柄2, 整数型
    .参数 欲搜索的类名1, 文本型
    .参数 欲搜索的类名2, 文本型, , 没有填 字符 (0)

.DLL命令 api_FindWindow, 整数型, "user32", "FindWindowA", , 寻找窗口列表中第一个符合指定条件的顶级窗口
    .参数 lpClassName, 文本型, , 窗口类名
    .参数 lpWindowName, 文本型, , 窗口文本（标题）

.DLL命令 api_FindWindowA, 整数型, "user32", "FindWindowA"
    .参数 窗口类名, 文本型
    .参数 窗口文本, 文本型

.DLL命令 api_GetForegroundWindow, 整数型, , "GetForegroundWindow", 公开, user32.dll返回前台窗口句柄

.DLL命令 api_ClientToScreen, 整数型, , "ClientToScreen", , user32.dll_取窗口区域位置,非零表示成功，零表示失败
    .参数 窗口句柄, 整数型, , hwnd，要取出区域坐标的窗口句柄
    .参数 位置变量, 坐标, , lpPoint，参见相关帮助

.DLL命令 api_GetWindowThreadProcessId, 整数型, "user32.dll", "GetWindowThreadProcessId", , 取窗口进程ID，获取与指定窗口关联在一起的一个线程和进程标识符
    .参数 窗口句柄, 整数型, , hwnd，指定窗口句柄
    .参数 进程标识符, 整数型, 传址, lpdwProcessId，指定一个变量，用于装载拥有那个窗口的一个进程的标识符

.DLL命令 api_GetCurrentThreadId, 整数型, , "GetCurrentThreadId", , ,_取当前线程获取当前线程一个唯一的线程标识符(返回线程标识符)

.DLL命令 api_AttachThreadInput, 整数型, , "AttachThreadInput", , _连接线程输入,允许线程和进程共享输入队列。连接了线程后，输入焦点、窗口激活、鼠标捕获、键盘状态以及输入队列状态都会进入共享状态
    .参数 dwthreadthis, , , 欲连接线程的标识符（ID）
    .参数 dwthreadforeground, , , 与idAttach线程连接的另一个线程的标识符
    .参数 fAttach, 逻辑型, , TRUE（非零）连接，FALSE撤消连接

.DLL命令 api_GetFocus, 整数型, , "GetFocus", , 获得拥有输入焦点的窗口的句柄(输入框句柄)。如没有窗口拥有输入焦点，则返回零

.DLL命令 api_SetFocus, 整数型, , "SetFocus", , 将输入焦点设到指定的窗口。如有必要，会激活窗口  (返回前一个拥有焦点的窗口的句柄)
    .参数 窗口句柄, 整数型, , 准备接收焦点的窗口的句柄(必须为同一线程)

.DLL命令 api_ShowWindow, 整数型, , "ShowWindow", , 控制窗口的状态（在vb里使用：针对vb窗体及控件，请使用对应的vb属性） 成功:返回TRUE（非零），失败:返回FALSE（零）
    .参数 hwnd, 整数型, , 窗口句柄，要向这个窗口应用由命令
    .参数 nCmdShow, 整数型, , 为窗口指定的一个命令。请用下述任何一个常数:0 隐藏取消激活  1 还原激活 2 最小化激活 3 最大化激活 4 还原 6 最小化取消激活 7 最小化 9 还原激活

.DLL命令 api_SetForegroundWindow, 整数型, , "SetForegroundWindow", 公开, 将窗口设为系统的前台窗口。可用于改变用户目前正在操作的应用程序 (非零表示成功，零表示失败)
    .参数 窗口句柄, 整数型, , 带到前台的窗口

.DLL命令 api_SetActiveWindow, 整数型, , "SetActiveWindow", , 激活指定的窗口，返回前一个活动窗口的句柄
    .参数 hwnd, 整数型, , 待激活窗口的句柄

.DLL命令 api_SetCursorPos, , "user32", "SetCursorPos", 公开
    .参数 x, 整数型
    .参数 y, 整数型

.DLL命令 api_SHGetPathFromIDListA, 整数型, , "SHGetPathFromIDListA", , _从列表id取路径
    .参数 pIdl, 整数型
    .参数 pszPath, 文本型, 传址

.DLL命令 api_SHBrowseForFolder, 整数型, , "SHBrowseForFolderA"
    .参数 lpBrowseInfo, BROWSEINFO

.DLL命令 api_ShellExecute, 整数型, "shell32.dll", "ShellExecuteA", , ;    查找与指定文件关联在一起的程序的文件名  非零表示成功，零表示失败。会设置GetLastError
    .参数 hwnd, 整数型, , 指定一个窗口的句柄，有时候，windows程序有必要在创建自己的主窗口前显示一个消息框
    .参数 lpOperation, 文本型, , 指定字串“open”来打开lpFlie文档，或指定“Print”来打印它
    .参数 lpFile, 文本型, , 想用关联程序打印或打开一个程序名或文件名
    .参数 lpParameters, 文本型, , 如lpszFlie是可执行文件，则这个字串包含传递给执行程序的参数
    .参数 lpDirectory, 文本型, , 想使用的完整路径
    .参数 nShowCmd, 整数型, , 定义了如何显示启动程序的常数值。参考ShowWindow函数的nCmdShow参数;

.DLL命令 CopyTo_系统信息段, , , "RtlMoveMemory"
    .参数 Dest, 系统信息段
    .参数 Src, 字节集
    .参数 Length

.DLL命令 CopyTo_SectionInfo, , , "RtlMoveMemory"
    .参数 Dest, SectionInfo
    .参数 Src, 字节集
    .参数 Length

.DLL命令 CopyFrom_SectionInfo, , , "RtlMoveMemory"
    .参数 Dest, 字节集
    .参数 Src, SectionInfo
    .参数 Length

.DLL命令 CopyFrom_SectionHeader, , , "RtlMoveMemory"
    .参数 Dest, 字节集
    .参数 Src, SectionHeader
    .参数 Length

.DLL命令 api_GlobalSize, 整数型, , "GlobalSize"
    .参数 p, 系统信息段

.DLL命令 lstrcpyA_字节型, , , "lstrcpyA"
    .参数 dest, 字节型, 数组
    .参数 src, 文本型

.DLL命令 api_MoveWindow, 整数型, , "MoveWindow", , user32.dll
    .参数 hwnd, 整数型
    .参数 x, 整数型
    .参数 y, 整数型
    .参数 n, 整数型
    .参数 nHeight, 整数型
    .参数 bRepaint, 逻辑型

.DLL命令 api_SHFileOperationA, 整数型, "Shell32.dll", "SHFileOperationA"
    .参数 文件参数, SHFILEOPSTRUCT

.DLL命令 _查找子窗口, 整数型, "user32", "FindWindowExA", , 本子程序采用【精易编程助手】生成
    .参数 参数_父窗口句柄, 整数型
    .参数 参数_子窗口, 整数型
    .参数 参数_类名, 文本型
    .参数 参数_标题, 文本型

.DLL命令 GetWindowTextA, 整数型, , "GetWindowTextA", 公开, 窗口_取标题
    .参数 hwnd, 整数型
    .参数 标题名, 文本型, 传址
    .参数 长度, 整数型

.DLL命令 GetWindow, 整数型, "user32", "GetWindow", , 获得一个窗口的句柄，该窗口与某源窗口有特定的关系
    .参数 hwnd, 整数型, , 源窗口
    .参数 wCmd, 整数型, , 指定结果窗口与源窗口的关系

.DLL命令 SetParent, 整数型, "user32", "SetParent", 公开, 窗口_置父   指定一个窗口的新父(返回  前一个父窗口的句柄)
    .参数 子窗口句柄, 整数型
    .参数 父窗口句柄, 整数型

.DLL命令 GetClientRect, 逻辑型, "user32", "GetClientRect", 公开, ,窗口_取控件大小
    .参数 hwnd, 整数型
    .参数 矩形区域, 精易_矩形, 传址

.DLL命令 IsWindow, 整数型, "user32", "IsWindow", 公开, 窗口_句柄是否有效  判断一个窗口句柄是否有效(非零表示成功，零表示失败)
    .参数 hwnd, 整数型

.DLL命令 PostMessageA, , , "PostMessageA", 公开, 投递消息
    .参数 hwnd, 整数型
    .参数 消息号, 整数型
    .参数 消息参数1, 整数型
    .参数 消息参数2, 整数型

.DLL命令 SetForegroundWindow, 逻辑型, "user32", "SetForegroundWindow", 公开, 窗口_置焦点 将窗口设为系统的前台窗口。这个函数可用于改变用户目前正在操作的应用程序
    .参数 句柄, 整数型

.DLL命令 keybd_event, 整数型, "user32.dll", "keybd_event", 公开, 键盘模拟
    .参数 键代码, 整数型, , bVk,欲模拟的虚拟键码
    .参数 键扫描码, 整数型, , bScan,键的OEM扫描码
    .参数 标志值, 整数型, , dwFlags,标志常数
    .参数 特定值, 整数型, , dwExtraInfo,通常不用的一个值

.DLL命令 CreateWaitableTimerA, 整数型, , "CreateWaitableTimerA", 公开, 程序延时
    .参数 lpTimerAttributes, 整数型
    .参数 bManualReset, 逻辑型
    .参数 lpTimerName, 整数型

.DLL命令 MsgWaitForMultipleObjects, 整数型, , "MsgWaitForMultipleObjects", 公开
    .参数 nCount, 整数型
    .参数 pHandles, 整数型, 传址
    .参数 fWaitAll, 逻辑型
    .参数 dwMilliseconds, 整数型
    .参数 dwWakeMask, 整数型

.DLL命令 SetWaitableTimer, 整数型, , "SetWaitableTimer", 公开
    .参数 hTimer, 整数型
    .参数 pDueTime, 程序延时
    .参数 lPeriod, 整数型
    .参数 pfnCompletionRoutine, 整数型
    .参数 lpArgToCompletionRoutine, 整数型
    .参数 fResume, 逻辑型

.DLL命令 CloseHandle, 逻辑型, "kernel32", "CloseHandle", 公开, 关闭一个内核对象。其中包括文件、文件映射、进程、线程、安全和同步对象等。涉及文件处理时，这个函数通常与vb的close命令相似。应尽可能的使用close，因为它支持vb的差错控制。注意这个函数使用的文件句柄与vb的文件编号是完全不同的  非零表示成功，零表示失败。会设置GetLastError
    .参数 对象句柄, 整数型, , 欲关闭的一个对象的句柄;

.DLL命令 SendMessage, 整数型, "user32", "SendMessageA"
    .参数 hwnd, 整数型
    .参数 wMsg, 整数型
    .参数 wParam, 整数型
    .参数 lParam, 整数型

.DLL命令 SendMessage1, 整数型, "user32", "SendMessageA"
    .参数 hwnd, 整数型
    .参数 wMsg, 整数型
    .参数 wParam, 整数型
    .参数 lParam, TVITEMEX, 传址

.DLL命令 GetParent, 整数型, , "GetParent", 公开, 取父句柄
    .参数 hwnd, 整数型

.DLL命令 MessageBoxA, 整数型, "user32.dll", "MessageBoxA", 公开, 显示和操作一个消息框。该消息框包含一个应用程序定义的消息和标题，加上预定义的图标和按钮的任意组合。 wLanguageId参数指定为预定义的按钮使用的语言资源集。
    .参数 hwnd, 整数型, , 要创建的消息框的所有者窗口的句柄。如果这个参数为NULL，则消息框没有所有者窗口
    .参数 neirong, 文本型
    .参数 title, 文本型
    .参数 nom, 整数型

.DLL命令 GetCurrentProcessId, 整数型, "kernel32.dll", "GetCurrentProcessId", 公开, 取自进程ID

.DLL命令 EnumWindows, 逻辑型, , "EnumWindows", 公开, 窗口_枚举
    .参数 枚举过程, 子程序指针
    .参数 参数, 整数型

.DLL命令 IsWindowVisible, 逻辑型, , "IsWindowVisible", 公开, 窗口_是否可见  判断窗口是否可见  返回真为可见，否则为不可见
    .参数 hwnd, 整数型, , 要测试的那个窗口的句柄

.DLL命令 GetWindowThreadProcessId, 整数型, "user32.dll", "GetWindowThreadProcessId", 公开, 线程_取ID
    .参数 hwnd, 整数型, , 指定窗口句柄
    .参数 进程标识符, 整数型, 传址, 用于装载进程ID的变量

.DLL命令 EnumDisplaySettingsA, 整数型, , "EnumDisplaySettingsA", 公开, _系统置分辨率1
    .参数 lpszDeviceName, 整数型
    .参数 iModeNum, 整数型
    .参数 lpDevMode, 短整数型, 传址

.DLL命令 GetWindowRect, 逻辑型, , "GetWindowRect", 公开, 窗口_取位置和大小   获得整个窗口的范围矩形，窗口的边框、标题栏、滚动条及菜单等都在这个矩形内
    .参数 句柄, 整数型, , Long，想获得范围矩形的那个窗口的句柄
    .参数 大小, 精易_矩形, 传址, RECT，屏幕坐标中随同窗口装载的矩形(左边,顶边,右边,底边)

.DLL命令 MoveWindow, 逻辑型, "user32.dll", "MoveWindow", 公开, 窗口_置位置和大小
    .参数 hwnd, 整数型, , hwnd，欲移动窗口的句柄
    .参数 左侧位置, 整数型, , x，窗口新的左侧位置
    .参数 顶部位置, 整数型, , y，窗口新的顶部位置
    .参数 新宽度, 整数型, , nWidth，窗口的新宽度
    .参数 新高度, 整数型, , nHeight，窗口的高宽度
    .参数 是否重画, 逻辑型, , bRepaint，如窗口此时应重画，则设为TRUE（非零）

.DLL命令 取内存变量地址_TVITEMEX, 整数型, "kernel32.dll", "lstrcpyn"
    .参数 目标, TVITEMEX, 传址
    .参数 源, TVITEMEX, 传址
    .参数 长度, 整数型

.DLL命令 SHChangeNotify, , "shell32", "SHChangeNotify", 公开, 刷新关联图标
    .参数 SHCNE_ASSOCCHANGED, 整数型, , 134217728
    .参数 SHCNF_IDLIST, 整数型, , 0
    .参数 nil, 整数型, , 0
    .参数 nil, 整数型, , 0

.DLL命令 CryptAcquireContextA, 整数型, "advapi32.dll", "CryptAcquireContextA", 公开
    .参数 phProv, 整数型, 传址
    .参数 pszContainer, 文本型
    .参数 pszProvider, 文本型
    .参数 dwProvType, 整数型
    .参数 dwFlags, 整数型

.DLL命令 CryptCreateHash, 整数型, "advapi32.dll", "CryptCreateHash", 公开
    .参数 hProv, 整数型
    .参数 Algid, 整数型
    .参数 hKey, 整数型
    .参数 dwFlags, 整数型
    .参数 phHash, 整数型, 传址

.DLL命令 CryptReleaseContext, 整数型, "advapi32.dll", "CryptReleaseContext", 公开
    .参数 hProv, 整数型, 传址
    .参数 dwFlags, 整数型, 传址

.DLL命令 CryptDestroyHash, 整数型, "advapi32.dll", "CryptDestroyHash", 公开
    .参数 hHash, 整数型, 传址

.DLL命令 CryptHashData, 整数型, "advapi32.dll", "CryptHashData", 公开
    .参数 hHash, 整数型
    .参数 pbData, 字节集, 传址
    .参数 dwDataLen, 整数型
    .参数 dwFlags, 整数型

.DLL命令 CryptGetHashParam, 整数型, "advapi32.dll", "CryptGetHashParam", 公开
    .参数 hHash, 整数型
    .参数 dwParam, 整数型
    .参数 pByte, 字节集
    .参数 pdwDataLen, 整数型, 传址
    .参数 dwFlags, 整数型

.DLL命令 GetTempPathA, 整数型, "kernel32.dll", "GetTempPathA"
    .参数 nBufferLength, 整数型
    .参数 lpBuffer, 文本型

.DLL命令 InflateRect, 整数型, "user32", "InflateRect", 公开, 这个函数用于增大或减小一个矩形的大小。x加在右侧区域，并从左侧区域减去；如x为正，则能增大矩形的宽度；如x为负，则能减小它。y对顶部与底部区域产生的影响是是类似的　非零表示成功，零表示失败。会设置GetLastError
    .参数 lpRect, 精易_矩形, , RECT，欲修改的矩形
    .参数 x, 整数型, , 用这个数字修改宽度
    .参数 y, 整数型, , 用这个数字修改高度;  

.DLL命令 SendMessage_Rect, 整数型, "user32", "SendMessageA", 公开
    .参数 hwnd, 整数型
    .参数 wMsg, 整数型
    .参数 wParam, 整数型
    .参数 lParam, 精易_坐标

.DLL命令 SendMessageA_整数, 整数型, "user32", "SendMessageA", 公开
    .参数 hwnd, 整数型
    .参数 消息号, 整数型, , 消息的标识符
    .参数 参数一, 整数型, , 具体取决于消息
    .参数 参数二, 整数型, , 具体取决于消息

.DLL命令 取内存变量地址_TVINSERTSTRUCT, 整数型, "kernel32.dll", "lstrcpyn"
    .参数 目标, TVINSERTSTRUCT, 传址
    .参数 源, TVINSERTSTRUCT, 传址
    .参数 长度, 整数型

.DLL命令 RtlMoveMemory_字节集_1, , , "RtlMoveMemory"
    .参数 Destination, 字节集
    .参数 Source, 整数型
    .参数 Length, 整数型

.DLL命令 RtlMoveMemory_字节集_2, , , "RtlMoveMemory"
    .参数 Destination, 整数型
    .参数 Source, 字节集
    .参数 Length, 整数型

.DLL命令 LocalReAlloc, 整数型, "Kernel32", "LocalReAlloc", , , 改变大小或指定的本地内存中的对象的属性 ，大小可以增加或减少，如果函数调用成功，则返回值是一个重新分配的内存对象的句柄。。
    .参数 hMem, 整数型, , 被重新分配到本地内存对象的句柄。
    .参数 uBytes, 整数型, , 新的内存块的大小（以字节为单位），uFlags指定LMEM_MODIFY，则忽略此参数。
    .参数 uFlags, 整数型, , 如果指定为LMEM_MODIFY修改内存中的对象的属性（uBytes参数被忽略）。否则，函数重新分配内存对象。可以选择性地结合LMEM_MODIFY以下值：LMEM_MOVEABLE(2)\LMEM_ZEROINIT(64)

.DLL命令 复制内存_获取大小信息, 整数型, "kernel32.dll", "RtlMoveMemory", , 将指定的内存移动到指定的位置，如果成功就返回1,则返回NULL。
    .参数 大小信息指针, 大小信息, , 指向目标地址的指针
    .参数 源地址指针, 整数型, , 指向源地址的指针
    .参数 尺寸, 整数型, , 源地址大小

.DLL命令 复制内存_设置大小信息, 整数型, "kernel32.dll", "RtlMoveMemory", , 将指定的内存移动到指定的位置，如果成功就返回1,则返回NULL。
    .参数 源地址指针, 整数型, , 指向目标地址的指针
    .参数 大小信息指针, 大小信息, , 指向源地址的指针
    .参数 尺寸, 整数型, , 源地址大小

.DLL命令 GetAsyncKeyState, 短整数型, "user32.dll", "GetAsyncKeyState", , 确定在调用函数时键是向上还是向下，以及键是在上一次调用GetAsyncKeyState 后按下的。
    .参数 vKey, 整数型, , 您可以使用左区分和右区分常量来指定某些键。有关详细信息，请参阅备注部分。
