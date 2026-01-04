#include "..\include_ecalc_header.h"

// 调用格式: SDT_BOOL (傅立叶变换).求傅立叶反变换, 命令说明: "采用快速Fourier变换技术(FFT)计算由N个等距调和函数组成的数据集的Fourier反变换。特别说明：1.数据点的个数(nd)必须是2的幂，例如32, 64, 128,......最大为8192。2.为了保存内存，本方法前两个参数xr和yi被用于输入和输出 (记注:在内存中浮点数占4字节，双精度数占8字节，因此，一个1024个元素的向量要求4k-8k字节!)。如果你想保留原始数据，把他拷贝到另一个向量中"
// 参数<1>: 数据集实值 数组 SDT_DOUBLE, 参数说明: "该参数是维数为\"数据点数\"的向量。假设本参数是xr[维数]数组，抽样数据集的实值包含从xr[1]起始的数组xr中。它返回数组xr中FFT调和函数的余弦系数。其中,xr[1]保存了DC分量,xr[2]保存了第1个谐波,xr[3]保存了第2个谐波，依次类推。系数与采样数据集中数据点的个数有关。对每一调和项a(i) * cos(...)的实际余弦函数用numdat / 2 除"
// 参数<2>: 数据集虚值 数组 SDT_DOUBLE, 参数说明: "该参数是维数为\"数据点数\"的向量，数据集的假设本参数是yi[维数]数组虚值包含从yi[1]起始的yi中。它返回yi中FFT调和函数的正弦系数 - 其中yi[2]保存第1个谐波，yi[3]保存了第2个谐波，依次类推。系数与采样数据集中数据点个数有关。对调和项a(i)sin(...)的实际正弦系数用numdat / 2 除"
// 参数<3>: 数据点数 SDT_INT, 参数说明: "包含了在前两个参数中数据点的个数"
ECALC_EXTERN_C void ecalc_FFTInvCalc_25_ecalc(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    void*    arg1 = pArgInf[1].m_pAryData;
    void*    arg2 = pArgInf[2].m_pAryData;
    INT      arg3 = pArgInf[3].m_int;

}

