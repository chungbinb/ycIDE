#include "..\include_opengl_header.h"

// 调用格式: SDT_INT 进入选择模式, 命令说明: "进入选择模式后画出的任何图元都不会显示出来。只有在选择模式下对模型进行命名的操作才有效。成功后将所有在选择区域中包含的命中记录存放在数组参数中。并且返回命中记录的个数。否则清空数组，并且返回0。\r\n\t选择缓存内容的排列规则：\r\n\t\t1.命中级别： 如果为顶级记录，值为1，二级为2，依次类推。\r\n\t\t2.最近距离：只表示在屏幕内的深度，并不对应Z坐标，数值为VC中的 unsigned int 数据类型，需要转换到0~4294967295范围的值。不同的投影方式，会有不同的结果。\r\n\t\t3.最远距离：同上。\r\n\t\t4.名字列表：成员1表示顶级名字，成员2表示二级名字，依次类推，最后一个成员表示命中名字。\r\n\t\t5.如果存在其他命中记录就重复1-4步。"
// 参数<1>: 屏幕横向位置 SDT_INT, 参数说明: "在当前OPenGL所显示的窗口组件中的横向位置"
// 参数<2>: 屏幕纵向位置 SDT_INT, 参数说明: "在当前OPenGL所显示的窗口组件中的纵向位置"
// 参数<3>: 选择区域宽度 SDT_INT, 参数说明: "需要进行选择的区域的宽度，宽度越大搜索范围越大，计算量也越大。"
// 参数<4>: 选择区域高度 SDT_INT, 参数说明: "需要进行选择的区域的高度，高度越大搜索范围越大，计算量也越大。"
// 参数<5>: 命名图元子程序 SDT_SUB_PTR, 参数说明: "子程序没有使用参数和返回值。只有在这个子程序中使用与名字栈有关的命令才有效。"
// 参数<6>: &存放选择选择缓存的数组 数组 SDT_INT, 参数说明: "原来的数据将被清除。并重新定义为一维数组。"
OPENGL_EXTERN_C void opengl_glSelect_122_opengl(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[0].m_int;
    INT      arg2 = pArgInf[1].m_int;
    INT      arg3 = pArgInf[2].m_int;
    INT      arg4 = pArgInf[3].m_int;
    DWORD    arg5 = pArgInf[4].m_dwSubCodeAdr;
    void**   arg6 = pArgInf[5].m_ppAryData;

}

