#if !defined(__E_STATIC_LIB)
#include "include_internet_header.h"

//TODO 静态库需要的部分,会记录所有的函数名到数组里,静态编译的时候需要取回命令名
static ARG_INFO g_argumentInfo_internet_global_var[] = 
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

    /*000*/ {"发信邮件服务器地址", "指定用作发送邮件的SMTP邮件服务器地址", 0, 0, SDT_TEXT, 0, NULL},
    /*001*/ {"端口号", "指定欲连接到SMTP邮件服务器上的端口号，如果本参数被省略，默认值为标准 25 端口", 0, 0, SDT_INT, 0, AS_DEFAULT_VALUE_IS_EMPTY},
    /*002*/ {"用户名", "指定在SMTP邮件服务器上的用户帐号名称，如果本参数被省略，默认值为空文本，即无需登录", 0, 0, SDT_TEXT, 0, AS_DEFAULT_VALUE_IS_EMPTY},
    /*003*/ {"密码", "指定在SMTP邮件服务器上的用户帐号密码，如果本参数被省略，默认值为空文本，即无需登录", 0, 0, SDT_TEXT, 0, AS_DEFAULT_VALUE_IS_EMPTY},
    /*004*/ {"最长等待时间", "指定在发信过程中等待SMTP邮件服务器响应的最大时间，单位为毫秒，如果本参数被省略，默认值为 2000 毫秒", 0, 0, SDT_INT, 0, AS_DEFAULT_VALUE_IS_EMPTY},

    /*005*/ {"欲添加的附件文件", NULL, 0, 0, SDT_TEXT, 0, NULL},

    /*006*/ {"欲添加的附件数据", NULL, 0, 0, SDT_BIN, 0, NULL},
    /*007*/ {"附件标题", NULL, 0, 0, SDT_TEXT, 0, NULL},

    /*008*/ {"主题", NULL, 0, 0, SDT_TEXT, 0, NULL},
    /*009*/ {"正文", NULL, 0, 0, SDT_TEXT, 0, NULL},
    /*010*/ {"收信人邮件地址", "可以为地址列表并包括友好名称，示例如：“客服〈service@dywt.com.cn〉;售前〈sale@dywt.com.cn〉” 或：“service@dywt.com.cn;sale@dywt.com.cn”都可以，多地址之间用逗号或分号分隔，友好名称后的邮件地址用半角尖括号括住", 0, 0, SDT_TEXT, 0, NULL},
    /*011*/ {"抄送邮件地址", "地址格式等同于“收信人邮件地址”，如果本参数被省略，默认为空文本", 0, 0, SDT_TEXT, 0, AS_DEFAULT_VALUE_IS_EMPTY},
    /*012*/ {"暗送邮件地址", "地址格式等同于“收信人邮件地址”，如果本参数被省略，默认为空文本", 0, 0, SDT_TEXT, 0, AS_DEFAULT_VALUE_IS_EMPTY},
    /*013*/ {"发信人邮件地址", "只能为单个邮件地址，可以包括友好名称，示例如：“客服〈service@dywt.com.cn〉”或“service@dywt.com.cn”", 0, 0, SDT_TEXT, 0, NULL},
    /*014*/ {"回复邮件地址", "指定收件人回复此邮件的地址。如果被省略，默认为与发信人邮件地址相同", 0, 0, SDT_TEXT, 0, AS_DEFAULT_VALUE_IS_EMPTY},

    /*015*/ {"代理服务器名称", "如果要取消代理服务器设置，请提供空文本。如果本参数被省略，默认值为空文本", 0, 0, SDT_TEXT, 0, AS_DEFAULT_VALUE_IS_EMPTY},
    /*016*/ {"用户名", NULL, 0, 0, SDT_TEXT, 0, AS_DEFAULT_VALUE_IS_EMPTY},
    /*017*/ {"密码", NULL, 0, 0, SDT_TEXT, 0, AS_DEFAULT_VALUE_IS_EMPTY},

    /*018*/ {"HTTP文件地址", "本参数指定欲读取文件在互联网上的HTTP地址（即URL）", 0, 0, SDT_TEXT, 0, NULL},

    /*019*/ {"FTP服务器地址", "本参数指定FTP服务器在互联网上的地址", 0, 0, SDT_TEXT, 0, NULL},
    /*020*/ {"用户名", "本参数提供用作通过FTP服务器用户验证的用户名称，如果被省略，默认值为匿名用户名“anonymous”", 0, 0, SDT_TEXT, 0, AS_DEFAULT_VALUE_IS_EMPTY},
    /*021*/ {"密码", "本参数提供用作通过FTP服务器用户验证的密码，如果被省略，默认值为空文本", 0, 0, SDT_TEXT, 0, AS_DEFAULT_VALUE_IS_EMPTY},
    /*022*/ {"端口号", "本参数提供欲连接到FTP服务器上的端口号，如果被省略，默认值为标准端口 21 ", 0, 0, SDT_INT, 0, AS_DEFAULT_VALUE_IS_EMPTY},
    /*023*/ {"是否使用被动语义", "如果本参数被省略，默认值为假", 0, 0, SDT_BOOL, 0, AS_DEFAULT_VALUE_IS_EMPTY},

    /*024*/ {"欲下载的FTP文件", "本参数指定在FTP服务器上欲下载文件的路径", 0, 0, SDT_TEXT, 0, NULL},
    /*025*/ {"本地文件名", "本参数指定被下载文件的本地保存位置", 0, 0, SDT_TEXT, 0, NULL},
    /*026*/ {"是否使用ASCII码方式", "指定是否使用ASCII编码方式传递文件数据。如果本参数被省略，默认值为假，即使用二进制方式传送", 0, 0, SDT_BOOL, 0, AS_DEFAULT_VALUE_IS_EMPTY},

    /*027*/ {"欲上传的本地文件", "本参数指定欲上传到FTP服务器上本地文件的路径", 0, 0, SDT_TEXT, 0, NULL},
    /*028*/ {"FTP文件路径", "本参数指定上传到FTP服务器上文件的保存位置", 0, 0, SDT_TEXT, 0, NULL},
    /*029*/ {"是否使用ASCII码方式", "指定是否使用ASCII编码方式传递文件数据。如果本参数被省略，默认值为假，即使用二进制方式传送", 0, 0, SDT_BOOL, 0, AS_DEFAULT_VALUE_IS_EMPTY},

    /*030*/ {"欲删除的FTP文件", "本参数指定在FTP服务器上欲删除文件的路径", 0, 0, SDT_TEXT, 0, NULL},

    /*031*/ {"欲改名的FTP文件", "本参数指定FTP服务器上欲改名文件的路径", 0, 0, SDT_TEXT, 0, NULL},
    /*032*/ {"新文件名称", "本参数指定欲改为的文件名称", 0, 0, SDT_TEXT, 0, NULL},

    /*033*/ {"FTP目录路径", NULL, 0, 0, SDT_TEXT, 0, NULL},

    /*034*/ {"欲列出的文件", "可以包含路径，可以使用多字符 (*) 和单字符 (?) 通配符来指定多重文件及目录", 0, 0, SDT_TEXT, 0, NULL},
    /*035*/ {"欲列出文件的属性", "参数值可以为以下常量值或其和： 1、#只读文件； 2、#隐藏文件； 4、#系统文件； 16、#子目录； 32、#存档文件 。通过将这些常量值加起来可以一次设置多个文件属性。如果省略本参数，默认为搜寻所有子目录和文件", 0, 0, SDT_INT, 0, AS_DEFAULT_VALUE_IS_EMPTY},
    /*036*/ {"存放文件名的数组变量", "在命令执行完毕后，本变量数组内被顺序填入所找到的匹配文件和子目录名称。变量数组内原有数据被全部销毁，变量数组的维数被自动调整为所找到的文件数目。如果本参数被省略，默认值为空", 0, 0, SDT_TEXT, 0, AS_DEFAULT_VALUE_IS_EMPTY | AS_RECEIVE_VAR_ARRAY},
    /*037*/ {"存放文件属性的数组变量", "在命令执行完毕后，本变量数组内被顺序填入所找到的匹配文件的属性，并与文件名数组成员一一对应。此属性值由以下常量或其和构成： 1、#只读文件； 2、#隐藏文件；  4、#系统文件； 16、#子目录；32、#存档文件 。要判断是否设置了某个属性，在返回值与想要得知的属性值之间使用“位与”命令进行比较。如果所得的结果不为零，则表示设置了这个属性值。变量数组内原有数据被全部销毁，变量数组的维数被自动调整为所找到的文件数目。如果本参数被省略，默认值为空", 0, 0, SDT_INT, 0, AS_DEFAULT_VALUE_IS_EMPTY | AS_RECEIVE_VAR_ARRAY},
    /*038*/ {"存放文件尺寸的数组变量", "在命令执行完毕后，本变量数组内被顺序填入所找到的匹配文件的尺寸，并与文件名数组成员一一对应。变量数组内原有数据被全部销毁，变量数组的维数被自动调整为所找到的文件数目。如果本参数被省略，默认值为空", 0, 0, SDT_INT, 0, AS_DEFAULT_VALUE_IS_EMPTY | AS_RECEIVE_VAR_ARRAY},
    /*039*/ {"存放文件时间的数组变量", "在命令执行完毕后，本变量数组内被顺序填入所找到的匹配文件的最后修改时间，并与文件名数组成员一一对应。此文件时间即FTP服务器返回的文件时间，不做任何时区转换。变量数组内原有数据被全部销毁，变量数组的维数被自动调整为所找到的文件数目。如果本参数被省略，默认值为空", 0, 0, SDT_DATE_TIME, 0, AS_DEFAULT_VALUE_IS_EMPTY | AS_RECEIVE_VAR_ARRAY},

    /*040*/ {"拨号网络索引", "指定获取哪一个拨号网络配置信息，索引值从 0 开始，最大值为“取拨号连接数”命令的返回值减一", 0, 0, SDT_INT, 0, NULL},

    /*041*/ {"电话号码", "指定欲拨出的电话号码，如果欲使用已经存在的用户拨号网络配置中的电话号码，请使用“@n”格式文本，其中n为指定拨号网络配置的索引数值，从0开始。如：拨号 (“@0”, ...) 即使用第一个拨号网络配置中的电话号码拨号", 0, 0, SDT_TEXT, 0, NULL},
    /*042*/ {"用户帐号", "指定用户帐号名称", 0, 0, SDT_TEXT, 0, NULL},
    /*043*/ {"帐号密码", "指定用户帐号的密码", 0, 0, SDT_TEXT, 0, NULL},
    /*044*/ {"是否等待连接完毕", "指定命令是否一直等到连接完毕（成功或失败）后才返回，如果本参数为真，则“状态标签”参数无效。如果本参数被省略，默认值为假", 0, 0, SDT_BOOL, 0, AS_DEFAULT_VALUE_IS_EMPTY},
    /*045*/ {"状态标签", "如果提供了一个已经存在的标签型窗口组件并且“是否等待连接完毕”参数为假，拨号过程中的状态文本将即时显示到该标签上。如果本参数被省略，默认值为空", 0, 0, DTP_LABEL, 0, AS_DEFAULT_VALUE_IS_EMPTY},

};

#ifdef _DEBUG    // 这里是为了确认参数序号是否正确, 成员数比最后一个序号大1就是正确
const int dbg_cmd_arg_count__ = sizeof(g_argumentInfo_internet_global_var) / sizeof(g_argumentInfo_internet_global_var[0]);
#endif


#define INTERNET_DEF_CMDINFO(_index, _szName, _szEgName, _szExplain, _shtCategory, _wState, _dtRetValType, _wReserved, _shtUserLevel, _shtBitmapIndex, _shtBitmapCount, _nArgCount, _pBeginArgInfo) \
    { _szName, ______E_FNENAME(_szEgName), _szExplain, _shtCategory, _wState, _dtRetValType, _wReserved, _shtUserLevel, _shtBitmapIndex, _shtBitmapCount, _nArgCount, _pBeginArgInfo },


// 易语言每个命令的定义, 会把信息显示在支持库列表里, 这里每个成员都是详细的描述一个命令的信息
CMD_INFO g_cmdInfo_internet_global_var[] = 
{
    INTERNET_DEF(INTERNET_DEF_CMDINFO)
};

int g_cmdInfo_internet_global_var_count = sizeof(g_cmdInfo_internet_global_var) / sizeof(g_cmdInfo_internet_global_var[0]);

#endif

