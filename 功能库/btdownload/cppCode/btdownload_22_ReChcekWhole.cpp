#include "..\include_btdownload_header.h"

// 调用格式: SDT_BOOL 重新检查完整性, 命令说明: "重新检查下载后文件的完整性。"
// 参数<1>: 发布文件名 SDT_TEXT, 参数说明: "本参数表示要检查的发布文件名(torrent)。"
// 参数<2>: 本地文件路径 SDT_TEXT, 参数说明: "本参数表示被下载文件的本地保存路径。"
// 参数<3>: 本地文件名 SDT_TEXT, 参数说明: "本参数表示被下载文件的本地文件名，如本参数为空则用发布文件中默认的文件名。"
// 参数<4>: &百分比 SDT_INT, 参数说明: "本参数传回下载了百分之几。"
// 参数<5>: &字节数 SDT_INT64, 参数说明: "本参数传回已写入硬盘的字节数。"
// 参数<6>: &任务内容 SDT_BIN, 参数说明: "本参数传回任务内容，可用作传入“增加新任务”方法的参数中，以避免效验文件。"
// 参数<7>: 日志 SDT_SUB_PTR, 参数说明: "本参数表示检查进度的日志回调函数，详见“其它日志”数据类型的“检查完整性中”成员。"
BTDOWNLOAD_EXTERN_C void btdownload_ReChcekWhole_22_btdownload(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR    arg1 = pArgInf[0].m_pText;
    LPSTR    arg2 = pArgInf[1].m_pText;
    LPSTR    arg3 = pArgInf[2].m_pText;
    PINT     arg4 = pArgInf[3].m_pInt;
    PINT64   arg5 = pArgInf[4].m_pInt64;
    LPBYTE*  arg6 = pArgInf[5].m_ppBin;
    DWORD    arg7 = pArgInf[6].m_dwSubCodeAdr;

}

