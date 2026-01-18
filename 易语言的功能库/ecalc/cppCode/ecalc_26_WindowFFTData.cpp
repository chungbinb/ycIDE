#include "..\include_ecalc_header.h"

// 调用格式: SDT_BOOL (傅立叶变换).窗口傅立叶, 命令说明: "在谐波分析中使用窗口来减少与光谱泄露有关的不必要的影响，光谱泄露是抽样波形中不连续的结果。因为波形是对一有限时间长度的抽样，因此在波形的端点会产生不连续性。窗口是用于初始数据的加权函数，以减少与有限次观察区间有关的光谱泄露。窗口通常是用一个函数在样本区间中乘以数据，该函数在其中心处为1，在两个端点处为0。特别说明：1.数据点的个数必须是2的幂，即32,64,128......,最大为8192。2.为了保存内存，前两个方法参数均被用于输入和输出(记注：在内存中浮点数占4字节，双精度数占8字节，因此一个1024个元素的向量要求4k~8K字节！)。如果您想保留原始数据，请把它拷贝到另一个向量中"
// 参数<1>: 数据集实值 数组 SDT_DOUBLE, 参数说明: "本参数是列数为\"数据点数\"的向量，假设本参数为xr[维数]数组，它包含了从xr[1]处起始的在xr中抽样数据集的实值，并返回带有窗口的xr值"
// 参数<2>: 数据集虚值 数组 SDT_DOUBLE, 参数说明: "本参数是列数为\"数据点数\"的向量，假设本参数为yi[维数]数组，它包含了从yi[1]处起始的在yi中的抽样数据集的虚值，并返回带有窗口的yi值"
// 参数<3>: 数据点数 SDT_INT, 参数说明: "包含在前两个参数中的数据点个数"
// 参数<4>: 抽样数据窗口 SDT_INT, 参数说明: "包含应用于抽样数据的窗口，为以下常量之一：0、#矩形Window；1、#Parzen Window2、#Hanning Window；3、#Welch Window；4、#Hamming Window；5、#Exact Blackman Window"
ECALC_EXTERN_C void ecalc_WindowFFTData_26_ecalc(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    void*    arg1 = pArgInf[1].m_pAryData;
    void*    arg2 = pArgInf[2].m_pAryData;
    INT      arg3 = pArgInf[3].m_int;
    INT      arg4 = pArgInf[4].m_int;

}

