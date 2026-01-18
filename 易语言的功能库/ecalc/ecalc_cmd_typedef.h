#pragma once

#define __ECALC_NAME(_index, _name) __LIB2_FNE_NAME_LEFT(__E_FNENAME)##_##_name##_##_index##_

// 传递函数名和索引, 拼接成 定义库名_名字_序号_定义库名, 比如 ecalc_test_0_ecalc
#define ECALC_NAME(_index, _name) __LIB2_FNE_NAME_LEFT(__ECALC_NAME(_index, _name))__LIB2_FNE_NAME_LEFT(__E_FNENAME)

// 传递函数名和索引, 拼接成 "定义库名_名字_序号_定义库名", 比如 "ecalc_test_0_ecalc"
#define ECALC_NAME_STR(_index, _name) ______E_FNENAME(__ECALC_NAME(_index, _name))

// 这个宏定义了所有的命令, 以后需要命令名数组, 声明命令等, 都可以使用这个宏
// 下面为_MAKE宏参数列表, 除了第一个成员外, 其他都是 CMD_INFO 这个结构的成员
//  0 _index            = 命令索引, 会根据这个索引生成命令名, 这个索引也能让你更直观的看到这个命令是在命令数组里的下标
//  1 _szName           = 命令的中文名, 需要加双引号, 这个会显示在易语言支持库命令列表里
//  2 _szEgName         = 命令的英文名, 不能加双引号, 这个会显示在易语言支持库命令列表英文名字里, 会根据这个英文名字生成c++实现功能函数名, 这个也是静态库导出的符号名
//  3 _szExplain        = 命令详细解释, 需要加双引号, 这个会显示在易语言支持库命令列表的详细解释里
//  4 _shtCategory      = 全局命令的所属类别, 从1开始, 对象成员命令的此值为-1
//  5 _wState           = 标记, CT_开头常量, _CMD_OS(__OS_WIN) 表示支持win系统, _CMD_OS(OS_ALL) 表示支持所有系统, 如果返回数组或者变长参数, 需要或上对应标志位
//  6 _dtRetValType     = 返回值类型, 使用前注意转换HIWORD为0的内部数据类型值到程序中使用的数据类型值
//  7 _wReserved        = 保留字段, 填0
//  8 _shtUserLevel     = 命令的用户学习难度级别, LVL_SIMPLE=初级命令, LVL_SECONDARY=中级命令, LVL_HIGH=高级命令
//  9 _shtBitmapIndex   = 指定图像索引, 从1开始, 0表示无, 显示在支持库列表里的图标
// 10 _shtBitmapCount   = 图像数目(用作动画)
// 11 _nArgCount        = 命令的参数数目
// 12 _pBeginArgInfo    = 参数起始地址
#define ECALC_DEF(_MAKE) \
    _MAKE(  0, "复数构造", ComplexConstruction, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED | CT_IS_OBJ_CONSTURCT_CMD, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ecalc_global_var + 0)\
    _MAKE(  1, "复数析构", ComplexDestruction, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED | CT_IS_OBJ_FREE_CMD, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ecalc_global_var + 0)\
    _MAKE(  2, "复数相加", ComplexAdd, "求两个复数之和", -1, _CMD_OS(__OS_WIN), MAKELONG(0x01, 0), 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_ecalc_global_var + 0)\
    _MAKE(  3, "复数相减", ComplexSub, "求两个复数之差", -1, _CMD_OS(__OS_WIN), MAKELONG(0x01, 0), 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_ecalc_global_var + 2)\
    _MAKE(  4, "复数相乘", ComplexMul, "求两个复数乘积", -1, _CMD_OS(__OS_WIN), MAKELONG(0x01, 0), 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_ecalc_global_var + 4)\
    _MAKE(  5, "复数相除", ComplexDiv, "求两个负数除商", -1, _CMD_OS(__OS_WIN), MAKELONG(0x01, 0), 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_ecalc_global_var + 6)\
    _MAKE(  6, "求复数指数", ComplexExp, "计算以e为底，以复数为幂的值", -1, _CMD_OS(__OS_WIN), MAKELONG(0x01, 0), 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_ecalc_global_var + 8)\
    _MAKE(  7, "求复数长度", ComplexMag, "计算一个复数的长度", -1, _CMD_OS(__OS_WIN), SDT_DOUBLE, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_ecalc_global_var + 8)\
    _MAKE(  8, "求复数极角", ComplexAngle, "计算复数的极角", -1, _CMD_OS(__OS_WIN), SDT_DOUBLE, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_ecalc_global_var + 8)\
    _MAKE(  9, "取实数", GetRealComplex, "获取复数中的实数部分", -1, _CMD_OS(__OS_WIN), SDT_DOUBLE, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ecalc_global_var + 0)\
    _MAKE( 10, "取虚数", GetImaComplex, "获取复数中的虚数部分", -1, _CMD_OS(__OS_WIN), SDT_DOUBLE, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ecalc_global_var + 0)\
    _MAKE( 11, "置实数", SetRealComplex, "设置复数的实数部分", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_ecalc_global_var + 9)\
    _MAKE( 12, "置虚数", SetImaComplex, "设置复数的虚数部分", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_ecalc_global_var + 9)\
    _MAKE( 13, "矩阵相乘", MatrixMul, "计算两个矩阵的乘积，注意：必须保证矩阵1的列数等于矩阵2的行数，如果参数错误或者计算失败，本方法将返回一个1维宽度为1的数组", -1, _CMD_OS(__OS_WIN) | CT_RETRUN_ARY_TYPE_DATA, SDT_DOUBLE, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_ecalc_global_var + 10)\
    _MAKE( 14, "复矩阵相乘", CMatrixMul, "计算两个复矩阵的乘积，注意：必须保证矩阵1的列数等于矩阵2的行数，如果参数错误或者计算失败，本方法将返回一个1维宽度为1的数组", -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED | CT_RETRUN_ARY_TYPE_DATA, MAKELONG(0x01, 0), 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_ecalc_global_var + 12)\
    _MAKE( 15, "矩阵相加", MatrixAdd, "求两个二维矩阵之和，两个矩阵的宽度和高度必须相等，如果参数错误或者计算失败，本方法将返回一个1维宽度为1的数组", -1, _CMD_OS(__OS_WIN) | CT_RETRUN_ARY_TYPE_DATA, SDT_DOUBLE, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_ecalc_global_var + 14)\
    _MAKE( 16, "复矩阵相加", CMatrixMul, "计算两个复矩阵之和", -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED | CT_RETRUN_ARY_TYPE_DATA, MAKELONG(0x01, 0), 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_ecalc_global_var + 16)\
    _MAKE( 17, "实矩阵逆", MatrixInvert, "计算一个实矩阵的逆。如果参数错误或者计算失败，本方法将返回一个大小为 1 的一维数组。不是所有矩阵都有逆矩阵。", -1, _CMD_OS(__OS_WIN) | CT_RETRUN_ARY_TYPE_DATA, SDT_DOUBLE, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_ecalc_global_var + 18)\
    _MAKE( 18, "复矩阵逆", CMatrixInvert, "计算复矩阵的逆阵。采用的算法是把N x N复矩阵转换成2N x 2N的实数矩阵。标准Gauss Jordan函数被用于计算2N x 2N数组的逆。这个数组的逆又被变换回所要求的复数逆数组", -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED | CT_RETRUN_ARY_TYPE_DATA, MAKELONG(0x01, 0), 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_ecalc_global_var + 20)\
    _MAKE( 19, "矩阵转置", MatrixTranspose, "计算矩阵的转置,如果参数错误或者计算失败，本方法将返回一个1维宽度为1的数组", -1, _CMD_OS(__OS_WIN) | CT_RETRUN_ARY_TYPE_DATA, SDT_DOUBLE, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_ecalc_global_var + 22)\
    _MAKE( 20, "复矩阵转置", CMatrixTranspose, "计算一个复矩阵的转置", -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED | CT_RETRUN_ARY_TYPE_DATA, MAKELONG(0x01, 0), 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_ecalc_global_var + 23)\
    _MAKE( 21, "求矩阵特征", CyclicJacobi, "用循环Jacobi算法计算一个实对称矩阵的特征值", -1, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 5, g_argumentInfo_ecalc_global_var + 24)\
    _MAKE( 22, "矩阵标量乘积", MatrixScalarMul, "计算矩阵和一个标量值的乘积,如果参数错误或者计算失败，本方法将返回一个1维宽度为1的数组", -1, _CMD_OS(__OS_WIN) | CT_RETRUN_ARY_TYPE_DATA, SDT_DOUBLE, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_ecalc_global_var + 29)\
    _MAKE( 23, "复矩阵标量乘积", CMatrixScalarMul, "计算矩阵和一个复数的乘积", -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED | CT_RETRUN_ARY_TYPE_DATA, MAKELONG(0x01, 0), 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_ecalc_global_var + 31)\
    _MAKE( 24, "求傅立叶变换", FFTCalc, "采用快速Fourier变换技术(FFT)计算由N个等距样本组成的数据集的Fourier变换。特别说明：1.数据点的个数必须是2的幂，例如32,64,128......,最大为81922.为了保存内存，第一个参数和第二个参数用于输入和输出（记住：在内存中浮点数占4个字节，双精度数占8个字节，因此一个1024个元素的向量要求4K - 8K的字节!)。如果你想保存原始数据，则把他拷贝到另一个向量中。3.把调和转换成频率:为了确定一个波形的主要频率成分，通常需要在一个调和信号上运行FFT。为了从调和数据中计算频率，你需对原始数据集知道样本频率(或两个样本间的周期)。谐波频率可用下述公式计算：频率 = (谐波下标) / (nd_数据点个数 * 样本周期)", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_ecalc_global_var + 33)\
    _MAKE( 25, "求傅立叶反变换", FFTInvCalc, "采用快速Fourier变换技术(FFT)计算由N个等距调和函数组成的数据集的Fourier反变换。特别说明：1.数据点的个数(nd)必须是2的幂，例如32, 64, 128,......最大为8192。2.为了保存内存，本方法前两个参数xr和yi被用于输入和输出 (记注:在内存中浮点数占4字节，双精度数占8字节，因此，一个1024个元素的向量要求4k-8k字节!)。如果你想保留原始数据，把他拷贝到另一个向量中", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_ecalc_global_var + 36)\
    _MAKE( 26, "窗口傅立叶", WindowFFTData, "在谐波分析中使用窗口来减少与光谱泄露有关的不必要的影响，光谱泄露是抽样波形中不连续的结果。因为波形是对一有限时间长度的抽样，因此在波形的端点会产生不连续性。窗口是用于初始数据的加权函数，以减少与有限次观察区间有关的光谱泄露。窗口通常是用一个函数在样本区间中乘以数据，该函数在其中心处为1，在两个端点处为0。特别说明：1.数据点的个数必须是2的幂，即32,64,128......,最大为8192。2.为了保存内存，前两个方法参数均被用于输入和输出(记注：在内存中浮点数占4字节，双精度数占8字节，因此一个1024个元素的向量要求4k~8K字节！)。如果您想保留原始数据，请把它拷贝到另一个向量中", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 4, g_argumentInfo_ecalc_global_var + 39)\
    _MAKE( 27, "二维傅立叶变换", FFT2DCalc, "采用快速Fourier变换技术(FFT)计算由C x R个等距样本组成的数据集的二维Fourier变换。特别说明：1.行和列(c和r)必须是2的幂，例如2,4,8......128。2.为了保存内存，两个参数数组均被用于输入和输出(记住：在内存中浮点数占4字节，双精度数占8字节，因此，一个有1024个元素的向量要求4K~8K字节!)。如果你想要保留原始数据，把它拷贝到另一个变量中。3.二维FFT占用很多内存。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_ecalc_global_var + 43)\
    _MAKE( 28, "求能谱周期图", PowerSpectrumCalc, "计算抽样数据集的能谱周期图。返回每个频率箱的能谱密度，以及每个频率箱的精确频率(以Hz为单位)。特别说明：1.数据点的个数必须是2的幂，例如：32,64,128......,最大为8192。2.为保存内存。两个数组参数均被用于输入和输出。如果你想保留原始数据，则应把它拷贝到另一个变量中", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_ecalc_global_var + 46)\
    _MAKE( 29, "离散化数值积分", IntegrateVector, "计算一个离散化的抽样数据集的数值积分(即通常曲线下的面积)。采用的算法是Simpson 1/3法则和Simpson 3/8法则，与其他方法不同，这个方法可以同时处理偶数次和奇数次观察的数据集", -1, _CMD_OS(__OS_WIN), SDT_DOUBLE, 0, LVL_SIMPLE, 0, 0, 4, g_argumentInfo_ecalc_global_var + 49)\
    _MAKE( 30, "函数数值积分", IntegrateFunction, "计算一个所指定的函数的数值积分(即通常曲线下的面积)。采用的算法是Simpson 1/3法则和Simpson 3/8 法则", -1, _CMD_OS(__OS_WIN), SDT_DOUBLE, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_ecalc_global_var + 53)\
    _MAKE( 31, "一阶微分方程", RungeKutta, "求解形式如下的一阶微分方程y' = f(x,y)", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 8, g_argumentInfo_ecalc_global_var + 56)\
    _MAKE( 32, "统计数据概要", SummaryStats, "对数据集的每一列计算概要统计 - 最大值、最小值、范围、总和、平均值、方差、标准偏差、众数和平均值的标准误差", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 10, g_argumentInfo_ecalc_global_var + 64)\
    _MAKE( 33, "线性方程组", GaussJordan, "本函数采用Gauss - Jordan方法求解实型线性方程组", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 5, g_argumentInfo_ecalc_global_var + 74)\
    _MAKE( 34, "复数线性方程组", ComplexGaussJordan, "本函数采用Gauss - Jordan方法求解复型线性方程系统。函数使用了一种把N个复数方程转换成2N个实数方法的算法，再利用标准GaussJordan函数求解这个新的方程组，产生2N个解。然后，实数解向量被转换回N个复数解矢量，原始复数系数矩阵的逆也返回给用户。返回的行列是一实值，它是临时的2N个方程组的行列式。这个行列式可用来确定这个函数是否可收敛到一个精确解", -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 5, g_argumentInfo_ecalc_global_var + 79)\
    _MAKE( 35, "多重回归", MulReg, "对数据集建立一个最小二乘线性多重回归方程。它返回回归方程的系数、y的估值、残差、估计的标准误差、回归系数的标准误差、确定系数和相关系数", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 10, g_argumentInfo_ecalc_global_var + 84)\
    _MAKE( 36, "二乘曲线拟合", PolyCurveFit, "本函数是一个广义最小二乘曲线拟合函数，它对一个相关或不相关变量数据集用线性系数拟合一个多项式", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 10, g_argumentInfo_ecalc_global_var + 94)\
    _MAKE( 37, "三次多项式方程", CubicSplines, "本函数对离散数据集拟合一系列三次多项式方程。通过把原始数据点区间上的三次方程应用于插值中要使用的x值，在那个区间上的y值可能被插值。在每个子区间上(sv[i],sv[i+1])，三次样条形如：y的估值 = a[i] + b[i] * (x - xv[i]) + c[i] * (x - sv[i]) ^ 2 + d[i] * (s - sv[i]) ^ 3", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_ecalc_global_var + 104)\
    _MAKE( 38, "三次样条插值", CalcSpline, "该函数计算给定一个x值和由\"三次多项式方程\"计算的三次样条系数矩阵，y值的三次样条插值", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 4, g_argumentInfo_ecalc_global_var + 107)\
    _MAKE( 39, "求双曲正切", Tanh, NULL, -1, _CMD_OS(__OS_WIN), SDT_DOUBLE, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_ecalc_global_var + 111)\
    _MAKE( 40, "求双曲余弦", Cosh, NULL, -1, _CMD_OS(__OS_WIN), SDT_DOUBLE, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_ecalc_global_var + 111)\
    _MAKE( 41, "求双曲正弦", Sinh, NULL, -1, _CMD_OS(__OS_WIN), SDT_DOUBLE, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_ecalc_global_var + 111)\
    _MAKE( 42, "求双曲正割", Sech, NULL, -1, _CMD_OS(__OS_WIN), SDT_DOUBLE, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_ecalc_global_var + 111)\
    _MAKE( 43, "求反双曲正切", ArcTanh, NULL, -1, _CMD_OS(__OS_WIN), SDT_DOUBLE, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_ecalc_global_var + 111)\
    _MAKE( 44, "求伽玛", Gamma, "求指定数的伽玛函数", -1, _CMD_OS(__OS_WIN), SDT_DOUBLE, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_ecalc_global_var + 112)\
    _MAKE( 45, "求伽玛对数", LogGamma, "求指定数的伽玛函数的对数", -1, _CMD_OS(__OS_WIN), SDT_DOUBLE, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_ecalc_global_var + 112)\
    _MAKE( 46, "求不完全伽玛", IncGamma, "求指定数的不完全伽玛函数，本函数是定义在所有大于0的实数上的", -1, _CMD_OS(__OS_WIN), SDT_DOUBLE, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_ecalc_global_var + 114)\
    _MAKE( 47, "求不完全伽玛2", IncGammaComp, "本函数是定义在所有大于0的实数上的", -1, _CMD_OS(__OS_WIN), SDT_DOUBLE, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_ecalc_global_var + 116)\
    _MAKE( 48, "求贝它", Beta, "根据指定参数求贝它", -1, _CMD_OS(__OS_WIN), SDT_DOUBLE, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_ecalc_global_var + 118)\
    _MAKE( 49, "求不完全贝它", IncBeta, "根据指定参数求贝它", -1, _CMD_OS(__OS_WIN), SDT_DOUBLE, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_ecalc_global_var + 121)\
    _MAKE( 50, "求贝塞尔", Bessel, "通过指定参数求贝塞尔", -1, _CMD_OS(__OS_WIN), SDT_DOUBLE, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_ecalc_global_var + 124)\
    _MAKE( 51, "求修正贝塞尔", ModBessel, "通过指定参数求修正过的贝塞尔值", -1, _CMD_OS(__OS_WIN), SDT_DOUBLE, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_ecalc_global_var + 127)\
    _MAKE( 52, "求修正贝塞尔R", ModBesselR, "通过指定参数求修正过的贝塞尔值", -1, _CMD_OS(__OS_WIN), SDT_DOUBLE, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_ecalc_global_var + 130)\
    _MAKE( 53, "求高斯误差", ErrFunc, "求指定参数的高斯误差值", -1, _CMD_OS(__OS_WIN), SDT_DOUBLE, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_ecalc_global_var + 133)\
    _MAKE( 54, "求误差余", ErrFuncCom, "求解误差函数的余函数", -1, _CMD_OS(__OS_WIN), SDT_DOUBLE, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_ecalc_global_var + 135)\
    _MAKE( 55, "求误差实部", ErrFuncR, "计算误差函数Z = x + iy 的实部", -1, _CMD_OS(__OS_WIN), SDT_DOUBLE, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_ecalc_global_var + 137)\
    _MAKE( 56, "求误差虚部", ErrFuncI, "计算误差函数Z = x + iy的虚部", -1, _CMD_OS(__OS_WIN), SDT_DOUBLE, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_ecalc_global_var + 140)\
    _MAKE( 57, "求厄密多项式", Hermite, "求解Hermite方程中多项式的第n个根", -1, _CMD_OS(__OS_WIN), SDT_DOUBLE, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_ecalc_global_var + 142)\
    _MAKE( 58, "求勒让德方程系数", Legend, "求Gauss Legendre方程系数", -1, _CMD_OS(__OS_WIN), SDT_DOUBLE, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_ecalc_global_var + 145)\
    _MAKE( 59, "求拉格尔", Laguerre, "本函数是在下列方程中寻找多项式第n个根的向前方法:Ln(a)(x) = xy'' + (a + 1 - x)y' + ny = 0", -1, _CMD_OS(__OS_WIN), SDT_DOUBLE, 0, LVL_SIMPLE, 0, 0, 4, g_argumentInfo_ecalc_global_var + 148)\
    _MAKE( 60, "求雅各比方程", Jacobi, "求解雅各比方程。由于雅各比方程中包含特殊符号，因此方程原型无法在此显示，对此表示抱歉", -1, _CMD_OS(__OS_WIN), SDT_DOUBLE, 0, LVL_SIMPLE, 0, 0, 5, g_argumentInfo_ecalc_global_var + 152)\
    _MAKE( 61, "求车切多项式", Tcheb, "求解n阶的Tchebychev的多项式: Tn(x) = (1 - x ^ 2)y' - xy + n ^ 2 * y = 0", -1, _CMD_OS(__OS_WIN), SDT_DOUBLE, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_ecalc_global_var + 157)\
    _MAKE( 62, "大数构造", UnlNumConstruction, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED | CT_IS_OBJ_CONSTURCT_CMD, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ecalc_global_var + 0)\
    _MAKE( 63, "大数析构", UnlNumDestruction, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED | CT_IS_OBJ_FREE_CMD, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ecalc_global_var + 0)\
    _MAKE( 64, "销毁", Destroy, "删除本对象已存在的数据，使用本方法后，本对象的数据将被变成整数\"0\"", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ecalc_global_var + 0)\
    _MAKE( 65, "导入文本文件", ImportTxtFile, "导入存储数字的文本文件，该文本文件必须仅为存储数字的格式，否则本方法将失败，并返回假，本方法成功返回真，本对象以前存储的数据将被舍弃", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_ecalc_global_var + 160)\
    _MAKE( 66, "导出文本文件", ImportTxtFile, "导出本对象当前所存储的数字到文本文件，成功返回真，失败返回假", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_ecalc_global_var + 162)\
    _MAKE( 67, "导入文本", ImportString, "导入存储数字的文本型的数据为本对象的当前数据，该文本型必须为存储合法数字格式的文本，该数字可以是任何长度和精度，本对象以前存储的数据将被舍弃，成功返回真，失败返回假", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_ecalc_global_var + 164)\
    _MAKE( 68, "导出文本", ExportString, "以文本型的格式导出当前本对象所存储的数字数据", -1, _CMD_OS(__OS_WIN), SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ecalc_global_var + 0)\
    _MAKE( 69, "导入数字", ImportNum, "导入当前易语言支持的所有格式的数字为当前数据类型的数据，以前的数据将被舍弃，成功返回真，失败返回假，注意：如果您导入的数据是小数有可能产生误差", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_ecalc_global_var + 165)\
    _MAKE( 70, "取类型", GetNumType, "计算并获取当前本对象所对应易语言的基础数据类型，便于导出数据到易语言基础数字数据类型，返回值为以下常量值之一：0、#非任何类型；1、#字节型；2、#短整数型；3、#整数型；4、#长整数型；5、#小数型；6、#双精度小数型", -1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ecalc_global_var + 0)\
    _MAKE( 71, "取长度", GetStrLen, "计算并获取本对象如果转换为文本型数据时，字符的长度", -1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ecalc_global_var + 0)\
    _MAKE( 72, "取整数长度", GetIntStrLen, "计算并获取本对象如果转换为文本型数据时，整数部分字符的长度", -1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ecalc_global_var + 0)\
    _MAKE( 73, "取小数长度", GetFloatStrLen, "计算并获取本对象如果转换为文本型数据时，小数部分字符的长度，注意：尾端0将被忽略", -1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ecalc_global_var + 0)\
    _MAKE( 74, "取字符", GetChar, "获取本对象所表示的数字文本在任意位置的数字字符，指定的序号参数必须大于等于1且小于等于字符串长度", -1, _CMD_OS(__OS_WIN), SDT_BYTE, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_ecalc_global_var + 166)\
    _MAKE( 75, "导出字节", ExportByte, "导出本对象到字节数据，注意：本对象当前所存储的数据必须是吻合字节型的数据", -1, _CMD_OS(__OS_WIN), SDT_BYTE, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ecalc_global_var + 0)\
    _MAKE( 76, "导出短整数", ExportShort, "导出本对象到短整型数据，注意：本对象当前所存储的数据必须是吻合短整型的数据", -1, _CMD_OS(__OS_WIN), SDT_SHORT, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ecalc_global_var + 0)\
    _MAKE( 77, "导出整数", ExportInt, "导出本对象到整数型数据，注意：本对象当前所存储的数据必须是吻合整数型的数据", -1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ecalc_global_var + 0)\
    _MAKE( 78, "导出长整数", ExportInt64, "导出本对象到长整型数据，注意：本对象当前所存储的数据必须是吻合长整型的数据", -1, _CMD_OS(__OS_WIN), SDT_INT64, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ecalc_global_var + 0)\
    _MAKE( 79, "导出小数", ExportFloat, "导出本对象到小数型数据，注意：本对象当前所存储的数据必须是吻合小数型的数据", -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, SDT_FLOAT, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ecalc_global_var + 0)\
    _MAKE( 80, "导出双精度数", ExportDouble, "导出本对象到双精度型数据，注意：本对象当前所存储的数据必须是吻合双精度型的数据", -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, SDT_DOUBLE, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ecalc_global_var + 0)\
    _MAKE( 81, "复制到", CopyTo, "复制当前数据到另一个目标存储大数的对象中，目标大数以前所存储的数据将被舍弃", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_ecalc_global_var + 167)\
    _MAKE( 82, "取符号", GetSignal, "获取本对象所表示的符号，如果本对象表示正数或0，返回真，如果表示负数，则返回假", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ecalc_global_var + 0)\
    _MAKE( 83, "是否有小数", HasFloat, "判断本对象是否有存储小数，有则返回真，否则返回假", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ecalc_global_var + 0)\
    _MAKE( 84, "是否为零", IsZero, "判断本对象当前所存储的数据是否为\"0\"，如果为\"0\"，则返回真；否则，返回假", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ecalc_global_var + 0)\
    _MAKE( 85, "等于", EqualThan, "判断本对象和指定对象是否相等。如相等，则返回真；否则，返回假", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_ecalc_global_var + 168)\
    _MAKE( 86, "大于", BiggerThan, "判断本对象是否大于指定对象。如大于，则返回真；否则，返回假", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_ecalc_global_var + 168)\
    _MAKE( 87, "小于", SmallerThan, "判断本对象是否小于指定对象。如小于，则返回真；否则，返回假", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_ecalc_global_var + 168)\
    _MAKE( 88, "大于等于", BiggerEqualThan, "判断本对象是否大于等于指定对象。如大于等于，则返回真；否则，返回假", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_ecalc_global_var + 168)\
    _MAKE( 89, "小于等于", SmallerEqualThan, "判断本对象是否小于等于指定对象。如小于等于，则返回真；否则，返回假", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_ecalc_global_var + 168)\
    _MAKE( 90, "取整数", GetIntPart, "获取本对象的整数部分，小数部分将被忽略，注意：返回数据的符号和原数据一致", -1, _CMD_OS(__OS_WIN), MAKELONG(0x0A, 0), 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ecalc_global_var + 0)\
    _MAKE( 91, "取小数", GetFloatPart, "获取本对象的小数部分，整数部分将被忽略， 注意：返回数据的符号和原数据一致", -1, _CMD_OS(__OS_WIN), MAKELONG(0x0A, 0), 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ecalc_global_var + 0)\
    _MAKE( 92, "加", Add, "将本对象与指定对象做加法运算，并返回运算后的结果", -1, _CMD_OS(__OS_WIN), MAKELONG(0x0A, 0), 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_ecalc_global_var + 169)\
    _MAKE( 93, "减", Sub, "将本对象与指定对象做减法运算，并返回运算后的结果", -1, _CMD_OS(__OS_WIN), MAKELONG(0x0A, 0), 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_ecalc_global_var + 169)\
    _MAKE( 94, "乘", Mul, "将本对象与指定对象做乘法运算，并返回运算后的结果", -1, _CMD_OS(__OS_WIN), MAKELONG(0x0A, 0), 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_ecalc_global_var + 169)\
    _MAKE( 95, "除", Div, "将本对象与指定对象做除法运算，并返回运算后的结果", -1, _CMD_OS(__OS_WIN), MAKELONG(0x0A, 0), 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_ecalc_global_var + 170)\
    _MAKE( 96, "整除", DivExact, "将本对象与指定对象做整除运算，并返回运算后的结果", -1, _CMD_OS(__OS_WIN), MAKELONG(0x0A, 0), 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_ecalc_global_var + 169)\
    _MAKE( 97, "求余", Mod, "将本对象与指定对象做求余运算，并返回运算后的结果", -1, _CMD_OS(__OS_WIN), MAKELONG(0x0A, 0), 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_ecalc_global_var + 169)\
    _MAKE( 98, "求次方", Pow, "计算并返回本对象的指定次方", -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, MAKELONG(0x0A, 0), 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_ecalc_global_var + 172)\
    _MAKE( 99, "取绝对值", Abs, "取本对象的绝对值", -1, _CMD_OS(__OS_WIN), MAKELONG(0x0A, 0), 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ecalc_global_var + 0)\
    _MAKE(100, "负", Neg, "反转一个本对象的正负符号并返回反转后的值", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ecalc_global_var + 0)\
    _MAKE(101, "四舍五入", Round, "返回本对象按照指定的方式进行四舍五入运算的结果", -1, _CMD_OS(__OS_WIN), MAKELONG(0x0A, 0), 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_ecalc_global_var + 173)\
    _MAKE(102, "清除小数", KillFloatNum, "清除本对象的小数部分，成功返回真，失败返回假", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ecalc_global_var + 0)\
    _MAKE(103, "清除整数", KillIntNum, "清除本对象的整数部分，整数部分将被重置为0，成功返回真，失败返回假", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ecalc_global_var + 0)\
    _MAKE(104, "清除尾导零", KillBackZero, "清除本对象小数部分的尾段0，成功返回真，失败返回假", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ecalc_global_var + 0)\
    _MAKE(105, "求正弦", Sin, "反回本对象的正弦值，目前支持计算整数的正弦值，小数部分将被忽略", -1, _CMD_OS(__OS_WIN), MAKELONG(0x0A, 0), 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_ecalc_global_var + 174)\
    _MAKE(106, "求余弦", Cos, "反回本对象的余弦值，目前支持计算整数的余弦值，小数部分将被忽略", -1, _CMD_OS(__OS_WIN), MAKELONG(0x0A, 0), 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_ecalc_global_var + 174)\
    _MAKE(107, "求正切", Tan, "反回本对象的正切值， 目前支持计算整数的正切值，小数部分将被忽略", -1, _CMD_OS(__OS_WIN), MAKELONG(0x0A, 0), 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_ecalc_global_var + 174)\
    _MAKE(108, "求余切", Atn, "反回本对象的余切值， 目前支持计算整数的余切值，小数部分将被忽略", -1, _CMD_OS(__OS_WIN), MAKELONG(0x0A, 0), 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_ecalc_global_var + 174)\
    _MAKE(109, "求积分表达式", CalcIntegrateExpression, "本方法可求各种复杂的积分表达式，可含任何数学函数，如  3*exp(x)*tan(x)/(8+log(x))，并且可以检查出表达式的错误，如果积分表达式有误或计算失败，本方法将返回假，计算成功本方法返回真", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 5, g_argumentInfo_ecalc_global_var + 175)\
    _MAKE(110, "大数复制构造", UnlNumCopyCmd, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED | CT_IS_OBJ_COPY_CMD, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_ecalc_global_var + 180)\
    _MAKE(111, "复数复制构造", ComplexCopyCmd, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED | CT_IS_OBJ_COPY_CMD, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_ecalc_global_var + 181)

