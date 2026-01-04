#if !defined(__E_STATIC_LIB)
#include "include_eapi_header.h"

//TODO 静态库需要的部分,会记录所有的函数名到数组里,静态编译的时候需要取回命令名
static ARG_INFO g_argumentInfo_eapi_global_var[] = 
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

    /*000*/ {"指示灯类别", "可以被省略。键盘指示灯类别，为以下常量值之一：0、#接口常量.数字锁定键盘灯； 1、#接口常量.大小写锁定键盘灯； 2、#接口常量.滚动锁定键盘灯", 0, 0, SDT_INT, 0, AS_DEFAULT_VALUE_IS_EMPTY},

    /*001*/ {"虚拟键值1", NULL, 0, 0, _SDT_ALL, 0, NULL},
    /*002*/ {"虚拟键值2", NULL, 0, 0, _SDT_ALL, 0, AS_DEFAULT_VALUE_IS_EMPTY},
    /*003*/ {"虚拟键值3", NULL, 0, 0, _SDT_ALL, 0, AS_DEFAULT_VALUE_IS_EMPTY},

    /*004*/ {"X坐标", NULL, 0, 0, SDT_INT, 0, NULL},
    /*005*/ {"Y坐标", NULL, 0, 0, SDT_INT, 0, NULL},
    /*006*/ {"点击类型", "可以被省略。模拟鼠标点击的类型，可以为以下常量值之一：0、#接口常量.左键单击；1、#接口常量.左键双击；2、#接口常量.右键单击；3、#接口常量.右键双击。", 0, 0, SDT_INT, 0, AS_DEFAULT_VALUE_IS_EMPTY},

    /*007*/ {"硬盘信息", "待取得的硬盘信息", 0, 0, MAKELONG(0x07, 0), 0, AS_RECEIVE_VAR},
    /*008*/ {"硬盘序号", "可以被省略。待取得的硬盘信息的硬盘序号。可以为以下常量之一：0、#接口常量.磁盘0；1、#接口常量.磁盘1；2、#接口常量.磁盘2；3、#接口常量.磁盘3。", 0, 0, SDT_INT, 0, AS_DEFAULT_VALUE_IS_EMPTY},

    /*009*/ {"驱动器类型", "可以被省略。驱动器的类型，可以为以下常量值之一：0、#接口常量.所有驱动器；1、#接口常量.硬盘驱动器；2、#接口常量.光盘驱动器；3、#接口常量.可移动驱动器；4、#接口常量.网络驱动器。", 0, 0, SDT_INT, 0, AS_DEFAULT_VALUE_IS_EMPTY},

    /*010*/ {"进程标识", "要终止进程的进程ID、进程名称或窗口标题。", 0, 0, _SDT_ALL, 0, NULL},

    /*011*/ {"进程标识符", "要终止进程的进程ID、进程名称或窗口标题。置-1则取所有正在使用DLL列表。", 0, 0, SDT_INT, 0, AS_DEFAULT_VALUE_IS_EMPTY},

    /*012*/ {"系统信息", "待取得的系统信息。", 0, 0, MAKELONG(0x04, 0), 0, AS_RECEIVE_VAR},

    /*013*/ {"BIOS信息", "待取得的系统信息。", 0, 0, MAKELONG(0x06, 0), 0, AS_RECEIVE_VAR},

    /*014*/ {"文件名", "要取得版本信息的文件名。", 0, 0, SDT_TEXT, 0, NULL},
    /*015*/ {"文件版本信息", "待取得的文件版本信息。", 0, 0, MAKELONG(0x05, 0), 0, AS_RECEIVE_VAR},

    /*016*/ {"CPU信息", "待取得的CPU信息。", 0, 0, MAKELONG(0x03, 0), 0, AS_RECEIVE_VAR},

    /*017*/ {"内存类型", "可以被省略。要取得的内存类型，可以为以下常量值之一：0、#接口常量.总物理内存；1、#接口常量.可用物理内存；2、#接口常量.总虚拟内存；3、#接口常量.可用虚拟内存；4、#接口常量.总页面文件；5、#接口常量.可用页面文件。", 0, 0, SDT_INT, 0, NULL},

    /*018*/ {"作用的区域或扩展名", "右键菜单在什么位置出现，例如指定:.txt,那么右键.txt文件时才会出现添加的菜单，“ * ”：所有地方 ；“D”：桌面右键；“URL”：文件夹右键；“SAVER”：文件右键；“.*”：指定后缀名右键，按照.*的格式写，不存在则创建（如.txt）,默认所有地方生效", 0, 0, SDT_TEXT, 0, AS_DEFAULT_VALUE_IS_EMPTY},
    /*019*/ {"右键菜单标题", "右键后显示的标题，同时为类名，删除时指定此参数即可", 0, 0, SDT_TEXT, 0, NULL},
    /*020*/ {"点击后执行的命令行", "点击右键菜单触发的命令，如在.txt文件添加\"用易运行\"并绑定命令 run e.exe 文件名 点击后就会指定运行命令行", 0, 0, SDT_TEXT, 0, AS_DEFAULT_VALUE_IS_EMPTY},
    /*021*/ {"快捷按键", "指定右键后载弹种按下某键可以快捷调用此菜单，直接填A-Z中的一个就好", 0, 0, SDT_TEXT, 0, AS_DEFAULT_VALUE_IS_EMPTY},
    /*022*/ {"图标路径", "指定图标路径,.ico图标路径/.exe程序路径，会自动读取图标", 0, 0, SDT_TEXT, 0, AS_DEFAULT_VALUE_IS_EMPTY},
    /*023*/ {"仅shift显示", "是否在只按住SHIFT的情况下显示（只在一级菜单中有效），默认为假", 0, 0, SDT_BOOL, 0, AS_DEFAULT_VALUE_IS_EMPTY},

    /*024*/ {"作用的区域或扩展名", "右键菜单在什么位置出现，例如指定:.txt,那么右键.txt文件时才会出现添加的菜单，“ * ”：所有地方 ；“D”：桌面右键；“URL”：文件夹右键；“SAVER”：文件右键；“.*”：指定后缀名右键，按照.*的格式写，不存在则创建（如.txt）,默认所有地方生效", 0, 0, SDT_TEXT, 0, AS_DEFAULT_VALUE_IS_EMPTY},
    /*025*/ {"右键菜单标题", "右键后显示的标题，同时为类名，删除时指定此参数即可", 0, 0, SDT_TEXT, 0, NULL},

    /*026*/ {"程序类名", "程序类名、置于启动组或者注册表中的名称", 0, 0, SDT_TEXT, 0, NULL},
    /*027*/ {"程序路径", "要设置为自动运行的程序文件名（包含绝对路径）。", 0, 0, SDT_TEXT, 0, NULL},
    /*028*/ {"位置", "可以被省略。为以下常量值之一：0、#接口常量.到注册表；1、#接口常量.到启动组。", 0, 0, SDT_INT, 0, AS_DEFAULT_VALUE_IS_EMPTY},

    /*029*/ {"临时文件类型", "可以被省略。临时文件类型，可以为以下常量值之一：0、#接口常量.系统临时文件；1、#接口常量.IE临时文件；2、#接口常量.回收站；3、#接口常量.存根文件。", 0, 0, SDT_INT, 0, AS_DEFAULT_VALUE_IS_EMPTY},

    /*030*/ {"历史记录类型", "可以被省略。历史记录类型，可以为以下常量值之一：0、#接口常量.浏览器历史地址记录；1、#接口常量.表单自动完成历史记录；2、#接口常量.自动密码历史记录；3、#接口常量.收藏夹；4、#接口常量.自动拨号历史记录；5、#接口常量.运行中的自动匹配历史记录；6、#接口常量.打开文档历史记录；7、#接口常量.网络连接历史记录；8、#接口常量.远程登录历史记录；9、#接口常量.浏览网址历史记录。", 0, 0, SDT_INT, 0, AS_DEFAULT_VALUE_IS_EMPTY},

    /*031*/ {"字体文件名或字体资源", "字体资源文件的文件名。可以是.FON，.FNT，.TTF或.FOT文件。或者字体资源", 0, 0, _SDT_ALL, 0, NULL},

    /*032*/ {"字体名或字体句柄", "字体资源文件的文件名。可以是.FON，.FNT，.TTF或.FOT文件。或者字体资源", 0, 0, _SDT_ALL, 0, NULL},

    /*033*/ {"图片资源或图片路径", "图片路径或资源", 0, 0, _SDT_ALL, 0, NULL},

    /*034*/ {"资源文件名", "要提取图标的资源文件名。扩展名为EXE、DLL、ICL、ICO。", 0, 0, SDT_TEXT, 0, NULL},
    /*035*/ {"是否提取到剪辑板", "0为假其余为真，兼容之前易库,默认为假", 0, 0, SDT_INT, 0, AS_DEFAULT_VALUE_IS_EMPTY},
    /*036*/ {"保存文件名", "如需要提取到文件则填写提取的图标要保存的文件名。扩展名为ICO、BMP。，默认为空", 0, 0, SDT_TEXT, 0, AS_DEFAULT_VALUE_IS_EMPTY},
    /*037*/ {"图标序号", "当资源文件中包含多个图标时，表示要提取图标的序号。", 0, 0, SDT_INT, 0, AS_DEFAULT_VALUE_IS_EMPTY},

    /*038*/ {"按钮文字", "标题", 0, 0, SDT_TEXT, 0, NULL},
    /*039*/ {"图标文件名", "图标路径", 0, 0, SDT_TEXT, 0, NULL},
    /*040*/ {"执行动作", "在IE中按下按钮后执行一个可执行文件或打开网页，就是命令行。", 0, 0, SDT_TEXT, 0, NULL},
    /*041*/ {"按钮热点图标", "按钮处于热点状态显示的图标，默认不变更", 0, 0, SDT_TEXT, 0, AS_DEFAULT_VALUE_IS_EMPTY},
    /*042*/ {"按钮是否被显示", "是否在工具栏显示，默认为真", 0, 0, SDT_BOOL, 0, AS_DEFAULT_VALUE_IS_EMPTY},

    /*043*/ {"按钮文字", NULL, 0, 0, _SDT_ALL, 0, NULL},

    /*044*/ {"文件夹名称", "要创建的程序组名称。", 0, 0, SDT_TEXT, 0, NULL},
    /*045*/ {"特殊目录位置", "程序组的起始位置。可以为以下常量值之一：0、#接口常量.开始菜单；1、#接口常量.开始菜单程序；2、#接口常量.桌面。", 0, 0, SDT_INT, 0, AS_DEFAULT_VALUE_IS_EMPTY},

    /*046*/ {"文件夹名称", "要删除的程序组名称。", 0, 0, SDT_TEXT, 0, NULL},
    /*047*/ {"特殊目录位置", "程序组的起始位置。可以为以下常量值之一：0、#接口常量.开始菜单；1、#接口常量.开始菜单程序；2、#接口常量.桌面。", 0, 0, SDT_INT, 0, AS_DEFAULT_VALUE_IS_EMPTY},

    /*048*/ {"程序组文件夹", "要创建的快捷方式的程序组文件夹名称，非路径。", 0, 0, SDT_TEXT, 0, NULL},
    /*049*/ {"程序项文件夹", "要创建的快捷方式名称", 0, 0, SDT_TEXT, 0, NULL},
    /*050*/ {"快捷方式链接的程序路径", "绑定执行程序文件名的路径。", 0, 0, SDT_TEXT, 0, NULL},
    /*051*/ {"特殊目录位置", "程序组的起始位置。可以为以下常量值之一：0、#接口常量.开始菜单；1、#接口常量.开始菜单程序；2、#接口常量.桌面。", 0, 0, SDT_INT, 0, AS_DEFAULT_VALUE_IS_EMPTY},

    /*052*/ {"程序组文件夹", "要创建的快捷方式的程序组文件夹名称，非路径。", 0, 0, SDT_TEXT, 0, NULL},
    /*053*/ {"程序项文件夹", "要创建的快捷方式名称", 0, 0, SDT_TEXT, 0, NULL},
    /*054*/ {"特殊目录位置", "程序组的起始位置。可以为以下常量值之一：0、#接口常量.开始菜单；1、#接口常量.开始菜单程序；2、#接口常量.桌面。", 0, 0, SDT_INT, 0, AS_DEFAULT_VALUE_IS_EMPTY},

    /*055*/ {"快捷方式文件名", NULL, 0, 0, SDT_TEXT, 0, NULL},
    /*056*/ {"目标", "快捷方式文件名绑定的程序路径", 0, 0, SDT_TEXT, 0, AS_DEFAULT_VALUE_IS_EMPTY | AS_RECEIVE_VAR},
    /*057*/ {"参数", "默认参数", 0, 0, SDT_TEXT, 0, AS_DEFAULT_VALUE_IS_EMPTY | AS_RECEIVE_VAR},
    /*058*/ {"启始位置", "快捷方式的上级目录", 0, 0, SDT_TEXT, 0, AS_DEFAULT_VALUE_IS_EMPTY | AS_RECEIVE_VAR},
    /*059*/ {"图标", "返回的文本格式为：图标所属文件 + , + 图标序号（0为第一个），例如：“c:\\x.exe,1”。", 0, 0, SDT_TEXT, 0, AS_DEFAULT_VALUE_IS_EMPTY | AS_RECEIVE_VAR},
    /*060*/ {"运行方式", "。1：常规窗口；2：最小化；3：最大化", 0, 0, SDT_INT, 0, AS_DEFAULT_VALUE_IS_EMPTY | AS_RECEIVE_VAR},
    /*061*/ {"快捷键", "可以传入文本型或整数型型变量。如果为文本型，返回快捷键文本形式，如“Ctrl + Shift + Alt + A”。如果为整数型，返回一个表示该快捷键的数值（参见http://msdn.microsoft.com/en-us/library/bb774926", 0, 0, _SDT_ALL, 0, AS_DEFAULT_VALUE_IS_EMPTY | AS_RECEIVE_VAR},
    /*062*/ {"备注", "快捷方式的描述", 0, 0, SDT_TEXT, 0, AS_DEFAULT_VALUE_IS_EMPTY | AS_RECEIVE_VAR},

    /*063*/ {"网卡序号", "可以被省略。当本机有多块网卡时，指定要对其操作的网卡序号（从1开始）。", 0, 0, SDT_INT, 0, AS_DEFAULT_VALUE_IS_EMPTY},

    /*064*/ {"IP地址", NULL, 0, 0, SDT_TEXT, 0, NULL},

    /*065*/ {"Email地址", "目标Email地址", 0, 0, SDT_TEXT, 0, NULL},

    /*066*/ {"网络类型", "网络类型名，为空则取所有工作组。", 0, 0, SDT_TEXT, 0, AS_DEFAULT_VALUE_IS_EMPTY},

    /*067*/ {"工作组名称", "工作组名称，为空则取所有计算机。", 0, 0, SDT_TEXT, 0, AS_DEFAULT_VALUE_IS_EMPTY},

    /*068*/ {"端口", "检测的端口。", 0, 0, SDT_SHORT, 0, NULL},
    /*069*/ {"ip地址", "置空则检测本机端口。", 0, 0, SDT_TEXT, 0, AS_DEFAULT_VALUE_IS_EMPTY},

    /*070*/ {"窗口类型", "系统窗口类型，为以下常量值之一：0、#接口常量.控制面板；1、#接口常量.系统属性；2、#接口常量.显示属性；3、#接口常量.辅助功能选项；4、#接口常量.添加打印机向导；5、#接口常量.添加或删除程序；6、#接口常量.日期和时间属性；7、#接口常量.字体文件夹；8、#接口常量.游戏控制器；9、#接口常量.鼠标属性；10、#接口常量.键盘属性；11、#接口常量.声音和音频属性；12、#接口常量.ODBC数据源管理器；13、#接口常量.电源选项属性；14、#接口常量.区域和语言选项；15、#接口常量.互联网属性；16、#接口常量.用户帐号；17、#接口常量.系统信息对话框。", 0, 0, SDT_INT, 0, NULL},
    /*071*/ {"属性页序号", "有多个属性页时，表示要打开属性页的序号（从0开始）。", 0, 0, SDT_TEXT, 0, AS_DEFAULT_VALUE_IS_EMPTY},

    /*072*/ {"网址url或文件路径", NULL, 0, 0, SDT_TEXT, 0, AS_DEFAULT_VALUE_IS_EMPTY},

    /*073*/ {"文件名", "墙纸文件名（bmp格式）。将文件名置为空可以取消墙纸。", 0, 0, SDT_TEXT, 0, NULL},
    /*074*/ {"样式", "墙纸样式，可以为以下常量值之一：0、#接口常量.居中；1、#接口常量.平铺；2、#接口常量.拉伸。。", 0, 0, SDT_INT, 0, AS_DEFAULT_VALUE_IS_EMPTY},

    /*075*/ {"窗口句柄", "要设置透明度的窗口句柄。", 0, 0, SDT_INT, 0, NULL},
    /*076*/ {"透明度", "墙纸样式，可以为以下常量值之一：0、#接口常量.居中；1、#接口常量.平铺；2、#接口常量.拉伸。。", 0, 0, SDT_INT, 0, AS_DEFAULT_VALUE_IS_EMPTY},
    /*077*/ {"透明色", "(#颜色)指定某颜色为完全透明(完全透明处不属于该窗口) 可空为不指定.", 0, 0, SDT_INT, 0, AS_DEFAULT_VALUE_IS_EMPTY},
    /*078*/ {"鼠标穿透", "设置鼠标穿透，默认为假", 0, 0, SDT_BOOL, 0, AS_DEFAULT_VALUE_IS_EMPTY},

    /*079*/ {"宽度", "屏幕显示区域的宽度，单位为像素点。", 0, 0, SDT_INT, 0, NULL},
    /*080*/ {"高度", "屏幕显示区域的高度，单位为像素点。", 0, 0, SDT_INT, 0, NULL},
    /*081*/ {"位数", "每像素的位数。", 0, 0, SDT_INT, 0, AS_DEFAULT_VALUE_IS_EMPTY},
    /*082*/ {"刷新频率", "屏幕的刷新频率。", 0, 0, SDT_INT, 0, AS_DEFAULT_VALUE_IS_EMPTY},

    /*083*/ {"源单位值", "屏幕显示区域的宽度，单位为像素点。", 0, 0, SDT_DOUBLE, 0, NULL},
    /*084*/ {"源单位", "单位类型，为以下常量值之一：0、#接口常量.像素；1、#接口常量.英寸；2、#接口常量.毫米；3、#接口常量.厘米。", 0, 0, SDT_INT, 0, AS_DEFAULT_VALUE_IS_EMPTY},
    /*085*/ {"目的单位", "单位类型，为以下常量值之一：0、#接口常量.像素；1、#接口常量.英寸；2、#接口常量.毫米；3、#接口常量.厘米。", 0, 0, SDT_INT, 0, AS_DEFAULT_VALUE_IS_EMPTY},

    /*086*/ {"R", "要取得的R值", 0, 0, SDT_INT, 0, AS_RECEIVE_VAR},
    /*087*/ {"G", "要取得的G值", 0, 0, SDT_INT, 0, AS_RECEIVE_VAR},
    /*088*/ {"B", "要取得的B值", 0, 0, SDT_INT, 0, AS_RECEIVE_VAR},

    /*089*/ {"窗口句柄", "要接收所有鼠标输入的窗口的句柄。", 0, 0, SDT_INT, 0, NULL},

    /*090*/ {"屏幕区域", "要截取的屏幕区域，为空则截取整个屏幕。", 0, 0, MAKELONG(0x02, 0), 0, AS_DEFAULT_VALUE_IS_EMPTY},
    /*091*/ {"截取到", "要截取屏幕区域到何处，可以为以下常量值之一：0、#接口常量.到文件； 1、#接口常量.到剪贴板； 2、#接口常量.到字节集。", 0, 0, SDT_INT, 0, AS_DEFAULT_VALUE_IS_EMPTY},
    /*092*/ {"屏幕区域", NULL, 0, 0, SDT_TEXT, 0, AS_DEFAULT_VALUE_IS_EMPTY},

    /*093*/ {"网络资源路径", "要映射到本地的网络资源路径。", 0, 0, SDT_TEXT, 0, NULL},
    /*094*/ {"驱动器", "映射到本地的驱动器名。", 0, 0, SDT_TEXT, 0, NULL},

    /*095*/ {"发送目标", "将要接收消息的目标机器，可以为机器名、IP地址、工作组名（具体由目标类型决定）", 0, 0, SDT_TEXT, 0, AS_DEFAULT_VALUE_IS_EMPTY},
    /*096*/ {"消息", "将要发送的消息文本", 0, 0, SDT_TEXT, 0, NULL},
    /*097*/ {"保留参数", NULL, 0, 0, SDT_INT, 0, AS_DEFAULT_VALUE_IS_EMPTY},

    /*098*/ {"计算机", "计算机名称，为空则取所有计算机的共享资源。", 0, 0, SDT_TEXT, 0, AS_DEFAULT_VALUE_IS_EMPTY},
    /*099*/ {"共享资源类型", "共享资源类型，可以为以下常量值之一：0、#接口常量.所有共享资源；1、#接口常量.共享目录或文件；2、#接口常量.共享打印机。", 0, 0, SDT_INT, 0, AS_DEFAULT_VALUE_IS_EMPTY},

    /*100*/ {"驱动器", "映射到本地的驱动器名。", 0, 0, SDT_TEXT, 0, NULL},

    /*101*/ {"程序名", "设置自动运行时的程序名称。", 0, 0, SDT_TEXT, 0, NULL},

    /*102*/ {"格式化字符串", "设置自动运行时的程序名称。", 0, 0, _SDT_ALL, 0, NULL},
    /*103*/ {"格式化字符串", "设置自动运行时的程序名称。", 0, 0, _SDT_ALL, 0, NULL},

    /*104*/ {"打印机名称", "要设置为默认打印机的名称。", 0, 0, SDT_TEXT, 0, NULL},

    /*105*/ {"对话框类型", "要打开的对话框类型。可以为以下两个常量值之一：0、#接口常量.标准打印设置对话框；1.打印机MFC对话框，废弃，对于MFC静态编译会出现问题 2、#接口常量.打印属性对话框。默认值为0", 0, 0, SDT_INT, 0, AS_DEFAULT_VALUE_IS_EMPTY},
    /*106*/ {"打印信息", "待取得的硬盘信息", 0, 0, MAKELONG(0x0B, 0), 0, AS_DEFAULT_VALUE_IS_EMPTY | AS_RECEIVE_VAR},

    /*107*/ {"窗口句柄", "欲取所在窗口的软件窗口句柄，为空则取主屏幕。", 0, 0, SDT_INT, 0, AS_DEFAULT_VALUE_IS_EMPTY},

    /*108*/ {"图片路径或数据", "可以选择图片路径，或者内存图片字节集。", 0, 0, _SDT_ALL, 0, NULL},

    /*109*/ {" 默认类型", " 默认: 0,标准: 1,拒绝 :2,询问 3,感叹 48,特别 64", 0, 0, SDT_INT, 0, AS_DEFAULT_VALUE_IS_EMPTY},

    /*110*/ {"窗口句柄", "欲修改的窗口句柄", 0, 0, SDT_INT, 0, NULL},
    /*111*/ {"圆角宽度", NULL, 0, 0, SDT_INT, 0, AS_DEFAULT_VALUE_IS_EMPTY},
    /*112*/ {"圆角高度", NULL, 0, 0, SDT_INT, 0, AS_DEFAULT_VALUE_IS_EMPTY},

    /*113*/ {"窗口句柄", "欲修改的窗口句柄", 0, 0, SDT_INT, 0, NULL},
    /*114*/ {"是否总在最前", "默认为假", 0, 0, SDT_BOOL, 0, AS_DEFAULT_VALUE_IS_EMPTY},

};

