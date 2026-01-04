#include "..\include_btdownload_header.h"

// 调用格式: SDT_INT 制做发布文件, 命令说明: "制做发布文件(Torrent文件)。返回0表示成功，-1表示参数错误，-2表示编码出错，-3表示写文件失败, -4表示计算SHA1值失败。"
// 参数<1>: 文件类型 SDT_INT, 参数说明: "本参数表示要发布文件的类型，1表示目录，2表示文件。"
// 参数<2>: 名称 SDT_TEXT, 参数说明: "本参数表示目录名或文件名。"
// 参数<3>: 服务器地址 SDT_TEXT, 参数说明: "本参数表示Tracker服务器的地址。多个服务器之间用“;”分隔。"
// 参数<4>: 发布路径 SDT_TEXT, 参数说明: "本参数表示生成的发布文件的路径。"
// 参数<5>: 块大小 SDT_INT, 参数说明: "本参数表示每一块的大小。值可以为“块大小”数据类型中的常量值。"
// 参数<6>: 注释 SDT_TEXT, 参数说明: "本参数表示关于此发布文件的注释。"
// 参数<7>: [创建者 SDT_TEXT], 参数说明: "如果被省略，默认为“ebit”。"
BTDOWNLOAD_EXTERN_C void btdownload_MakeTorrentFile_14_btdownload(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[0].m_int;
    LPSTR    arg2 = pArgInf[1].m_pText;
    LPSTR    arg3 = pArgInf[2].m_pText;
    LPSTR    arg4 = pArgInf[3].m_pText;
    INT      arg5 = pArgInf[4].m_int;
    LPSTR    arg6 = pArgInf[5].m_pText;
    LPSTR    arg7 = pArgInf[6].m_pText;

}

