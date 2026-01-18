#include "..\include_internet_header.h"

// 调用格式: SDT_TEXT 发送邮件, 命令说明: "发送邮件到指定信箱，注意邮件中包含所有使用“添加附件文件”和“添加附件数据”命令添加的附件。成功返回空文本，失败返回具体错误信息文本"
// 参数<1>: 主题 SDT_TEXT, 参数说明: NULL
// 参数<2>: 正文 SDT_TEXT, 参数说明: NULL
// 参数<3>: 收信人邮件地址 SDT_TEXT, 参数说明: "可以为地址列表并包括友好名称，示例如：“客服〈service@dywt.com.cn〉;售前〈sale@dywt.com.cn〉” 或：“service@dywt.com.cn;sale@dywt.com.cn”都可以，多地址之间用逗号或分号分隔，友好名称后的邮件地址用半角尖括号括住"
// 参数<4>: [抄送邮件地址 SDT_TEXT], 参数说明: "地址格式等同于“收信人邮件地址”，如果本参数被省略，默认为空文本"
// 参数<5>: [暗送邮件地址 SDT_TEXT], 参数说明: "地址格式等同于“收信人邮件地址”，如果本参数被省略，默认为空文本"
// 参数<6>: 发信人邮件地址 SDT_TEXT, 参数说明: "只能为单个邮件地址，可以包括友好名称，示例如：“客服〈service@dywt.com.cn〉”或“service@dywt.com.cn”"
// 参数<7>: [回复邮件地址 SDT_TEXT], 参数说明: "指定收件人回复此邮件的地址。如果被省略，默认为与发信人邮件地址相同"
INTERNET_EXTERN_C void internet_SendMail_5_internet(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR    arg1 = pArgInf[0].m_pText;
    LPSTR    arg2 = pArgInf[1].m_pText;
    LPSTR    arg3 = pArgInf[2].m_pText;
    LPSTR    arg4 = pArgInf[3].m_pText;
    LPSTR    arg5 = pArgInf[4].m_pText;
    LPSTR    arg6 = pArgInf[5].m_pText;
    LPSTR    arg7 = pArgInf[6].m_pText;

}

