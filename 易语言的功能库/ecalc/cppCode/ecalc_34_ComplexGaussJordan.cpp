#include "..\include_ecalc_header.h"

// 本命令被隐藏, 原始名字 = "复数线性方程组"
// 调用格式: SDT_BOOL (联立方程).复数线性方程组, 命令说明: "本函数采用Gauss - Jordan方法求解复型线性方程系统。函数使用了一种把N个复数方程转换成2N个实数方法的算法，再利用标准GaussJordan函数求解这个新的方程组，产生2N个解。然后，实数解向量被转换回N个复数解矢量，原始复数系数矩阵的逆也返回给用户。返回的行列是一实值，它是临时的2N个方程组的行列式。这个行列式可用来确定这个函数是否可收敛到一个精确解"
// 参数<1>: x变量系数 数组 MAKELONG(0x01, 0), 参数说明: "二维数组。行表示方程的系数，列表示x变量的系数"
// 参数<2>: y向量 数组 MAKELONG(0x01, 0), 参数说明: "列数和第一个参数一样的一维数组"
// 参数<3>: x变量解 数组 SDT_DOUBLE, 参数说明: "列数和第一个参数一样的一维数组，它是保存复数x变量的解向量- [1]表示值x0,[2]表示值x1,依次类推"
// 参数<4>: x变量系数逆阵 数组 MAKELONG(0x01, 0), 参数说明: "行列数和第一个参数一样的二维数组，它保存x变量系数矩阵的逆阵"
// 参数<5>: &x矩阵行列式 SDT_DOUBLE, 参数说明: "返回x变量系数矩阵的总行列式"
ECALC_EXTERN_C void ecalc_ComplexGaussJordan_34_ecalc(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    void*    arg1 = pArgInf[1].m_pAryData;
    void*    arg2 = pArgInf[2].m_pAryData;
    void*    arg3 = pArgInf[3].m_pAryData;
    void*    arg4 = pArgInf[4].m_pAryData;
    double*  arg5 = pArgInf[5].m_pDouble;

}

