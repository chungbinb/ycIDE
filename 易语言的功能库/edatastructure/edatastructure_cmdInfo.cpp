#if !defined(__E_STATIC_LIB)
#include "include_edatastructure_header.h"

//TODO 静态库需要的部分,会记录所有的函数名到数组里,静态编译的时候需要取回命令名
static ARG_INFO g_argumentInfo_edatastructure_global_var[] = 
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



    /*000*/ {"源节点", "“拷贝构造”方法中欲拷贝的源节点值", 0, 0, MAKELONG(0x01, 0), 0, NULL},

    /*001*/ {"属性名称", "“加入属性”方法中欲加入的新的属性名称", 0, 0, SDT_TEXT, 0, NULL},
    /*002*/ {"属性值", "“加入属性”方法中欲加入的新的属性值", 0, 0, _SDT_ALL, 0, NULL},

    /*003*/ {"属性名称", "“删除属性”方法中欲删除的属性名称", 0, 0, SDT_TEXT, 0, NULL},

    /*004*/ {"属性名称", "“修改属性”方法中欲修改的属性名称", 0, 0, SDT_TEXT, 0, NULL},
    /*005*/ {"新属性值", "“修改属性”方法中欲修改的新的属性值", 0, 0, _SDT_ALL, 0, NULL},

    /*006*/ {"属性名称数组", "“取全部属性名”方法的填充参数。方法执行完毕后，该参数的内容为全部属性名称的数组。如果方法执行失败，该参数内容无意义", 0, 0, SDT_TEXT, 0, AS_RECEIVE_VAR_ARRAY},

    /*007*/ {"属性名称", "“取属性类型”方法中欲取类型的属性名称", 0, 0, SDT_TEXT, 0, NULL},
    /*008*/ {"类型值", "“取属性类型”方法的填充参数。方法执行完毕后，该参数的内容为属性类型的代表数值。0代表数值型，1代表逻辑型，2代表日期型，3代表文本型，4代表字节集型。如果方法执行失败，该参数内容无意义", 0, 0, SDT_INT, 0, AS_RECEIVE_VAR},

    /*009*/ {"属性名称", "“取数值”方法中欲取值的属性名称", 0, 0, SDT_TEXT, 0, NULL},
    /*010*/ {"属性值", "“取数值”方法的填充参数。该参数必须为数值型，否则“取数值”方法返回假。方法执行完毕后，该参数的内容为属性的数值，如果方法执行失败，该参数内容无意义", 0, 0, _SDT_ALL, 0, AS_RECEIVE_VAR},

    /*011*/ {"属性名称", "“取逻辑值”方法中欲取值的属性名称", 0, 0, SDT_TEXT, 0, NULL},
    /*012*/ {"属性值", "“取逻辑值”方法的填充参数。方法执行完毕后，该参数的内容为属性的逻辑值，如果方法执行失败，该参数内容无意义", 0, 0, SDT_BOOL, 0, AS_RECEIVE_VAR},

    /*013*/ {"属性名称", "“取日期值”方法中欲取值的属性名称", 0, 0, SDT_TEXT, 0, NULL},
    /*014*/ {"属性值", "“取日期值”方法的填充参数。方法执行完毕后，该参数的内容为属性的日期值，如果方法执行失败，该参数内容无意义", 0, 0, SDT_DATE_TIME, 0, AS_RECEIVE_VAR},

    /*015*/ {"属性名称", "“取文本值”方法中欲取值的属性名称", 0, 0, SDT_TEXT, 0, NULL},
    /*016*/ {"属性值", "“取文本值”方法的填充参数。方法执行完毕后，该参数的内容为属性的文本值，如果方法执行失败，该参数内容无意义", 0, 0, SDT_TEXT, 0, AS_RECEIVE_VAR},

    /*017*/ {"属性名称", "“取字节集值”方法中欲取值的属性名称", 0, 0, SDT_TEXT, 0, NULL},
    /*018*/ {"属性值", "“取字节集值”方法的填充参数。方法执行完毕后，该参数的内容为属性的字节集值，如果方法执行失败，该参数内容无意义", 0, 0, SDT_BIN, 0, AS_RECEIVE_VAR},

    /*019*/ {"源链表", "“拷贝构造”方法中欲拷贝的源链表", 0, 0, MAKELONG(0x02, 0), 0, NULL},

    /*020*/ {"节点值", "“加入节点”方法中欲加入到链表中的节点数据", 0, 0, MAKELONG(0x01, 0), 0, NULL},
    /*021*/ {"键值", "“加入节点”方法中欲加入到链表中的节点数据的键值。这个值也是节点在链表中的唯一标识，节点在链表中存在的过程中键值是不允许改变的。该键值必须大于等于0", 0, 0, SDT_INT, 0, AS_DEFAULT_VALUE_IS_EMPTY},

    /*022*/ {"键值", "“删除节点”方法中欲删除节点数据的键值", 0, 0, SDT_INT, 0, NULL},

    /*023*/ {"节点值", "“修改节点”方法中链表节点被更改后的新节点数据", 0, 0, MAKELONG(0x01, 0), 0, NULL},
    /*024*/ {"键值", "“修改节点”方法中欲更改的节点数据的键值", 0, 0, SDT_INT, 0, NULL},

    /*025*/ {"节点值", "“修改当前节点”方法中链表节点被更改后的新节点数据", 0, 0, MAKELONG(0x01, 0), 0, NULL},

    /*026*/ {"键值", "“搜索节点”方法中欲搜索节点数据的键值", 0, 0, SDT_INT, 0, NULL},

    /*027*/ {"键值", "“取当前节点键值”方法的填充参数。方法成功执行，该参数的内容为链表中当前节点的键值", 0, 0, SDT_INT, 0, AS_RECEIVE_VAR},

    /*028*/ {"节点值", "“取当前节点”方法的填充参数。方法成功执行，该参数的内容为链表中当前节点。如果方法执行失败，该参数内容无意义", 0, 0, MAKELONG(0x01, 0), 0, AS_RECEIVE_VAR},

    /*029*/ {"节点值", "“取节点”方法的填充参数。方法成功执行，该参数的内容为链表中的节点数据。如果方法执行失败，该参数内容无意义", 0, 0, MAKELONG(0x01, 0), 0, AS_RECEIVE_VAR},
    /*030*/ {"键值", "“取节点”方法中欲取得节点数据的键值", 0, 0, SDT_INT, 0, NULL},

    /*031*/ {"源栈", "“拷贝构造”方法中欲拷贝的源栈", 0, 0, MAKELONG(0x03, 0), 0, NULL},

    /*032*/ {"节点值", "“取栈顶节点”方法的填充参数，该方法执行完毕，该参数的内容为从栈顶的节点值。如果方法执行失败，该参数内容无意义", 0, 0, MAKELONG(0x01, 0), 0, AS_RECEIVE_VAR},

    /*033*/ {"节点值", "“压入”方法中欲压到栈中节点", 0, 0, MAKELONG(0x01, 0), 0, NULL},

    /*034*/ {"节点值", "“弹出”方法的填充参数，该方法执行完毕，该参数的内容为从栈顶弹出的节点值。如果方法执行失败，该参数内容无意义", 0, 0, MAKELONG(0x01, 0), 0, AS_RECEIVE_VAR},

    /*035*/ {"源队列", "“拷贝构造”方法中欲拷贝的源队列", 0, 0, MAKELONG(0x04, 0), 0, NULL},

    /*036*/ {"节点值", "“取队列首节点”方法的填充参数，该方法执行完毕，该参数的内容为队列首节点值。如果方法执行失败，该参数内容无意义", 0, 0, MAKELONG(0x01, 0), 0, AS_RECEIVE_VAR},

    /*037*/ {"节点值", "“压入”方法中欲压到队列尾的节点", 0, 0, MAKELONG(0x01, 0), 0, NULL},

    /*038*/ {"节点值", "“弹出”方法的填充参数，该方法执行完毕，该参数的内容为从队列首弹出的节点值", 0, 0, MAKELONG(0x01, 0), 0, AS_RECEIVE_VAR},

    /*039*/ {"源二叉树", "“拷贝构造”方法中欲拷贝的源二叉树", 0, 0, MAKELONG(0x05, 0), 0, NULL},

    /*040*/ {"节点键值", "欲搜索二叉树中节点的键值", 0, 0, SDT_INT, 0, NULL},

    /*041*/ {"节点值", "“取当前节点”方法的填充参数，该方法执行完毕，该参数的内容为二叉树的当前节点值", 0, 0, MAKELONG(0x01, 0), 0, AS_RECEIVE_VAR},

    /*042*/ {"键值", "“取当前节点键值”方法的填充参数，该方法执行完毕，该参数的内容为二叉树的当前节点的键值", 0, 0, SDT_INT, 0, AS_RECEIVE_VAR},

    /*043*/ {"节点值", "“插入节点”方法中欲插入到二叉树中的节点值", 0, 0, MAKELONG(0x01, 0), 0, NULL},
    /*044*/ {"键值", "“插入节点”方法中欲插入到二叉树中的节点的键值。该键值不能为负数。该参数可以省略，如果省略，则新插入节点的键值为二叉树中的最大键值加一", 0, 0, SDT_INT, 0, AS_DEFAULT_VALUE_IS_EMPTY},

    /*045*/ {"节点值", "“修改节点”方法中修改后的节点值", 0, 0, MAKELONG(0x01, 0), 0, NULL},
    /*046*/ {"键值", "“修改节点”方法中欲修改节点的键值", 0, 0, SDT_INT, 0, NULL},

    /*047*/ {"节点值", "“修改当前节点”方法中修改后的节点值", 0, 0, MAKELONG(0x01, 0), 0, NULL},

    /*048*/ {"键值", "“删除节点”方法中欲删除节点的键值", 0, 0, SDT_INT, 0, NULL},
    /*049*/ {"删除标记", "该标记为真时表示删除节点及其所有的子节点，为假时表示仅删除单个节点。该参数可以省略，默认为假", 0, 0, SDT_BOOL, 0, AS_DEFAULT_VALUE_IS_EMPTY},

    /*050*/ {"删除标记", "该标记为真时表示删除节点及其所有的子节点，为假时表示仅删除单个节点。该参数可以省略，默认为假", 0, 0, SDT_BOOL, 0, AS_DEFAULT_VALUE_IS_EMPTY},

    /*051*/ {"源树", "“拷贝构造”方法中欲拷贝的源树", 0, 0, MAKELONG(0x06, 0), 0, NULL},

    /*052*/ {"节点值", "“取当前节点”方法的填充参数，该方法执行完毕，该参数的内容为树的当前节点值", 0, 0, MAKELONG(0x01, 0), 0, AS_RECEIVE_VAR},

    /*053*/ {"节点值", "“插入节点”方法中欲插入到树中的节点值", 0, 0, MAKELONG(0x01, 0), 0, NULL},

    /*054*/ {"删除标记", "该标记为真时表示删除节点及其所有的子节点，为假时表示仅删除单个节点。如果该节点有子节点，该参数同时为假，该方法不执行删除操作，返回假。该参数可以省略，默认为假", 0, 0, SDT_BOOL, 0, AS_DEFAULT_VALUE_IS_EMPTY},

    /*055*/ {"源MAP", "“拷贝构造”方法中欲拷贝的源MAP", 0, 0, MAKELONG(0x07, 0), 0, NULL},

    /*056*/ {"键", NULL, 0, 0, SDT_BIN, 0, NULL},
    /*057*/ {"值", NULL, 0, 0, MAKELONG(0x01, 0), 0, NULL},

    /*058*/ {"键", NULL, 0, 0, SDT_BIN, 0, NULL},
    /*059*/ {"值", NULL, 0, 0, MAKELONG(0x01, 0), 0, AS_RECEIVE_VAR},

};

