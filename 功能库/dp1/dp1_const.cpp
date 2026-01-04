#ifndef __E_STATIC_LIB
#include "include_dp1_header.h"

LIB_CONST_INFO g_ConstInfo_dp1_global_var[] = 
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
    /*000*/ { "DES算法", NULL, NULL, 1, CT_NUM, NULL, 1 },
    /*001*/ { "RC4算法", NULL, NULL, 1, CT_NUM, NULL, 2 },
};
int g_ConstInfo_dp1_global_var_count = sizeof(g_ConstInfo_dp1_global_var) / sizeof(g_ConstInfo_dp1_global_var[0]);
#endif
