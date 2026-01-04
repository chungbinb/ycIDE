#include "..\include_ecalc_header.h"

// 调用格式: SDT_BOOL (傅立叶变换).二维傅立叶变换, 命令说明: "采用快速Fourier变换技术(FFT)计算由C x R个等距样本组成的数据集的二维Fourier变换。特别说明：1.行和列(c和r)必须是2的幂，例如2,4,8......128。2.为了保存内存，两个参数数组均被用于输入和输出(记住：在内存中浮点数占4字节，双精度数占8字节，因此，一个有1024个元素的向量要求4K~8K字节!)。如果你想要保留原始数据，把它拷贝到另一个变量中。3.二维FFT占用很多内存。"
// 参数<1>: 数据集实值 数组 SDT_DOUBLE, 参数说明: "该参数为二维数组，它包含了从数组起始数据中的抽样数据的实值，并返回FFT调和函数的余弦系数"
// 参数<2>: 数据集虚值 数组 SDT_DOUBLE, 参数说明: "该数组的维数、行列数是和前一个参数一样的数组，它包含了从数组起始到本数组中的抽样数据的虚值，并返回FFT调和系数的正弦余数"
// 参数<3>: 标志 SDT_BOOL, 参数说明: "真表示反变换，假表示正变换"
ECALC_EXTERN_C void ecalc_FFT2DCalc_27_ecalc(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    void*    arg1 = pArgInf[1].m_pAryData;
    void*    arg2 = pArgInf[2].m_pAryData;
    BOOL     arg3 = pArgInf[3].m_bool;

}

