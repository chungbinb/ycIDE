#pragma once

#define __EDATASTRUCTURE_NAME(_index, _name) __LIB2_FNE_NAME_LEFT(__E_FNENAME)##_##_name##_##_index##_

// 传递函数名和索引, 拼接成 定义库名_名字_序号_定义库名, 比如 edatastructure_test_0_edatastructure
#define EDATASTRUCTURE_NAME(_index, _name) __LIB2_FNE_NAME_LEFT(__EDATASTRUCTURE_NAME(_index, _name))__LIB2_FNE_NAME_LEFT(__E_FNENAME)

// 传递函数名和索引, 拼接成 "定义库名_名字_序号_定义库名", 比如 "edatastructure_test_0_edatastructure"
#define EDATASTRUCTURE_NAME_STR(_index, _name) ______E_FNENAME(__EDATASTRUCTURE_NAME(_index, _name))

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
#define EDATASTRUCTURE_DEF(_MAKE) \
    _MAKE(  0, "创建", CreateNode, "在内存中创建一个节点的数据类型。该命令在进入变量的作用于后，系统会自动调用", -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED | CT_IS_OBJ_CONSTURCT_CMD, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_edatastructure_global_var + 0)\
    _MAKE(  1, "释放", FreeNode, "释放已经创建的节点。该命令在离开变量的作用域后，系统会自动调用该方法", -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED | CT_IS_OBJ_FREE_CMD, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_edatastructure_global_var + 0)\
    _MAKE(  2, "拷贝构造函数", NodeCopy, "拷贝构造。该命令在系统调用“=”时，系统会自动调用该方法", -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED | CT_IS_OBJ_COPY_CMD, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_edatastructure_global_var + 0)\
    _MAKE(  3, "加入属性", AddAttr, "在节点中加入属性。成功加入返回真，失败返回假。如果失败，可以通过“取失败原因”方法得到出错信息", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_edatastructure_global_var + 1)\
    _MAKE(  4, "删除属性", DelAttr, "从节点中删除属性。成功删除返回真，失败返回假。如果失败，可以通过“取失败原因”方法得到出错信息", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_edatastructure_global_var + 3)\
    _MAKE(  5, "修改属性", ModifyAttr, "修改节点的属性值。成功修改返回真，失败返回假。如果失败，可以通过“取失败原因”方法得到出错信息", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_edatastructure_global_var + 4)\
    _MAKE(  6, "取全部属性名", GetAllAttrName, "取出节点的全部属性名称。成功返回真，失败返回假。如果失败，可以通过“取失败原因”方法得到出错信息", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_edatastructure_global_var + 6)\
    _MAKE(  7, "取属性类型", GetAttrKind, "取出节点属性值的数据类型。成功返回真，失败返回假。如果失败，可以通过“取失败原因”方法得到出错信息", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_edatastructure_global_var + 7)\
    _MAKE(  8, "取数值", GetDoubleValue, "取出节点的数值型属性值。成功返回真，失败返回假。如果失败，可以通过“取失败原因”方法得到出错信息", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_edatastructure_global_var + 9)\
    _MAKE(  9, "取逻辑值", GetBoolValue, "取出节点的逻辑型属性值。成功返回真，失败返回假。如果失败，可以通过“取失败原因”方法得到出错信息", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_edatastructure_global_var + 11)\
    _MAKE( 10, "取日期值", GetDateValue, "取出节点的日期型属性值。成功返回真，失败返回假。如果失败，可以通过“取失败原因”方法得到出错信息", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_edatastructure_global_var + 13)\
    _MAKE( 11, "取文本值", GetTextValue, "取出节点的文本型属性值。成功返回真，失败返回假。如果失败，可以通过“取失败原因”方法得到出错信息", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_edatastructure_global_var + 15)\
    _MAKE( 12, "取字节集值", GetBinValue, "取出节点的字节集型属性值。成功返回真，失败返回假。如果失败，可以通过“取失败原因”方法得到出错信息", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_edatastructure_global_var + 17)\
    _MAKE( 13, "创建", ListCreate, "链表的构造函数，默认调用", -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED | CT_IS_OBJ_CONSTURCT_CMD, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_edatastructure_global_var + 0)\
    _MAKE( 14, "释放", ListRelease, "链表的析构函数，默认调用", -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED | CT_IS_OBJ_FREE_CMD, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_edatastructure_global_var + 0)\
    _MAKE( 15, "拷贝构造函数", ListCopy, "拷贝构造。该命令在系统调用“=”时，系统会自动调用该方法", -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED | CT_IS_OBJ_COPY_CMD, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_edatastructure_global_var + 19)\
    _MAKE( 16, "是否为空", ListIsEmpty, "判断链表中是否存有数据。如果链表为空返回真，如果链表非空返回假", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_edatastructure_global_var + 0)\
    _MAKE( 17, "取大小", ListGetCount, "取出链表中节点的数量", -1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_edatastructure_global_var + 0)\
    _MAKE( 18, "加入节点", ListAddNode, "在链表中加入新的节点。加入链表中自动根据键值参数进行从小到大的排序。键值参数可以省略，如果省略，默认键值为链表中最大的键值加一。该方法执行成功返回真，失败返回假。如果失败，可以通过“取失败原因”方法得到出错信息", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_edatastructure_global_var + 20)\
    _MAKE( 19, "删除首节点", ListRemoveHead, "删除链表的首节点。该方法执行成功返回真，失败返回假。如果失败，可以通过“取失败原因”方法得到出错信息", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_edatastructure_global_var + 0)\
    _MAKE( 20, "删除尾节点", ListRemoveTail, "删除链表的尾节点。该方法执行成功返回真，失败返回假。如果失败，可以通过“取失败原因”方法得到出错信息", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_edatastructure_global_var + 0)\
    _MAKE( 21, "全部删除", ListRemoveAll, "删除链表的全部节点。该方法执行成功返回真，失败返回假。如果失败，可以通过“取失败原因”方法得到出错信息", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_edatastructure_global_var + 0)\
    _MAKE( 22, "删除当前节点", ListRemoveCurNode, "删除链表的当前节点。该方法执行成功返回真，失败返回假。如果失败，可以通过“取失败原因”方法得到出错信息", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_edatastructure_global_var + 0)\
    _MAKE( 23, "删除节点", ListRemoveNodeFromKey, "删除链表中指定键值的节点。该方法执行成功返回真，失败返回假。如果失败，可以通过“取失败原因”方法得到出错信息", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_edatastructure_global_var + 22)\
    _MAKE( 24, "修改节点", ListUpdateNodeFromKey, "使用参数节点替换链表中指定键值的节点。该方法执行成功返回真，失败返回假。如果失败，可以通过“取失败原因”方法得到出错信息", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_edatastructure_global_var + 23)\
    _MAKE( 25, "修改当前节点", ListUpdateCurNode, "修改链表中的当前节点为参数节点。该方法执行成功返回真，失败返回假。如果失败，可以通过“取失败原因”方法得到出错信息", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_edatastructure_global_var + 25)\
    _MAKE( 26, "向下移动", ListGoNext, "链表中的当前节点向下移动一个节点。该方法执行成功返回真，执行失败返回假。如果是链表尾节点执行“向下移动”，返回假", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_edatastructure_global_var + 0)\
    _MAKE( 27, "向上移动", ListGoPrev, "链表中的当前节点向上移动一个节点。该方法执行成功返回真，执行失败返回假。如果是链表首节点执行“向上移动”，返回假", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_edatastructure_global_var + 0)\
    _MAKE( 28, "到首节点", ListGoHead, "链表中的当前节点移动到首节点", -1, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_edatastructure_global_var + 0)\
    _MAKE( 29, "到尾节点", ListGoTail, "链表中的当前节点移动到尾节点", -1, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_edatastructure_global_var + 0)\
    _MAKE( 30, "搜索节点", ListSearch, "链表中的当前节点移动到参数键值指定的节点。该方法执行成功返回真，执行失败返回假。如果失败，可以通过“取失败原因”方法得到出错信息", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_edatastructure_global_var + 26)\
    _MAKE( 31, "取当前节点键值", ListGetCurNodeKey, "取链表中当前节点的键值。该方法执行成功返回真，执行失败返回假。如果失败，可以通过“取失败原因”方法得到出错信息", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_edatastructure_global_var + 27)\
    _MAKE( 32, "取当前节点", CListGetCurNode, "取链表中的当前节点。该方法执行成功返回真，执行失败返回假。如果失败，可以通过“取失败原因”方法得到出错信息", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_edatastructure_global_var + 28)\
    _MAKE( 33, "取节点", CListGetNode, "根据参数键值取链表中的节点。该方法执行成功返回真，执行失败返回假。如果失败，可以通过“取失败原因”方法得到出错信息", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_edatastructure_global_var + 29)\
    _MAKE( 34, "创建", StackCreate, "栈的构造函数，默认调用", -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED | CT_IS_OBJ_CONSTURCT_CMD, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_edatastructure_global_var + 0)\
    _MAKE( 35, "释放", StackRelease, "栈的析构函数，默认调用", -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED | CT_IS_OBJ_FREE_CMD, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_edatastructure_global_var + 0)\
    _MAKE( 36, "拷贝构造函数", StackCopy, "拷贝构造。该命令在系统调用“=”时，系统会自动调用该方法", -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED | CT_IS_OBJ_COPY_CMD, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_edatastructure_global_var + 31)\
    _MAKE( 37, "是否为空", StackIsEmpty, "判断栈中是否存有数据。如果栈为空返回真，如果栈非空返回假", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_edatastructure_global_var + 0)\
    _MAKE( 38, "取大小", StackGetCount, "取出栈中节点的数量", -1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_edatastructure_global_var + 0)\
    _MAKE( 39, "清空", StackClear, "删除栈中的所有节点", -1, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_edatastructure_global_var + 0)\
    _MAKE( 40, "取栈顶节点", StackPeek, "取栈顶端的节点值，与“弹出”方法不同的是栈顶的节点并不出栈。该方法执行成功返回真，执行失败返回假。如果失败，可以通过“取失败原因”方法得到出错信息", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_edatastructure_global_var + 32)\
    _MAKE( 41, "压入", StackPush, "向栈中压入一个节点。该方法执行成功返回真，执行失败返回假。如果失败，可以通过“取失败原因”方法得到出错信息", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_edatastructure_global_var + 33)\
    _MAKE( 42, "弹出", StackPop, "从栈中弹出一个节点。该方法执行成功返回真，执行失败返回假。如果失败，可以通过“取失败原因”方法得到出错信息", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_edatastructure_global_var + 34)\
    _MAKE( 43, "创建", QueueCreate, "队列的构造函数，默认调用", -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED | CT_IS_OBJ_CONSTURCT_CMD, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_edatastructure_global_var + 0)\
    _MAKE( 44, "释放", QueueRelease, "队列的析构函数，默认调用", -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED | CT_IS_OBJ_FREE_CMD, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_edatastructure_global_var + 0)\
    _MAKE( 45, "拷贝构造函数", QueueCopy, "拷贝构造。该命令在系统调用“=”时，系统会自动调用该方法", -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED | CT_IS_OBJ_COPY_CMD, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_edatastructure_global_var + 35)\
    _MAKE( 46, "是否为空", QueueIsEmpty, "判断队列中是否存有数据。如果栈为空返回真，如果栈非空返回假", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_edatastructure_global_var + 0)\
    _MAKE( 47, "取大小", QueueGetCount, "取出队列中节点的数量", -1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_edatastructure_global_var + 0)\
    _MAKE( 48, "清空", QueueClear, "删除队列中的所有节点", -1, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_edatastructure_global_var + 0)\
    _MAKE( 49, "取队列首节点", QueuePeek, "取队列的首节点，与“弹出”方法不同的是对列的首节点并不弹出，仍保留在对列中。该方法执行成功返回真，执行失败返回假。如果失败，可以通过“取失败原因”方法得到出错信息", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_edatastructure_global_var + 36)\
    _MAKE( 50, "压入", QueuePush, "向队列的尾部压入一个节点。该方法执行成功返回真，执行失败返回假。如果失败，可以通过“取失败原因”方法得到出错信息", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_edatastructure_global_var + 37)\
    _MAKE( 51, "弹出", QueuePop, "从队列的首部弹出一个节点。该方法执行成功返回真，执行失败返回假。如果失败，可以通过“取失败原因”方法得到出错信息", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_edatastructure_global_var + 38)\
    _MAKE( 52, "创建", BiTreeCreate, "二叉树的构造函数，默认调用", -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED | CT_IS_OBJ_CONSTURCT_CMD, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_edatastructure_global_var + 0)\
    _MAKE( 53, "释放", BiTreeRelease, "二叉树的析构函数，默认调用", -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED | CT_IS_OBJ_FREE_CMD, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_edatastructure_global_var + 0)\
    _MAKE( 54, "拷贝构造函数", BiTreeCopy, "拷贝构造。该命令在系统调用“=”时，系统会自动调用该方法", -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED | CT_IS_OBJ_COPY_CMD, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_edatastructure_global_var + 39)\
    _MAKE( 55, "是否为空", BiTreeIsEmpty, "判断二叉树中是否存有数据。如果二叉树为空返回真，如果二叉树非空返回假", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_edatastructure_global_var + 0)\
    _MAKE( 56, "取大小", BiTreeGetCount, "取出二叉树中节点的数量", -1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_edatastructure_global_var + 0)\
    _MAKE( 57, "取二叉树深度", BiTreeGetDeep, "取出二叉树的深度", -1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_edatastructure_global_var + 0)\
    _MAKE( 58, "清空", BiTreeEmpty, "删除二叉树中的所有节点", -1, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_edatastructure_global_var + 0)\
    _MAKE( 59, "到左子树", BiTreeGoLeft, "移动二叉树的当前节点到它的左子树节点上。成功返回真，如果失败或者二叉树的左子树为空，返回假", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_edatastructure_global_var + 0)\
    _MAKE( 60, "到右子树", BiTreeGoRight, "移动二叉树的当前节点到它的右子树节点上。成功返回真，如果失败或者二叉树的右子树为空，返回假", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_edatastructure_global_var + 0)\
    _MAKE( 61, "搜索节点", BiTreeFind, "移动二叉树的当前节点到指定键值的节点上。成功返回真，失败返回假", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_edatastructure_global_var + 40)\
    _MAKE( 62, "到根节点", BiTreeGoRoot, "移动二叉树的当前节点到根节点上。成功返回真，失败返回假", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_edatastructure_global_var + 0)\
    _MAKE( 63, "到父节点", BiTreeGoParent, "移动二叉树的当前节点到其父节点上。成功返回真，失败返回假。如果当前节点为根节点，执行该方法返回假", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_edatastructure_global_var + 0)\
    _MAKE( 64, "取当前节点", BiTreeGetCurNode, "取二叉树的当前节点。成功返回真，失败返回假", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_edatastructure_global_var + 41)\
    _MAKE( 65, "取当前节点键值", BiTreeGetCurNodeKey, "取二叉树的当前节点的键值。成功返回真，失败返回假", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_edatastructure_global_var + 42)\
    _MAKE( 66, "插入节点", BiTreeInsert, "向二叉树中插入节点。成功返回真，失败返回假", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_edatastructure_global_var + 43)\
    _MAKE( 67, "修改节点", BiTreeUpdate, "修改二叉树中指定键值的节点。成功返回真，失败返回假", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_edatastructure_global_var + 45)\
    _MAKE( 68, "修改当前节点", BiTreeUpdateCurNode, "修改二叉树中当前节点。成功返回真，失败返回假", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_edatastructure_global_var + 47)\
    _MAKE( 69, "删除节点", BiTreeDelete, "删除二叉树中指定键值的节点。如果参数“删除标志”为假，仅删除一个节点的情况下分为以下几种不同的情形和执行结果：一，欲删除节点没有子节点，直接删除节点。二，欲删除节点仅有左子节点，删除节点后，把删除节点的左子节点连接到删除节点的双亲上。三，欲删除节点仅有右子节点，删除节点后，把删除节点的右子节点连接到删除节点的双亲上。四，欲删除节点有左子节点和右子节点，删除节点后，把删除节点的左子树上键值最大的节点（也是最右边的节点）连接到删除节点的双亲上，也就是提升到删除节点的位置，然后把它的左子树连接到它以前位置的双亲上。成功删除后，二叉树的当前节点值恢复为根节点值。该方法成功返回真，失败返回假", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_edatastructure_global_var + 48)\
    _MAKE( 70, "删除当前节点", BiTreeDeleteCurNode, "删除二叉树中当前节点。删除原则和“删除节点”方法相同。成功删除后，二叉树的当前节点值恢复为根节点值。该方法成功返回真，失败返回假", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_edatastructure_global_var + 50)\
    _MAKE( 71, "创建", TreeCreate, "树的构造函数，默认调用", -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED | CT_IS_OBJ_CONSTURCT_CMD, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_edatastructure_global_var + 0)\
    _MAKE( 72, "释放", TreeRelease, "树的析构函数，默认调用", -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED | CT_IS_OBJ_FREE_CMD, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_edatastructure_global_var + 0)\
    _MAKE( 73, "拷贝构造函数", TreeCopy, "拷贝构造。该命令在系统调用“=”时，系统会自动调用该方法", -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED | CT_IS_OBJ_COPY_CMD, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_edatastructure_global_var + 51)\
    _MAKE( 74, "是否为空", TreeIsEmpty, "判断树中是否存有数据。如果树为空返回真，如果树非空返回假", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_edatastructure_global_var + 0)\
    _MAKE( 75, "取大小", TreeGetCount, "取出树中节点的数量", -1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_edatastructure_global_var + 0)\
    _MAKE( 76, "清空", TreeEmpty, "删除树中的所有节点", -1, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_edatastructure_global_var + 0)\
    _MAKE( 77, "到子节点", TreeGoChild, "移动当前节点到它的第一个子节点。成功返回真，失败或者当前节点没有子节点返回假。树的一个节点可以有多个子节点，该方法仅能移动当前节点到它的第一个子节点，其他节点的遍历可以通过“到兄弟节点”方法实现", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_edatastructure_global_var + 0)\
    _MAKE( 78, "到兄弟节点", TreeGoSibling, "移动当前节点到它的下一个兄弟节点。成功返回真，失败或者当前节点没有下一个兄弟节点返回假。父节点的所有子节点以链表的形态存在，该方法仅能向一个方向移动，在其中的一个子节点可以通过该方法移动到下一个兄弟节点，但是无法反向移动", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_edatastructure_global_var + 0)\
    _MAKE( 79, "到根节点", TreeGoRoot, "移动当前节点到树的根节点。成功返回真，失败返回假。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_edatastructure_global_var + 0)\
    _MAKE( 80, "到父节点", TreeGoParent, "移动当前节点到其父节点。成功返回真，失败返回假。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_edatastructure_global_var + 0)\
    _MAKE( 81, "取当前节点", TreeGetCurNode, "取树的当前节点。成功返回真，失败返回假", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_edatastructure_global_var + 52)\
    _MAKE( 82, "取子节点数", TreeGetChildCount, "取当前节点的子节点数量", -1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_edatastructure_global_var + 0)\
    _MAKE( 83, "插入节点", TreeInsert, "向树中插入节点，插入位置为当前节点的子节点链表的末端。成功返回真，失败返回假。如果树是一个空的树，则把插入的节点作为树的根节点", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_edatastructure_global_var + 53)\
    _MAKE( 84, "修改当前节点", TreeUpdateCurNode, "修改树中当前节点的值为参数节点的值。成功返回真，失败返回假", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_edatastructure_global_var + 47)\
    _MAKE( 85, "删除当前节点", TreeDeleteCurNode, "删除树中当前节点。成功删除后，树的当前节点值恢复为根节点值。成功返回真，失败返回假", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_edatastructure_global_var + 54)\
    _MAKE( 86, "取节点索引", TreeGetCurNodeIndex, "取当前节点在其父节点的所有子节点数组中的索引。如果失败将返回-1", -1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_edatastructure_global_var + 0)\
    _MAKE( 87, "取失败原因", DSGetErrMsg, "得到上次方法执行失败的原因信息。该方法必须在方法执行失败后马上执行，才能正确取出失败原因", 1, _CMD_OS(__OS_WIN), SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_edatastructure_global_var + 0)\
    _MAKE( 88, "创建", CreateNode, "创建MAP。该命令在进入变量的作用于后，系统会自动调用", -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED | CT_IS_OBJ_CONSTURCT_CMD, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_edatastructure_global_var + 0)\
    _MAKE( 89, "释放", FreeNode, "释放已经创建的MAP。该命令在离开变量的作用域后，系统会自动调用该方法", -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED | CT_IS_OBJ_FREE_CMD, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_edatastructure_global_var + 0)\
    _MAKE( 90, "拷贝构造函数", MapCopy, "拷贝构造。该命令在系统调用“=”时，系统会自动调用该方法", -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED | CT_IS_OBJ_COPY_CMD, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_edatastructure_global_var + 55)\
    _MAKE( 91, "加入键值对", AddKeyValue, "将“键”和“值”两者配对加入到对象中。如果已存在相同的“键”，则新“值”将覆盖旧“值”。执行成功返回真，失败返回假。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_edatastructure_global_var + 56)\
    _MAKE( 92, "取键值", GetKeyValue, "如果对象中没有指定的键，则返回假。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_edatastructure_global_var + 58)\
    _MAKE( 93, "取键值数", GetSize, "返回对象中“键-值”对的个数。", -1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_edatastructure_global_var + 0)\
    _MAKE( 94, "取所有键", GetAllKeys, "返回由所有键组合的字节集数组。", -1, _CMD_OS(__OS_WIN) | CT_RETRUN_ARY_TYPE_DATA, SDT_BIN, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_edatastructure_global_var + 0)\
    _MAKE( 95, "删除键值对", DelKeyValue, "删除对象中指定的键及其值。成功返回真，失败返回假。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_edatastructure_global_var + 56)\
    _MAKE( 96, "删除所有键值对", DelAllKeyValues, NULL, -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_edatastructure_global_var + 0)

