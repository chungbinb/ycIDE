#include "..\include_ecalc_header.h"

// 调用格式: SDT_BOOL (概要统计).统计数据概要, 命令说明: "对数据集的每一列计算概要统计 - 最大值、最小值、范围、总和、平均值、方差、标准偏差、众数和平均值的标准误差"
// 参数<1>: 原数据 数组 SDT_DOUBLE, 参数说明: "必须是二维数组"
// 参数<2>: 最小值 数组 SDT_DOUBLE, 参数说明: "该参数为宽度和\"原数据\"高度一样的一维数组，它返回\"原数据\"中每一列数据的最小值"
// 参数<3>: 最大值 数组 SDT_DOUBLE, 参数说明: "该参数为宽度和\"原数据\"高度一样的一维数组，它返回\"原数据\"中每一列数据的最大值"
// 参数<4>: 范围值 数组 SDT_DOUBLE, 参数说明: "该参数为宽度和\"原数据\"高度一样的一维数组，它返回\"原数据\"中每一列数据的最大值和最小值的差"
// 参数<5>: 总和 数组 SDT_DOUBLE, 参数说明: "该参数为宽度和\"原数据\"高度一样的一维数组，它返回\"原数据\"中每一列数据的和"
// 参数<6>: 平均值 数组 SDT_DOUBLE, 参数说明: "该参数为宽度和\"原数据\"高度一样的一维数组，它返回\"原数据\"中每一列数据的平均值"
// 参数<7>: 方差值 数组 SDT_DOUBLE, 参数说明: "该参数为宽度和\"原数据\"高度一样的一维数组，它返回\"原数据\"中每一列数据的方差值"
// 参数<8>: 标准偏差值 数组 SDT_DOUBLE, 参数说明: "该参数为宽度和\"原数据\"高度一样的一维数组，它返回\"原数据\"中每一列数据的标准偏差值"
// 参数<9>: 标准误差 数组 SDT_DOUBLE, 参数说明: "该参数为宽度和\"原数据\"高度一样的一维数组，它返回\"原数据\"中每一列数据平均值的标准误差"
// 参数<10>: 众数值 数组 SDT_DOUBLE, 参数说明: "该参数为宽度和\"原数据\"高度一样的一维数组，它返回\"原数据\"中每一列数据的众数值"
ECALC_EXTERN_C void ecalc_SummaryStats_32_ecalc(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    void*    arg1 = pArgInf[1].m_pAryData;
    void*    arg2 = pArgInf[2].m_pAryData;
    void*    arg3 = pArgInf[3].m_pAryData;
    void*    arg4 = pArgInf[4].m_pAryData;
    void*    arg5 = pArgInf[5].m_pAryData;
    void*    arg6 = pArgInf[6].m_pAryData;
    void*    arg7 = pArgInf[7].m_pAryData;
    void*    arg8 = pArgInf[8].m_pAryData;
    void*    arg9 = pArgInf[9].m_pAryData;
    void*    arg10 = pArgInf[10].m_pAryData;

}

