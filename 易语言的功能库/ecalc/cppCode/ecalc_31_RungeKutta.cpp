#include "..\include_ecalc_header.h"

// 调用格式: SDT_BOOL (微积分).一阶微分方程, 命令说明: "求解形式如下的一阶微分方程y' = f(x,y)"
// 参数<1>: x已知值 SDT_DOUBLE, 参数说明: "包含x的已知值"
// 参数<2>: y已知值 SDT_DOUBLE, 参数说明: "包含y的已知值"
// 参数<3>: 要求解的x SDT_DOUBLE, 参数说明: "包含要求解的x的值"
// 参数<4>: 允许误差 SDT_DOUBLE, 参数说明: "保存允许误差，若误差估计小于 (et * h) / 32，则步长(后一个参数)在函数中使用时加倍，若误差估计大于 (本参数 * 步长)时，则步长减半"
// 参数<5>: 初始步长 SDT_DOUBLE, 参数说明: "包含用从已知x值迭代到x的初始步长"
// 参数<6>: 最小步长 SDT_DOUBLE, 参数说明: "包含最小步长(若步长小于本参数，则函数退出)，本参数必须为大于0的数"
// 参数<7>: &y的估值 SDT_DOUBLE, 参数说明: "返回y的估值"
// 参数<8>: &状态 SDT_BOOL, 参数说明: "若x值已被求解，则返回真；若在解出步长前已小于最小步长，则返回假"
ECALC_EXTERN_C void ecalc_RungeKutta_31_ecalc(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    double   arg1 = pArgInf[1].m_double;
    double   arg2 = pArgInf[2].m_double;
    double   arg3 = pArgInf[3].m_double;
    double   arg4 = pArgInf[4].m_double;
    double   arg5 = pArgInf[5].m_double;
    double   arg6 = pArgInf[6].m_double;
    double*  arg7 = pArgInf[7].m_pDouble;
    BOOL*    arg8 = pArgInf[8].m_pBool;

}

