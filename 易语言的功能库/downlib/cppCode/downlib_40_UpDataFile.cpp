#include "..\include_downlib_header.h"

// 调用格式: SDT_BOOL (FTP上传对象).上传文件, 命令说明: "增加一个上传文件的任务,当前支持FTP协议。成功返回真，失败返回假"
// 参数<1>: 配置信息 MAKELONG(0x04, 0), 参数说明: "配置任务相关信息"
DOWNLIB_EXTERN_C void downlib_UpDataFile_40_downlib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    void*    arg1 = pArgInf[1].m_pCompoundData;

}

