#pragma once

#define __BMPOPERATE_NAME(_index, _name) __LIB2_FNE_NAME_LEFT(__E_FNENAME)##_##_name##_##_index##_

// 传递函数名和索引, 拼接成 定义库名_名字_序号_定义库名, 比如 bmpoperate_test_0_bmpoperate
#define BMPOPERATE_NAME(_index, _name) __LIB2_FNE_NAME_LEFT(__BMPOPERATE_NAME(_index, _name))__LIB2_FNE_NAME_LEFT(__E_FNENAME)

// 传递函数名和索引, 拼接成 "定义库名_名字_序号_定义库名", 比如 "bmpoperate_test_0_bmpoperate"
#define BMPOPERATE_NAME_STR(_index, _name) ______E_FNENAME(__BMPOPERATE_NAME(_index, _name))

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
#define BMPOPERATE_DEF(_MAKE) \
    _MAKE(  0, "构造函数", Constructor, "“设备无关位图”类型的构造函数。隐藏", -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED | CT_IS_OBJ_CONSTURCT_CMD, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_bmpoperate_global_var + 0)\
    _MAKE(  1, "析构函数", Desstructor, "“设备无关位图”类型的析构函数。隐藏", -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED | CT_IS_OBJ_FREE_CMD, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_bmpoperate_global_var + 0)\
    _MAKE(  2, "复制构造函数", CopyConstructor, "“设备无关位图”类型的复制构造函数。隐藏", -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED | CT_IS_OBJ_COPY_CMD, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_bmpoperate_global_var + 0)\
    _MAKE(  3, "取错误信息", GetErrorText, "在调用本数据类型的其他命令失败后可以随后立即调用本命令取得失败的原因。如果前一个操作是成功的，返回空文本。", -1, _CMD_OS(__OS_WIN), SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_bmpoperate_global_var + 0)\
    _MAKE(  4, "载入文件", LoadBmpFile, "从bmp文件载入位图数据，成功返回真，失败返回假。如果失败，随后立即调用本对象的“取错误信息”命令将取得一个非空文本，该文本为失败的原因。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_bmpoperate_global_var + 1)\
    _MAKE(  5, "载入数据", LoadBin, "从字节集或资源载入位图数据，成功返回真，失败返回假。如果失败，随后立即调用本对象的“取错误信息”命令将取得一个非空文本，该文本为失败的原因。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_bmpoperate_global_var + 2)\
    _MAKE(  6, "取位图数据", GetBmFileBin, "取得该位图的二进制数据，数据格式为BMP文件格式，可以直接把本数据赋值给组件的“图片”/“底图”等属性或写到一个*.bmp文件。失败返回空字节集。如果失败，随后立即调用本对象的“取错误信息”命令将取得一个非空文本，该文本为失败的原因。", -1, _CMD_OS(__OS_WIN), SDT_BIN, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_bmpoperate_global_var + 0)\
    _MAKE(  7, "取宽度", GetWidth, "取得该位图的实际宽度，失败返回-1。如果失败，随后立即调用本对象的“取错误信息”命令将取得一个非空文本，该文本为失败的原因。", -1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_bmpoperate_global_var + 0)\
    _MAKE(  8, "取高度", GetWidth, "取得该位图的实际高度，如果该位图的颜色点阵为正向（top-down）存储，高度为负值（不常见）。失败返回-1。如果失败，随后立即调用本对象的“取错误信息”命令将取得一个非空文本，该文本为失败的原因。", -1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_bmpoperate_global_var + 0)\
    _MAKE(  9, "取位深度", GetBitCount, "取得该位图中颜色的位深度。失败返回-1。如果失败，随后立即调用本对象的“取错误信息”命令将取得一个非空文本，该文本为失败的原因。有效的返回值及其含义如下：\r\n\t1：  单色；\r\n\t4：  4位色（16色）；\r\n\t8：  8位色（256色）；\r\n\t16： 16位色（65536色）；\r\n\t24： 24位色（16777216色）；\r\n\t32： 32位色（4294967296色）。\r\n\t", -1, _CMD_OS(__OS_WIN), SDT_SHORT, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_bmpoperate_global_var + 0)\
    _MAKE( 10, "创建", Create, "创建一个指定大小、位深度的位图，本对象内原有数据会被自动释放。成功返回真，失败返回假。如果失败，随后立即调用本对象的“取错误信息”命令将取得一个非空文本，该文本为失败的原因。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 4, g_argumentInfo_bmpoperate_global_var + 3)\
    _MAKE( 11, "取颜色表", GetColorTable, "取得该位图使用的颜色表数据（注意，位深度大于等于16的位图不使用颜色表）。成功返回一个包含颜色表数据的数组，失败返回空数组（成员数为0）。如果失败，随后立即调用本对象的“取错误信息”命令将取得一个非空文本，该文本为失败的原因。", -1, _CMD_OS(__OS_WIN) | CT_RETRUN_ARY_TYPE_DATA, SDT_INT, 0, LVL_HIGH, 0, 0, 0, g_argumentInfo_bmpoperate_global_var + 0)\
    _MAKE( 12, "取像素点阵", GetBits, "取得该位图中代表实际像素点阵的数据，该数据的排列格式取决于该位图的位深度和实际宽度，具体请参阅相关资料。失败返回空字节集。如果失败，随后立即调用本对象的“取错误信息”命令将取得一个非空文本，该文本为失败的原因。", -1, _CMD_OS(__OS_WIN), SDT_BIN, 0, LVL_HIGH, 0, 0, 0, g_argumentInfo_bmpoperate_global_var + 0)\
    _MAKE( 13, "取某点颜色", GetPixelColor, "取得该位图中某点的实际颜色值，以位图的左上角为坐标原点。注意，对位深度为16的位图，取得的颜色可能有少许误差，这是由16位位图的特殊内部结构决定的。如果指定的像素点坐标越界，将导致调用失败，返回-1。如果失败，随后立即调用本对象的“取错误信息”命令将取得一个非空文本，该文本为失败的原因。", -1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_bmpoperate_global_var + 7)\
    _MAKE( 14, "取某点颜色索引", GetPixelColorIndex, "取得该位图中某点的颜色在颜色表中的索引，以位图的左上角为坐标原点。失败返回-1。对使用颜色表的位图调用本命令比调用“取某点颜色”执行速度更快。对不使用颜色表的位图调用该方法将导致调用失败。如果失败，随后立即调用本对象的“取错误信息”命令将取得一个非空文本，该文本为失败的原因。", -1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_HIGH, 0, 0, 2, g_argumentInfo_bmpoperate_global_var + 9)\
    _MAKE( 15, "是否使用颜色表", IsHasColorTable, "位深度小于16的位图使用颜色表，否则不使用。调用本命令后可能产生三种情况：\r\n\t1. 位图尚未创建/载入（即位图为空）。返回假，且设置“错误信息”为非空文本，随后调用“取错误信息”将取回该错误说明信息。\r\n\t2. 位图已经创建，该位图使用颜色表。返回真。\r\n\t3. 位图已经创建，该位图不使用颜色表。返回假。如果随后调用“取错误信息”将返回空文本。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_HIGH, 0, 0, 0, g_argumentInfo_bmpoperate_global_var + 0)\
    _MAKE( 16, "是否为空", IsEmpty, "如果该位图尚功创建或载入（即为空），返回真，否则返回假", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_bmpoperate_global_var + 0)\
    _MAKE( 17, "置某点颜色", SetPixelColor, "参看“取某点颜色”。成功返回真，否则返回假。注意，如果参数3所指定的颜色值不被当前位图格式所支持，系统会自动选择最接近的有效颜色", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_bmpoperate_global_var + 11)\
    _MAKE( 18, "置某点颜色索引", SetPixelColorIndex, "参看“取某点颜色索引”。成功返回真，否则返回假。如果该位图不使用颜色表或参数3指定的颜色索引超出了该位图颜色表的最大索引，本命令调用失败。如果失败，随后立即调用本对象的“取错误信息”命令将取得一个非空文本，该文本为失败的原因。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_HIGH, 0, 0, 3, g_argumentInfo_bmpoperate_global_var + 14)\
    _MAKE( 19, "置颜色表", SetColorTable, "参看“取颜色表”。成功返回真，否则返回假。如果该位图不使用颜色表或指定的颜色表格式错误，本命令调用失败。如果失败，随后立即调用本对象的“取错误信息”命令将取得一个非空文本，该文本为失败的原因。\r\n    注意，操作位图的颜色表需要对位图格式有一定的了解，错误的操作可能会破坏位图数据，请参阅相关资料", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_HIGH, 0, 0, 1, g_argumentInfo_bmpoperate_global_var + 17)\
    _MAKE( 20, "置像素点阵", SetBits, "参看“取像素点阵”。成功返回真，否则返回假。如果指定数据的格式错误，本命令调用失败。如果失败，随后立即调用本对象的“取错误信息”命令将取得一个非空文本，该文本为失败的原因。\r\n    注意，操作位图的像素点阵需要对位图格式有一定的了解，错误的操作可能会破坏位图数据，请参阅相关资料", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_HIGH, 0, 0, 1, g_argumentInfo_bmpoperate_global_var + 18)\
    _MAKE( 21, "转换位深度", SetBitCount, "转换本位图对象的颜色位深度，并返回转换后的对象。失败返回空位图对象。如果失败，随后立即调用本对象的“取错误信息”命令将取得一个非空文本，该文本为失败的原因。", -1, _CMD_OS(__OS_WIN), MAKELONG(0x01, 0), 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_bmpoperate_global_var + 19)\
    _MAKE( 22, "复制到", CopyTo, "将本位图的某一矩形区域复制到目的位图，成功返回真，失败返回假。如果失败，随后立即调用本对象的“取错误信息”命令将取得一个非空文本，该文本为失败的原因。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 8, g_argumentInfo_bmpoperate_global_var + 20)\
    _MAKE( 23, "取指针", GetPointers, "取得该位图数据中关键段的指针，成功返回真，否则返回假。如果失败，随后立即调用本对象的“取错误信息”命令将取得一个非空文本，该文本为失败的原因。\r\n    本命令适用于一些特殊的需要直接操作内存指针场合，一般用户不推荐使用！！如果需要修改这些指针所指向的数据，请务必谨慎，以免造成程序崩溃等意外结果", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_HIGH, 0, 0, 2, g_argumentInfo_bmpoperate_global_var + 28)\
    _MAKE( 24, "旋转90度", Rotate90, "将该位图顺时针或逆时针旋转90度，并返回旋转后的位图，失败返回空位图。如果失败，随后立即调用本对象的“取错误信息”命令将取得一个非空文本，该文本为失败的原因。", -1, _CMD_OS(__OS_WIN), MAKELONG(0x01, 0), 0, LVL_HIGH, 0, 0, 1, g_argumentInfo_bmpoperate_global_var + 30)\
    _MAKE( 25, "镜像", Mirror, "将该位图做水平/垂直镜像，并返回镜像后的新位图。失败返回空位图。如果失败，随后立即调用本对象的“取错误信息”命令将取得一个非空文本，该文本为失败的原因。", -1, _CMD_OS(__OS_WIN), MAKELONG(0x01, 0), 0, LVL_HIGH, 0, 0, 2, g_argumentInfo_bmpoperate_global_var + 31)\
    _MAKE( 26, "创建兼容位图", CreateCompitableBitmap, "根据指定窗口的设备环境（DC）创建出一个兼容位图（设备相关位图），并返回其位图句柄（HBITMAP），失败返回0。如果失败，随后立即调用本对象的“取错误信息”命令将取得一个非空文本，该文本为失败的原因。\r\n    本命令通常用于和API交互，一般用户不推荐使用。注意：该命令成功创建的位图句柄在不再需要时☆必须☆调用 DeleteObject API函数删除之！！否则可能造成GDI资源泄露，产生意外错误。\r\n    该API在易语言中的声明如下：\r\n\t.DLL命令 删除对象, 逻辑型, , \"DeleteObject\"\r\n\t.参数 GDI对象句柄, 整数型\r\n    直接把以上两行复制粘贴到易语言DLL命令区即可使用该API", -1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_HIGH, 0, 0, 1, g_argumentInfo_bmpoperate_global_var + 33)\
    _MAKE( 27, "从句柄创建", FromHandle, "从一个位图句柄（HBITMAP）创建出设备无关位图，该位图原有数据会被自动释放。成功返回真，失败返回假。如果失败，随后立即调用本对象的“取错误信息”命令将取得一个非空文本，该文本为失败的原因。注意：本方法创建出来的位图的颜色质量可能会受到当前屏幕显示模式的影响", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_HIGH, 0, 0, 1, g_argumentInfo_bmpoperate_global_var + 34)\
    _MAKE( 28, "旋转", Rotate, "把一个位图旋转一个角度，并返回旋转后的位图。如果失败，随后立即调用本对象的“取错误信息”命令将取得一个非空文本，该文本为失败的原因。注意：对一个位图进行旋转后会有少许失真。如果本旋转算法的精度无法满足要求，请根据实际情况用“取某点颜色/索引”、“置某点颜色/索引”结合相应的的旋转算法进行操作", -1, _CMD_OS(__OS_WIN), MAKELONG(0x01, 0), 0, LVL_HIGH, 0, 0, 2, g_argumentInfo_bmpoperate_global_var + 35)\
    _MAKE( 29, "取横向分辨率", GetHorzResolution, "取出本位图的横向分辨率。分辨率一般在打印时用到。单位为“像素/米”", -1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_HIGH, 0, 0, 0, g_argumentInfo_bmpoperate_global_var + 0)\
    _MAKE( 30, "取纵向分辨率", GetVertResolution, "取出本位图的纵向分辨率。分辨率一般在打印时用到。单位为“像素/米”", -1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_HIGH, 0, 0, 0, g_argumentInfo_bmpoperate_global_var + 0)\
    _MAKE( 31, "置横向分辨率", GetHorzResolution, "设置出本位图的横向分辨率。分辨率一般在打印时用到。单位为“像素/米”", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_HIGH, 0, 0, 1, g_argumentInfo_bmpoperate_global_var + 37)\
    _MAKE( 32, "置纵向分辨率", GetVertResolution, "设置本位图的纵向分辨率。分辨率一般在打印时用到。单位为“像素/米”", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_HIGH, 0, 0, 1, g_argumentInfo_bmpoperate_global_var + 37)\
    _MAKE( 33, "映射文件", MapFile, "和“载入文件”类似。二者的区别是：“载入文件”方法会一次性把整个文件读入内存中，而本方法则会使用Windows的文件映射来实现“边使用边读取”的效果。注意：使用本方法后，被映射的文件将被本对象打开并占有直到本对象被销毁", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_HIGH, 0, 0, 2, g_argumentInfo_bmpoperate_global_var + 38)\
    _MAKE( 34, "位图_转换位深度", Bitmap_ConvertBitCount, NULL, 1, _CMD_OS(__OS_WIN), SDT_BIN, 0, LVL_HIGH, 0, 0, 3, g_argumentInfo_bmpoperate_global_var + 40)

