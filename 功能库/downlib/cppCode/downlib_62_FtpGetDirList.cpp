#include "..\include_downlib_header.h"

// 调用格式: SDT_INT (FTP辅助对象).取目录列表, 命令说明: "返回FTP服务器上指定目录内的所有匹配文件和子目录信息。成功返回被找到的文件和子目录的数目，失败返回 -1失败的原因有可能是FTP服务器的目录结构暂时不被支持"
// 参数<1>: [欲列出的文件 SDT_TEXT], 参数说明: "可以包含路径，可以使用多字符 (*) 和单字符 (?) 通配符来指定多重文件及目录"
// 参数<2>: [&存放文件名的数组变量 数组/非数组 SDT_TEXT], 参数说明: "提供参数数据时只能提供变量数组。在命令执行完毕后，本变量数组内被顺序填入所找到的匹配文件和子目录名称。变量数组内原有数据被全部销毁，变量数组的维数被自动调整为所找到的文件数目"
// 参数<3>: [&存放文件属性的数组变量 数组/非数组 SDT_INT], 参数说明: "当前不被支持,留做以后扩展"
// 参数<4>: [&存放文件尺寸的数组变量 数组/非数组 SDT_INT64], 参数说明: "提供参数数据时只能提供变量数组。在命令执行完毕后，本变量数组内被顺序填入所找到的匹配文件的尺寸，并与文件名数组成员一一对应"
// 参数<5>: [&存放文件时间的数组变量 数组/非数组 SDT_DATE_TIME], 参数说明: "提供参数数据时只能提供变量数组。在命令执行完毕后，本变量数组内被顺序填入所找到的匹配文件的最后修改时间，并与文件名数组成员一一对应。变量数组内原有数据被全部销毁，变量数组的维数被自动调整为所找到的文件数目"
DOWNLIB_EXTERN_C void downlib_FtpGetDirList_62_downlib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR    arg1 = pArgInf[1].m_pText;
    void**   arg2 = pArgInf[2].m_ppAryData;
    void**   arg3 = pArgInf[3].m_ppAryData;
    void**   arg4 = pArgInf[4].m_ppAryData;
    void**   arg5 = pArgInf[5].m_ppAryData;

}

