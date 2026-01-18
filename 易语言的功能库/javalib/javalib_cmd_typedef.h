#pragma once

#define __JAVALIB_NAME(_index, _name) __LIB2_FNE_NAME_LEFT(__E_FNENAME)##_##_name##_##_index##_

// 传递函数名和索引, 拼接成 定义库名_名字_序号_定义库名, 比如 javalib_test_0_javalib
#define JAVALIB_NAME(_index, _name) __LIB2_FNE_NAME_LEFT(__JAVALIB_NAME(_index, _name))__LIB2_FNE_NAME_LEFT(__E_FNENAME)

// 传递函数名和索引, 拼接成 "定义库名_名字_序号_定义库名", 比如 "javalib_test_0_javalib"
#define JAVALIB_NAME_STR(_index, _name) ______E_FNENAME(__JAVALIB_NAME(_index, _name))

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
#define JAVALIB_DEF(_MAKE) \
    _MAKE(  0, "测试函数", TestFunction, "----", -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_javalib_global_var + 0)\
    _MAKE(  1, "创建", Create, "创建JAVA虚拟机，并同时设置“用户类路径(classpath)”和“本地库路径(librarypath)”。如果不需要特别指定“用户类路径”或“本地库路径”，不必显式调用本方法，“Java虚拟机”会在必要的时机（进入对象作用域时）自动创建。成功返回“真”，失败返回“假”。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_javalib_global_var + 2)\
    _MAKE(  2, "销毁", Destroy, "销毁已创建的JAVA虚拟机。不必显式调用本方法，“Java虚拟机”会在必要的时机（退出对象作用域时）自动销毁。", -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED | CT_IS_OBJ_FREE_CMD, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_javalib_global_var + 0)\
    _MAKE(  3, "连接", AttachCurrentThread, "将当前线程连接到Java虚拟机。试图连接已经连接的线程将不执行任何操作。本地线程不能同时连接到两个Java虚拟机上。成功返回“真”，失败返回“假”。", -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_javalib_global_var + 0)\
    _MAKE(  4, "断开", DetachCurrentThread, "断开当前线程与Java虚拟机之间的连接，释放该线程占用的所有Java监视程序，通知所有等待该线程终止的Java线程。主线程（即创建Java虚拟机的线程）不能使用本方法断开与虚拟机之间的连接，而必须调用“销毁()”方法来卸载整个虚拟机。成功返回“真”，失败返回“假”。", -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_javalib_global_var + 0)\
    _MAKE(  5, "取版本", GetVersionText, "取Java本地接口环境的当前版本。返回的文本格式为“A.B”，A、B均为数字，分别表示主、次版本号。", -1, _CMD_OS(__OS_WIN), SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_javalib_global_var + 0)\
    _MAKE(  6, "取次版本", GetMinorVersion, "返回Java本地接口环境的次版本号。", -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, SDT_INT, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_javalib_global_var + 0)\
    _MAKE(  7, "加载类", FindClass, "根据类名称加载指定的Java类。它将搜索由classpath环境变量中指定的目录和jar文件，以及调用“Java虚拟机.创建()”方法时指定的“用户类路径”和“本地库路径”。返回加载后的Java类标志符。如果出错则返回0。", -1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_javalib_global_var + 4)\
    _MAKE(  8, "字节集加载类", DefineClass, "从包含原始类数据(.class文件内容)的字节集中加载类，返回加载后的Java类标志符。如果出错则返回0。JNI不允许重复加载同一个类。", -1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_javalib_global_var + 5)\
    _MAKE(  9, "取父类", GetSuperclass, "取指定类的父类（或超类，SuperClass）。返回父类标志符，失败返回0。", -1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_javalib_global_var + 8)\
    _MAKE( 10, "分配对象", AllocObject, "分配指定Java类的新对象而不调用该类的任何构造函数。返回分配后的对象标志符，失败返回0。", -1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_javalib_global_var + 9)\
    _MAKE( 11, "创建对象", NewObject, "创建Java对象。返回创建后的对象标志符，失败返回0。", -1, _CMD_OS(__OS_WIN) | CT_ALLOW_APPEND_NEW_ARG, SDT_INT, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_javalib_global_var + 10)\
    _MAKE( 12, "取对象类", GetObjectClass, "返回“对象所属类”的类标志符，失败返回0。", -1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_javalib_global_var + 13)\
    _MAKE( 13, "是否为类实例", IsInstanceOf, "判断指定对象是否为指定类的实例。是则返回“真”，否则返回“假”。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_javalib_global_var + 14)\
    _MAKE( 14, "是否相同", IsSameObject, "判断两个变量是否引用同一Java对象。是则返回“真”，否则返回“假”。注意：如果两个参数标志符有其一为0，必将返回“假”（这一点与JNI不同）。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_javalib_global_var + 16)\
    _MAKE( 15, "可否强制转换", IsAssignableFrom, "判断参数一（“将被转换的类”）的对象是否可安全地强制转换为参数二（“将被转换到的类”）。以下情况下将返回“真”：第一及第二个类参数引用同一个 Java 类；第一个类是第二个类的子类；第二个类是第一个类的某个接口。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_javalib_global_var + 18)\
    _MAKE( 16, "创建全局引用", NewGlobalRef, "创建指定对象的新全局引用。返回创建后的全局引用对象标志符，失败返回0。全局引用需要通过调用“销毁全局引用()”方法来销毁。", -1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_javalib_global_var + 20)\
    _MAKE( 17, "销毁全局引用", DeleteGlobalRef, "销毁由参数所指定的全局引用对象。成功返回“真”，失败返回“假”。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_javalib_global_var + 21)\
    _MAKE( 18, "销毁局部引用", DeleteLocalRef, "销毁由参数所指定的局部引用对象。成功返回“真”，失败返回“假”。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_javalib_global_var + 22)\
    _MAKE( 19, "取成员标志符", GetFieldID, "返回类的实例成员（非静态成员）的成员标志符，该成员由其名称及签名指定。失败返回0。本方法可自动将未初始化的类初始化。本方法不能用于获取数组的长度，应调用“取数组长度()”方法。调用“取成员()”“置成员()”系列方法时都将用到本方法返回的成员标志符。", -1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_javalib_global_var + 23)\
    _MAKE( 20, "取成员", GetField, "取对象成员的值，结果存到第三个参数“结果”中。成功返回“真”，失败返回“假”。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_javalib_global_var + 26)\
    _MAKE( 21, "置成员", SetField, "设置对象成员的值。成功返回“真”，失败返回“假”。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_javalib_global_var + 29)\
    _MAKE( 22, "取对象成员", GetObjectField, "取对象成员的值，取出的是一个对象标志符，结果存到第三个参数“结果”中。成功返回“真”，失败返回“假”。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_javalib_global_var + 32)\
    _MAKE( 23, "置对象成员", SetObjectField, "设置对象成员的值。成功返回“真”，失败返回“假”。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_javalib_global_var + 35)\
    _MAKE( 24, "取静态成员标志符", GetStaticFieldID, "返回类的静态成员(static Field)的成员标志符，该成员由其名称及签名指定。失败返回0。本方法可自动将未初始化的类初始化。调用“取静态成员()”“置静态成员()”系列方法时都将用到本方法返回的静态成员标志符。", -1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_javalib_global_var + 38)\
    _MAKE( 25, "取静态成员", GetStaticField, "取对象静态成员的值，取出的是一个对象标志符，结果存到第三个参数“结果”中。成功返回“真”，失败返回“假”。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_javalib_global_var + 41)\
    _MAKE( 26, "置静态成员", SetStaticField, "设置对象静态成员的值。成功返回“真”，失败返回“假”。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_javalib_global_var + 44)\
    _MAKE( 27, "取静态对象成员", GetStaticObjectField, "取对象静态成员的值，结果存到第三个参数“结果”中。成功返回“真”，失败返回“假”。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_javalib_global_var + 47)\
    _MAKE( 28, "置静态对象成员", SetStaticObjectField, "设置对象静态成员的值。成功返回“真”，失败返回“假”。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_javalib_global_var + 50)\
    _MAKE( 29, "取方法标志符", GetMethodID, "返回类或接口实例方法（非静态方法）的方法标志符，该方法由其名称及签名指定。失败返回0。方法可在参数“类标志符”所指定的类的超类中定义。本方法可自动将未初始化的类初始化。要获得构造函数的方法标志符，应将<init>作为方法名，同时将void (V)作为返回类型。调用“[非虚拟][对象/空]方法()”系列方法时都将用到本方法返回的方法标志符。", -1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_javalib_global_var + 53)\
    _MAKE( 30, "方法", CallMethod, "调用对象的具有基本数据类型返回值方法。调用成功返回“真”，失败返回“假”。", -1, _CMD_OS(__OS_WIN) | CT_ALLOW_APPEND_NEW_ARG, SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 4, g_argumentInfo_javalib_global_var + 56)\
    _MAKE( 31, "对象方法", CallObjectMethod, "调用对象的具有对象型返回值的方法。调用成功返回“真”，失败返回“假”。", -1, _CMD_OS(__OS_WIN) | CT_ALLOW_APPEND_NEW_ARG, SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 4, g_argumentInfo_javalib_global_var + 60)\
    _MAKE( 32, "空方法", CallVoidMethod, "调用对象的没有返回值的方法。调用成功返回“真”，失败返回“假”。", -1, _CMD_OS(__OS_WIN) | CT_ALLOW_APPEND_NEW_ARG, SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_javalib_global_var + 64)\
    _MAKE( 33, "非虚拟方法", CallNonvirtualMethod, "根据指定的类和方法标志符调用某对象的具有基本数据类型返回值的实例（非静态）方法。调用成功返回“真”，失败返回“假”。", -1, _CMD_OS(__OS_WIN) | CT_ALLOW_APPEND_NEW_ARG, SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 5, g_argumentInfo_javalib_global_var + 67)\
    _MAKE( 34, "非虚拟对象方法", CallNonvirtualObjectMethod, "根据指定的类和方法标志符调用某对象的具有对象类型返回值的实例（非静态）方法。调用成功返回“真”，失败返回“假”。", -1, _CMD_OS(__OS_WIN) | CT_ALLOW_APPEND_NEW_ARG, SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 5, g_argumentInfo_javalib_global_var + 72)\
    _MAKE( 35, "非虚拟空方法", CallNonvirtualVoidMethod, "根据指定的类和方法标志符调用某对象的没有返回值的实例（非静态）方法。调用成功返回“真”，失败返回“假”。", -1, _CMD_OS(__OS_WIN) | CT_ALLOW_APPEND_NEW_ARG, SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 4, g_argumentInfo_javalib_global_var + 77)\
    _MAKE( 36, "取静态方法标志符", GetStaticMethodID, "取指定类的静态方法(static Method)的方法标志符，该方法由其名称和签名指定。失败返回0。本方法可自动将未初始化的类初始化。调用“静态[对象/空]方法()”系列方法时都将用到本方法返回的静态方法标志符。", -1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_javalib_global_var + 81)\
    _MAKE( 37, "静态方法", CallStaticMethod, "调用类的具有基本数据类型返回值静态方法。调用成功返回“真”，失败返回“假”。", -1, _CMD_OS(__OS_WIN) | CT_ALLOW_APPEND_NEW_ARG, SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 4, g_argumentInfo_javalib_global_var + 84)\
    _MAKE( 38, "静态对象方法", CallStaticObjectMethod, "调用类的具有对象型返回值的静态方法。调用成功返回“真”，失败返回“假”。", -1, _CMD_OS(__OS_WIN) | CT_ALLOW_APPEND_NEW_ARG, SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 4, g_argumentInfo_javalib_global_var + 88)\
    _MAKE( 39, "静态空方法", CallStaticVoidMethod, "调用类的静态无返回值方法，即static void方法。调用成功返回“真”，失败返回“假”。", -1, _CMD_OS(__OS_WIN) | CT_ALLOW_APPEND_NEW_ARG, SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_javalib_global_var + 92)\
    _MAKE( 40, "创建字符串", NewString, "使用易语言文本构造新的java.lang.String对象。返回创建后的字符串对象标志符，失败返回0。", -1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_javalib_global_var + 95)\
    _MAKE( 41, "取字符串长度", GetStringLength, "返回Java字符串的长度（Unicode字符数），数字、字母、汉字等都被当作一个字符。失败返回0。", -1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_javalib_global_var + 96)\
    _MAKE( 42, "取字符串文本", GetStringChars, "返回与指定Java字符串相对应的易语言文本。", -1, _CMD_OS(__OS_WIN), SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_javalib_global_var + 97)\
    _MAKE( 43, "释放字符串", ReleaseStringChars, "释放由“取字符串文本()”方法所返回的文本。成功返回“真”，失败返回“假”。注：内存释放由JAVA虚拟机负责，用户不必介入，故通常忽略本方法的返回值。", -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_javalib_global_var + 99)\
    _MAKE( 44, "创建UTF字符串", NewStringUTF, "使用易语言文本(UTF-8格式)构造新的java.lang.String对象，返回创建后的字符串对象标志符。失败返回0。", -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, SDT_INT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_javalib_global_var + 101)\
    _MAKE( 45, "取UTF字符串长度", GetStringUTFLength, "返回指定UTF字符串的长度，以字节(byte)为单位。", -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, SDT_INT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_javalib_global_var + 96)\
    _MAKE( 46, "取UTF字符串文本", GetStringUTFChars, "返回与指定Java字符串相对应的易语言文本。", -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_javalib_global_var + 102)\
    _MAKE( 47, "释放UTF字符串", ReleaseStringUTFChars, "释放由“取UTF字符串文本()”方法所返回的文本。成功返回“真”，失败返回“假”。注：内存释放由JAVA虚拟机负责，用户不必介入，故通常忽略本方法的返回值。", -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_javalib_global_var + 104)\
    _MAKE( 48, "取数组长度", GetArrayLength, "返回指定Java数组的长度。", -1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_javalib_global_var + 106)\
    _MAKE( 49, "创建数组", NewArray, "返回创建后的数组标志符。失败返回0。", -1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_javalib_global_var + 107)\
    _MAKE( 50, "取数组成员", GetArrayElement, "取出指定数组中指定成员并存放到本方法的第三个参数中。成功返回“真”，失败返回“假”。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_javalib_global_var + 109)\
    _MAKE( 51, "置数组成员", SetArrayElement, "成功返回“真”，失败返回“假”。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_javalib_global_var + 112)\
    _MAKE( 52, "创建对象数组", NewObjectArray, "创建一个可以容纳Java对象(object)的数组。返回创建后的数组标志符，失败返回0。", -1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_javalib_global_var + 115)\
    _MAKE( 53, "取对象数组成员", GetObjectArrayElement, "取指定“对象数组”中的某一个成员，由参数“成员索引”指定取该数组的哪一个成员。返回该成员的对象标志符，失败返回0。", -1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_javalib_global_var + 118)\
    _MAKE( 54, "置对象数组成员", SetObjectArrayElement, "为Java“对象数组”的指定成员赋值。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_javalib_global_var + 120)\
    _MAKE( 55, "抛出异常", Throw, "向Java虚拟机抛出一个已存在的异常对象。成功返回“真”，失败返回“假”。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_javalib_global_var + 123)\
    _MAKE( 56, "抛出新异常", ThrowNew, "创建一个异常对象，并向Java虚拟机抛出该异常对象。成功返回“真”，失败返回“假”。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_javalib_global_var + 124)\
    _MAKE( 57, "是否有异常", IsExceptionOccurred, "判断当前是否有正被抛出的异常对象。在调用“清除异常()”方法或Java代码处理该异常前，异常将始终保持抛出状态。如果有异常发生，返回“真”；否则返回“假”。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_javalib_global_var + 0)\
    _MAKE( 58, "取异常对象", ExceptionOccurred, "返回当前正被抛出的异常对象。在调用“清除异常()”方法或Java代码处理该异常前，异常将始终保持抛出状态。如果有异常发生，返回该异常对象标志符；如果没有异常发生，返回0。", -1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_javalib_global_var + 0)\
    _MAKE( 59, "输出异常", ExceptionDescribe, "将异常及堆栈的回溯信息文本输出到系统标准错误输出设备（例如stderr）。执行后将清除当前异常对象。成功返回“真”，失败返回“假”。本方法主要用于调试。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_javalib_global_var + 0)\
    _MAKE( 60, "取异常文本", GetExceptionMsg, "返回当前异常对象的描述文本。执行后将清除当前异常对象。", -1, _CMD_OS(__OS_WIN), SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_javalib_global_var + 0)\
    _MAKE( 61, "清除异常", ExceptionClear, "清除当前抛出的所有异常。如果当前无异常，则不做任何操作。成功返回“真”，失败返回“假”。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_javalib_global_var + 0)\
    _MAKE( 62, "致命错误", FatalError, "抛出致命错误并且不希望Java虚拟机进行修复。成功返回“真”，失败返回“假”。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_javalib_global_var + 126)\
    _MAKE( 63, "进入监视", MonitorEnter, "进入与指定对象所引用的基本Java对象相关联的监视程序。成功返回“真”，失败返回“假”。\r\n    每个Java对象都有一个相关联的监视程序。如果当前线程已经拥有与指定对象相关联的监视程序，它将使指示该线程进入监视程序次数的监视程序计数器增1。如果与指定对象相关联的监视程序并非由某个线程所拥有，则当前线程将变为该监视程序的所有者，同时将该监视程序的计数器设置为1。如果另一个线程已拥有与指定对象关联的监视程序，则在监视程序被释放前当前线程将处于等待状态。监视程序被释放后，当前线程将尝试重新获得所有权。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_javalib_global_var + 127)\
    _MAKE( 64, "退出监视", MonitorExit, "当前线程必须是与指定对象所引用的基本Java对象相关联的监视程序的所有者。线程将使指示进入监视程序次数的计数器减1。如果计数器的值变为0，当前线程释放监视程序。成功返回“真”，失败返回“假”。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_javalib_global_var + 127)\
    _MAKE( 65, "确认销毁JVM", DoDestroyJVM, "如果尚未销毁已创建的JAVA虚拟机，则销毁之。", -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED | CT_IS_OBJ_FREE_CMD, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_javalib_global_var + 0)