#ifdef _DEBUG    // 这里是为了确认参数序号是否正确, 成员数比最后一个序号大1就是正确
const int dbg_cmd_arg_count__ = sizeof(g_argumentInfo_eapi_global_var) / sizeof(g_argumentInfo_eapi_global_var[0]);
#endif


#define EAPI_DEF_CMDINFO(_index, _szName, _szEgName, _szExplain, _shtCategory, _wState, _dtRetValType, _wReserved, _shtUserLevel, _shtBitmapIndex, _shtBitmapCount, _nArgCount, _pBeginArgInfo) \
    { _szName, ______E_FNENAME(_szEgName), _szExplain, _shtCategory, _wState, _dtRetValType, _wReserved, _shtUserLevel, _shtBitmapIndex, _shtBitmapCount, _nArgCount, _pBeginArgInfo },


// 易语言每个命令的定义, 会把信息显示在支持库列表里, 这里每个成员都是详细的描述一个命令的信息
CMD_INFO g_cmdInfo_eapi_global_var[] = 
{
    EAPI_DEF(EAPI_DEF_CMDINFO)
};

int g_cmdInfo_eapi_global_var_count = sizeof(g_cmdInfo_eapi_global_var) / sizeof(g_cmdInfo_eapi_global_var[0]);

#endif

