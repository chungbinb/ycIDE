#include "..\include_ecalc_header.h"

// 调用格式: SDT_BOOL (傅立叶变换).求能谱周期图, 命令说明: "计算抽样数据集的能谱周期图。返回每个频率箱的能谱密度，以及每个频率箱的精确频率(以Hz为单位)。特别说明：1.数据点的个数必须是2的幂，例如：32,64,128......,最大为8192。2.为保存内存。两个数组参数均被用于输入和输出。如果你想保留原始数据，则应把它拷贝到另一个变量中"
// 参数<1>: 数据集实值 数组 SDT_DOUBLE, 参数说明: "一维数组向量，假设本参数为xr[维数]数组，它包含xr中从xr[1]处起始的抽样数据的实值，并返回原始数据(从xr[1]到xr[numdat/2] - 1的有效数据)的能谱"
// 参数<2>: 数据集虚值 数组 SDT_DOUBLE, 参数说明: "该数组的维数、行列数是和前一个参数一样的数组，假设该参数为yi[维数]数组，它包含从yi中yi[0]起始的抽样数据的虚值，并返回能谱调和函数(从yi[1]到yi[numdat/2] - 1的有效数据)的频率值"
// 参数<3>: 时间间隔 SDT_DOUBLE, 参数说明: "包含了前两个参数数据集中样本之间的时间间隔"
ECALC_EXTERN_C void ecalc_PowerSpectrumCalc_28_ecalc(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    void*    arg1 = pArgInf[1].m_pAryData;
    void*    arg2 = pArgInf[2].m_pAryData;
    double   arg3 = pArgInf[3].m_double;

}

