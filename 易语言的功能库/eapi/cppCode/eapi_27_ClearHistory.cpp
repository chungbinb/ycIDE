#include "..\include_eapi_header.h"

// 调用格式: SDT_BOOL 清除历史记录, 命令说明: "清除指定类型的历史记录。成功返回真；失败返回假。"
// 参数<1>: [历史记录类型 SDT_INT], 参数说明: "可以被省略。历史记录类型，可以为以下常量值之一：0、#接口常量.浏览器历史地址记录；1、#接口常量.表单自动完成历史记录；2、#接口常量.自动密码历史记录；3、#接口常量.收藏夹；4、#接口常量.自动拨号历史记录；5、#接口常量.运行中的自动匹配历史记录；6、#接口常量.打开文档历史记录；7、#接口常量.网络连接历史记录；8、#接口常量.远程登录历史记录；9、#接口常量.浏览网址历史记录。"
EAPI_EXTERN_C void eapi_ClearHistory_27_eapi(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[0].m_int;

}

