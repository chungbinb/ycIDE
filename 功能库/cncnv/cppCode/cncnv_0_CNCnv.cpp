#include "..\include_cncnv_header.h"

// 调用格式: SDT_TEXT 内码转换, 命令说明: "将指定汉字编码的文本转换为另外一种编码方式，其中的非汉字部分不受影响，返回转换结果文本"
// 参数<1>: 待转换文本 SDT_TEXT, 参数说明: "本参数用作提供需要转换的文本"
// 参数<2>: 转换方式 SDT_INT, 参数说明: "可以为以下常量值之一： 1、#GBK到BIG5； 2、#BIG5到GBK； 3、#GBK繁体到简体； 4、#GBK简体到繁体； 5、#BIG5到GBK简体； 6、#GBK到SJIS； 7、#SJIS到GBK"
CNCNV_EXTERN_C void cncnv_CNCnv_0_cncnv(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR    arg1 = pArgInf[0].m_pText;
    INT      arg2 = pArgInf[1].m_int;

}

