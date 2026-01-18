#ifndef __E_STATIC_LIB
#include "include_shell_header.h"

LIB_CONST_INFO g_ConstInfo_shell_global_var[] = 
{
//#define    CT_NULL 0
//#define    CT_NUM  1      // value sample: 3.1415926
//#define    CT_BOOL 2      // value sample: 1
//#define    CT_TEXT 3      // value sample: "abc"
//LPCSTR m_szName;          // 常量名
//LPCSTR m_szEgName;        // 英文名
//LPCSTR m_szExplain;       // 说明
//SHORT  m_shtLayout;       // 必须为 1
//SHORT  m_shtType;         // 常量类型, CT_NULL=空常量, CT_NUM=数值型,double保存值, CT_BOOL=逻辑型, CT_TEXT=文本型
//LPCSTR m_szText;          // CT_TEXT
//DOUBLE m_dbValue;         // CT_NUM、CT_BOOL
    /*000*/ { "编辑文档", NULL, NULL, 1, CT_NUM, NULL, 1 },
    /*001*/ { "浏览文件夹", NULL, NULL, 1, CT_NUM, NULL, 2 },
    /*002*/ { "寻找文件", NULL, NULL, 1, CT_NUM, NULL, 3 },
    /*003*/ { "打开文件", NULL, NULL, 1, CT_NUM, NULL, 4 },
    /*004*/ { "打印文档", NULL, NULL, 1, CT_NUM, NULL, 5 },
    /*005*/ { "我的文档", NULL, NULL, 1, CT_NUM, NULL, 1 },
    /*006*/ { "我的收藏夹", NULL, NULL, 1, CT_NUM, NULL, 2 },
    /*007*/ { "系统桌面", NULL, NULL, 1, CT_NUM, NULL, 3 },
    /*008*/ { "系统字体", NULL, NULL, 1, CT_NUM, NULL, 4 },
    /*009*/ { "开始菜单组", NULL, NULL, 1, CT_NUM, NULL, 5 },
    /*010*/ { "程序菜单组", NULL, NULL, 1, CT_NUM, NULL, 6 },
    /*011*/ { "启动菜单组", NULL, NULL, 1, CT_NUM, NULL, 7 },
    /*012*/ { "程序数据目录", NULL, NULL, 1, CT_NUM, NULL, 8 },
    /*013*/ { "Windows安装目录", NULL, NULL, 1, CT_NUM, NULL, 9 },
    /*014*/ { "Windows系统目录", NULL, NULL, 1, CT_NUM, NULL, 10 },
    /*015*/ { "临时文件目录", NULL, NULL, 1, CT_NUM, NULL, 11 },
    /*016*/ { "不询问删除", NULL, NULL, 1, CT_NUM, NULL, 1 },
    /*017*/ { "不提示错误", NULL, NULL, 1, CT_NUM, NULL, 2 },
    /*018*/ { "不显示进度", NULL, NULL, 1, CT_NUM, NULL, 4 },
    /*019*/ { "关机", NULL, NULL, 1, CT_NUM, NULL, 1 },
    /*020*/ { "重启", NULL, NULL, 1, CT_NUM, NULL, 2 },
    /*021*/ { "注销", NULL, NULL, 1, CT_NUM, NULL, 3 },
    /*022*/ { "休眠", NULL, NULL, 1, CT_NUM, NULL, 4 },
    /*023*/ { "冬眠", NULL, NULL, 1, CT_NUM, NULL, 5 },
};
int g_ConstInfo_shell_global_var_count = sizeof(g_ConstInfo_shell_global_var) / sizeof(g_ConstInfo_shell_global_var[0]);
#endif