#ifdef _DEBUG    // 这里是为了确认参数序号是否正确, 成员数比最后一个序号大1就是正确
const int dbg_cmd_arg_count__ = sizeof(g_argumentInfo_edatastructure_global_var) / sizeof(g_argumentInfo_edatastructure_global_var[0]);
#endif


#define EDATASTRUCTURE_DEF_CMDINFO(_index, _szName, _szEgName, _szExplain, _shtCategory, _wState, _dtRetValType, _wReserved, _shtUserLevel, _shtBitmapIndex, _shtBitmapCount, _nArgCount, _pBeginArgInfo) \
    { _szName, ______E_FNENAME(_szEgName), _szExplain, _shtCategory, _wState, _dtRetValType, _wReserved, _shtUserLevel, _shtBitmapIndex, _shtBitmapCount, _nArgCount, _pBeginArgInfo },


// 易语言每个命令的定义, 会把信息显示在支持库列表里, 这里每个成员都是详细的描述一个命令的信息
CMD_INFO g_cmdInfo_edatastructure_global_var[] = 
{
    EDATASTRUCTURE_DEF(EDATASTRUCTURE_DEF_CMDINFO)
};

int g_cmdInfo_edatastructure_global_var_count = sizeof(g_cmdInfo_edatastructure_global_var) / sizeof(g_cmdInfo_edatastructure_global_var[0]);

#endif

