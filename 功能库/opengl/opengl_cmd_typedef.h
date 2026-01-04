#pragma once

#define __OPENGL_NAME(_index, _name) __LIB2_FNE_NAME_LEFT(__E_FNENAME)##_##_name##_##_index##_

// 传递函数名和索引, 拼接成 定义库名_名字_序号_定义库名, 比如 opengl_test_0_opengl
#define OPENGL_NAME(_index, _name) __LIB2_FNE_NAME_LEFT(__OPENGL_NAME(_index, _name))__LIB2_FNE_NAME_LEFT(__E_FNENAME)

// 传递函数名和索引, 拼接成 "定义库名_名字_序号_定义库名", 比如 "opengl_test_0_opengl"
#define OPENGL_NAME_STR(_index, _name) ______E_FNENAME(__OPENGL_NAME(_index, _name))

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
#define OPENGL_DEF(_MAKE) \
    _MAKE(  0, "选择像素格式", ChoosePixelFormat, "为指定的设备选择一个匹配的像素格式。成功返回像素格式号，失败返回0。", 1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_opengl_global_var + 0)\
    _MAKE(  1, "置像素格式", SetPixelFormat, "使用像素格式号为指定的设备设置像素格式。成功返回真，失败返回假。", 1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_opengl_global_var + 2)\
    _MAKE(  2, "创建着色描述表", wglCreateContext, "为设备创建着色描述表，成功返回着色描述表号，失败返回0。在不用的时候必须删除着色描述表。", 1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_opengl_global_var + 5)\
    _MAKE(  3, "置当前着色描述表", wglMakeCurrent, "设置当前的着色描述表，成功以后所有OPenGL命令都作用在指定的设备上。成功返回真，失败返回假。在不使用着色描述表的时候要释放当前着色描述表。", 1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_opengl_global_var + 6)\
    _MAKE(  4, "删除着色描述表", wglDeleteContext, "删除当前的着色描述表，解除OPenGL命令对指定的设备的作用。成功返回真，失败返回假。", 1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_opengl_global_var + 8)\
    _MAKE(  5, "取任意设备句柄", gethdc, "可以取得任意一个窗口组件的设备句柄，成功返回着色描述表号，失败返回0。", 1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_opengl_global_var + 9)\
    _MAKE(  6, "显示光标", ShowCursor, "指定当前应用程序是否显示光标，也可以用来判断当前的光标是否显示，返回值大于0表示正在显示，返回值小于0表示正在隐藏。", 1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_opengl_global_var + 10)\
    _MAKE(  7, "释放设备句柄", ReleaseDC, "释放由调用“取任意设备句柄”命令获取的指定设备场景，执行成功为1，失败为0。", 1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_opengl_global_var + 11)\
    _MAKE(  8, "矩阵复位", glLoadIdentity, "让当前选中的坐标系回到原点。", 2, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_opengl_global_var + 0)\
    _MAKE(  9, "选择矩阵模式", glMatrixMode, "选择接下来将要进行操作的坐标矩阵。", 2, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_opengl_global_var + 13)\
    _MAKE( 10, "坐标平移", glTranslate, "平移当前坐标矩阵。在矩阵复位以前，将影响接下来的坐标操作。", 2, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_opengl_global_var + 14)\
    _MAKE( 11, "坐标旋转", glRotate, "转动轴为从原点指向参数点的一个向量。在矩阵复位以前，将影响接下来的坐标操作。", 2, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 4, g_argumentInfo_opengl_global_var + 17)\
    _MAKE( 12, "坐标缩放", glScale, "缩放当前坐标矩阵。在矩阵复位以前，将影响接下来的坐标操作。", 2, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_opengl_global_var + 21)\
    _MAKE( 13, "创建透视投影", glFrustum, "在屏幕里面建立一个长方体形状的透视投影的三维坐标空间。长方体的前后两面平行于屏幕，左右两面垂直屏幕并竖直，上下两面垂直屏幕并水平。近处物体大，远处物体小。", 2, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 6, g_argumentInfo_opengl_global_var + 24)\
    _MAKE( 14, "创建正投影", glOrtho, "在屏幕里面建立一个长方体形状的正投影的三维坐标空间。长方体的前后两面平行于屏幕，左右两面垂直屏幕并竖直，上下两面垂直屏幕并水平。近处物体和远处物体一样的大。", 2, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 6, g_argumentInfo_opengl_global_var + 30)\
    _MAKE( 15, "设置视口", glViewport, "设置设备中可以显示的范围。", 2, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 4, g_argumentInfo_opengl_global_var + 36)\
    _MAKE( 16, "矩阵入栈", glPushMatrix, "把当前矩阵压入栈顶。", 2, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_opengl_global_var + 0)\
    _MAKE( 17, "矩阵出栈", glPopMatrix, "退出栈顶矩阵。", 2, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_opengl_global_var + 0)\
    _MAKE( 18, "载入矩阵", glLoadMatrix, "把当前坐标模式变换成指定的矩阵。", 2, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_opengl_global_var + 40)\
    _MAKE( 19, "乘入矩阵", glMultMatrix, "把当前坐标模式再乘以指定的矩阵使它变换成目标的矩阵。", 2, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_opengl_global_var + 40)\
    _MAKE( 20, "创建对称透视投影", gluPerspective, "在屏幕里面建立一个四棱台形状的透视投影的三维坐标空间。四棱台的前后两面平行于屏幕，其余四面呈喇叭口形状对称展开。近处物体大，远处物体小。", 2, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 4, g_argumentInfo_opengl_global_var + 41)\
    _MAKE( 21, "开始画", glBegin, "画一组相似图元的开始语句。画不同的图元需要执行不同条数的“设置点”的命令。以“停止画”命令结尾。", 3, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_opengl_global_var + 45)\
    _MAKE( 22, "停止画", glEnd, "必须与“开始画”搭配使用，单独使用不会有效果。", 3, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_opengl_global_var + 0)\
    _MAKE( 23, "设置点", glVertex, "插入到“开始画”和“停止画”中来设置模型的顶点。", 3, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_opengl_global_var + 46)\
    _MAKE( 24, "画矩形", glRect, "在当前坐标原点指定的深度，使用先前选定的颜色画出一个填充颜色的矩形。", 3, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 4, g_argumentInfo_opengl_global_var + 49)\
    _MAKE( 25, "设置正面", glFrontFace, "为填充颜色的多边形指定正面的定义方向。当四个手指并拢依次绕着“设置点”命令所定义点的方向握紧时，大拇指所指的方向就是正面的方向。反面即为背面。必须启用“#精选面”才有效。默认为#逆时针。", 3, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_opengl_global_var + 53)\
    _MAKE( 26, "设置裁剪平面", glClipPlane, "裁剪面的背面被减去。必须启用“#裁剪测试”和“#零号裁剪面～#五号裁剪面”，才能执行裁剪，不使用的时候需要关闭。", 3, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_opengl_global_var + 54)\
    _MAKE( 27, "设置多边形模式", glPolygonMode, "当绘出图形为封闭的多边形时才起作用。默认设置正面和背面都为面。", 3, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_opengl_global_var + 56)\
    _MAKE( 28, "设置线型", glLineStipple, "设置线段的类型。默认使用实线。必须启用“#线型”才有效，不使用的时候需要关闭。", 3, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_opengl_global_var + 58)\
    _MAKE( 29, "设置图案填充", glPolygonStipple, "设置多边形的填充方式。必须启用“#图案填充”才有效，不使用的时候需要关闭。", 3, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_opengl_global_var + 60)\
    _MAKE( 30, "标记边界线", glEdgeFlag, "在“设置点”之前使用，指定多边形的该点或者线是否是边界线的一部分。只有当多边形的模式为点或者线的时候才有意义。", 3, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_opengl_global_var + 61)\
    _MAKE( 31, "矩形剪切", glScissor, "定义一个矩形剪切框。裁剪框内的内容被保留下来，其余被减掉。必须启用“#裁剪测试”才有效，不使用的时候需要关闭。", 3, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 4, g_argumentInfo_opengl_global_var + 62)\
    _MAKE( 32, "设置画面质量", glHint, "在权衡运行速度和画面质量之后，为显示效果的质量和画面的细节处理设置等级。", 4, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_opengl_global_var + 66)\
    _MAKE( 33, "通道混合测试", glAlphaFunc, "当比较函数返回真的时候才显示出效果来。只有在红绿蓝混合通道模式下，启用“#通道测试”后才有效。不使用的时候需要关闭。", 4, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_opengl_global_var + 68)\
    _MAKE( 34, "混合测试", glBlendFunc, "为混合运算设定运算方法。必须启用“#混合测试”才有效。不使用的时候需要关闭。", 4, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_opengl_global_var + 70)\
    _MAKE( 35, "深度测试", glDepthFunc, "当比较函数返回真的时候才显示出效果来。必须启用“#深度测试”，才能执行测试。不使用的时候需要关闭。", 4, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_opengl_global_var + 72)\
    _MAKE( 36, "模板测试", glStencilFunc, "为模板运算设定影响范围。必须启用“#模板测试”，才能执行测试。不使用的时候需要关闭。", 4, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_opengl_global_var + 73)\
    _MAKE( 37, "模板测试操作", glStencilOp, "为模板运算设定运算方法。必须启用“#模板测试”，才能执行测试。不使用的时候需要关闭。", 4, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_opengl_global_var + 76)\
    _MAKE( 38, "交换缓存", SwapBuffers, "交换后台缓存与前台缓存。将后台缓存显示出来，对前台缓存进行操作。成功返回真，失败返回假。在双缓存模式下才用效。", 5, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_opengl_global_var + 79)\
    _MAKE( 39, "结束绘图操作", glFinish, "单缓存模式下才可以使用。等待所有已经提交的OpenGL命令执行完毕后才返回。不能在“开始画”和“停止画”之间执行本语句。", 5, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_opengl_global_var + 0)\
    _MAKE( 40, "强制结束绘图操作", glFlush, "单缓存模式下才可以使用。强制结束，并且返回，不管已经提交的OpenGL命令是否执行完毕。不能在“开始画”和“停止画”之间执行本语句。", 5, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_opengl_global_var + 0)\
    _MAKE( 41, "操作累积缓存", glAccum, "对裁剪区域里的缓存颜色进行累积缓存操作。", 5, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_opengl_global_var + 80)\
    _MAKE( 42, "清除缓存", glClear, "指定清除的一项或几项缓存。", 5, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_opengl_global_var + 82)\
    _MAKE( 43, "清除累积缓存", glClearAccum, "设置累积缓存的默认值。不执行本命令前参数全部为0。", 5, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 4, g_argumentInfo_opengl_global_var + 83)\
    _MAKE( 44, "清除颜色缓存", glClearColor, "设置颜色缓存的默认值。不执行本命令前参数全部为0。", 5, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 4, g_argumentInfo_opengl_global_var + 87)\
    _MAKE( 45, "清除深度缓存", glClearDepth, "设置深度缓存的默认值。", 5, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_opengl_global_var + 91)\
    _MAKE( 46, "清除模板缓存", glClearStencil, "设置模板缓存的默认值。不执行本命令前模板值为0。", 5, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_opengl_global_var + 92)\
    _MAKE( 47, "清除颜色索引", glClearIndex, "设置颜色索引的默认值。不执行本命令前颜色索引号为0。", 5, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_opengl_global_var + 93)\
    _MAKE( 48, "设置颜色", glColor, "设置接下来的图元的颜色。能在“开始画”和“停止画”之间执行本语句。只有在红绿蓝混合通道模式下有效。", 5, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 4, g_argumentInfo_opengl_global_var + 94)\
    _MAKE( 49, "设置颜色索引", glIndex, "设置接下来的图元的颜色。能在“开始画”和“停止画”之间执行本语句。只有在颜色索引模式下有效。", 5, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_opengl_global_var + 98)\
    _MAKE( 50, "启用功能", glEnable, "启用某项功能后，会一直保持启用状态。要关闭该功能，必须使用停用功能才能停止。", 6, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_opengl_global_var + 99)\
    _MAKE( 51, "停用功能", glDisable, "与启用功能搭配使用。", 6, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_opengl_global_var + 99)\
    _MAKE( 52, "设置光源参数", glLight, "必须启用“#光照”后才有效。", 6, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_opengl_global_var + 100)\
    _MAKE( 53, "设置光照模式", glLightModel, NULL, 6, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_opengl_global_var + 103)\
    _MAKE( 54, "设置材质", glMaterial, NULL, 6, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_opengl_global_var + 105)\
    _MAKE( 55, "设置材质颜色", glColorMaterial, "使用当前颜色为材料的颜色。必须启用“#材质颜色”才有效，不使用的时候需要关闭。", 6, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_opengl_global_var + 108)\
    _MAKE( 56, "设置雾", glFog, "必须启用“#雾”后才有效。", 6, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_opengl_global_var + 110)\
    _MAKE( 57, "设置法向量", glNormal, "指定下一顶点的法向量。能在“开始画”和“停止画”之间执行本语句。启用“#单位法向量”后，法向量的长度就会变成1。", 6, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_opengl_global_var + 112)\
    _MAKE( 58, "设置明暗模式", glShadeModel, "设置明暗的表示方法。对点图元无效。", 6, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_opengl_global_var + 115)\
    _MAKE( 59, "绑定纹理号", glBindTexture, NULL, 7, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_opengl_global_var + 116)\
    _MAKE( 60, "纹理控制", glTexParameter, NULL, 7, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_opengl_global_var + 118)\
    _MAKE( 61, "定义一维纹理图片", glTexImage1D, NULL, 7, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 5, g_argumentInfo_opengl_global_var + 121)\
    _MAKE( 62, "定义二维纹理图片", glTexImage2D, "成功返回真，失败返回假", 7, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 6, g_argumentInfo_opengl_global_var + 126)\
    _MAKE( 63, "设置纹理映射坐标", glTexCoord, NULL, 7, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_opengl_global_var + 132)\
    _MAKE( 64, "设置纹理映射方式", glTexEnv, NULL, 7, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_opengl_global_var + 134)\
    _MAKE( 65, "生成纹理号", glGenTextures, NULL, 7, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_opengl_global_var + 136)\
    _MAKE( 66, "生成纹理坐标", glTexGen, "需要生成纹理号", 7, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_opengl_global_var + 138)\
    _MAKE( 67, "载入位图数据", auxDIBImageLoad, "成功载入返回真，失败返回假", 7, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_opengl_global_var + 141)\
    _MAKE( 68, "开始创建列表", glNewList, NULL, 8, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_opengl_global_var + 143)\
    _MAKE( 69, "停止创建列表", glEndList, "与“开始创建列表”搭配使用。", 8, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_opengl_global_var + 0)\
    _MAKE( 70, "生成列表号", glGenLists, "按次序自动生成一串连续的空列表号。返回其中的第一个列表号。失败返回-1", 8, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_opengl_global_var + 145)\
    _MAKE( 71, "调用列表", glCallList, "调用已经创建的列表。可以重复调用。成功返回真，失败返回假", 8, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_opengl_global_var + 143)\
    _MAKE( 72, "是否存在列表", glIsList, "如果该列表号存在已经编译的列表就返回真，否则返回假。", 8, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_opengl_global_var + 143)\
    _MAKE( 73, "删除列表列", glDeleteLists, "删除一列连续的列表。成功返回真，失败返回假", 8, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_opengl_global_var + 146)\
    _MAKE( 74, "设置偏移列表数", glListBase, "为“调用多显示列表”设定一个起始列表号。成功返回真，失败返回假", 8, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_opengl_global_var + 148)\
    _MAKE( 75, "调用多显示列表", glCallLists, "等效为多次使用“调用列表”的结果。", 8, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_opengl_global_var + 149)\
    _MAKE( 76, "设置光栅位置", glRasterPos, "设置下一次对光栅操作的坐标位置，会受到坐标变换的影响。如果要改变像素的颜色必须在本命令之前使用。", 9, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_opengl_global_var + 150)\
    _MAKE( 77, "画出位图", glBitmap, "必须先使用“设置像素存储字节数”。坐标的原点只与“设置光栅位置”有关与坐标变换命令无关。", 9, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 7, g_argumentInfo_opengl_global_var + 152)\
    _MAKE( 78, "复制像素", glCopyPixels, "复制一个矩形范围内的内容到现在的光栅位置处。成功返回真，失败返回假", 9, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 5, g_argumentInfo_opengl_global_var + 159)\
    _MAKE( 79, "设置像素存储字节数", glPixelStore, "设置位图像素数据在计算机内存中存储的方式", 9, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_opengl_global_var + 164)\
    _MAKE( 80, "像素缩放", glPixelZoom, "设置接下来的像素操作的缩放比例。成功返回真，失败返回假", 9, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_opengl_global_var + 165)\
    _MAKE( 81, "是否为双字节字符", IsDBCSLeadByte, "如果为双字节返回真，否则返回假。如果指定位置超出文本长度，返回0。", 10, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_opengl_global_var + 167)\
    _MAKE( 82, "创建文字轮廓", wglUseFontOutlinesW, "创建三维的文字外形轮廓。如果成功返回真，否则返回假。", 10, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 7, g_argumentInfo_opengl_global_var + 169)\
    _MAKE( 83, "创建实心球", glutSolidSphere, "创建一个圆心在坐标原点的实心球。", 11, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_opengl_global_var + 176)\
    _MAKE( 84, "创建空心球", glutWireSphere, "创建一个圆心在坐标原点的空心球。", 11, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_opengl_global_var + 179)\
    _MAKE( 85, "创建实心正方体", glutSolidCube, "创建一个中心在坐标原点的实心正方体。", 11, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_opengl_global_var + 182)\
    _MAKE( 86, "创建空心正方体", glutWireCube, "创建一个中心在坐标原点的空心正方体。", 11, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_opengl_global_var + 182)\
    _MAKE( 87, "创建实心圆锥体", glutSolidCone, "创建一个中心在坐标原点的实心圆锥体。", 11, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 4, g_argumentInfo_opengl_global_var + 183)\
    _MAKE( 88, "创建空心圆锥体", glutWireCone, "创建一个中心在坐标原点的空心圆锥体。", 11, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 4, g_argumentInfo_opengl_global_var + 187)\
    _MAKE( 89, "创建实心圆环", glutSolidTorus, "创建一个中心在坐标原点的实心圆环。", 11, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 4, g_argumentInfo_opengl_global_var + 191)\
    _MAKE( 90, "创建空心圆环", glutWireTorus, "创建一个中心在坐标原点的空心圆环。", 11, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 4, g_argumentInfo_opengl_global_var + 195)\
    _MAKE( 91, "创建实心十二面体", glutSolidDodecahedron, "创建一个中心在坐标原点的外接球半径为三分之二根号三的实心十二面体。", 11, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_opengl_global_var + 0)\
    _MAKE( 92, "创建空心十二面体", glutWireDodecahedron, "创建一个中心在坐标原点的外接球半径为三分之二根号三的空心十二面体。", 11, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_opengl_global_var + 0)\
    _MAKE( 93, "创建实心八面体", glutSolidOctahedron, "创建一个中心在坐标原点的外接球半径为一的实心八面体。", 11, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_opengl_global_var + 0)\
    _MAKE( 94, "创建空心八面体", glutWireOctahedron, "创建一个中心在坐标原点的外切球半径为一的空心八面体。", 11, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_opengl_global_var + 0)\
    _MAKE( 95, "创建实心四面体", glutSolidTetrahedron, "创建一个中心在坐标原点的外接球半径为根号三的实心四面体。", 11, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_opengl_global_var + 0)\
    _MAKE( 96, "创建空心四面体", glutWireTetrahedron, "创建一个中心在坐标原点的外接球半径为根号三的空心四面体。", 11, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_opengl_global_var + 0)\
    _MAKE( 97, "创建实心二十面体", glutSolidIcosahedron, "创建一个中心在坐标原点的外接球半径为一的实心二十面体。", 11, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_opengl_global_var + 0)\
    _MAKE( 98, "创建空心二十面体", glutWireIcosahedron, "创建一个中心在坐标原点的外接球半径为一的空心二十面体。", 11, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_opengl_global_var + 0)\
    _MAKE( 99, "创建实心圆柱体", glutSolidcylinder, "创建一个中心在坐标原点的实心圆柱体。", 11, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_opengl_global_var + 199)\
    _MAKE(100, "创建空心圆柱体", glutWirecylinder, "创建一个中心在坐标原点的空心圆柱体。", 11, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_opengl_global_var + 202)\
    _MAKE(101, "取错误信息文本", glGetError, "如果执行某OPenGL命令失败，在其后执行本命令可以取回错误信息文本。\t\t如果该OPenGL命令执行成功，执行本命令将返回空文本。\r\n\t\t如果参数值超出整数的范围，返回“无效整数”\r\n\t\t如果参数值超出可选的范围，返回“无效数值”\r\n\t\t如果OPenGL命令出现在错误的位置，返回“无效操作”\r\n\t\t如果在堆栈已满时继续入栈，返回“堆栈溢出”\r\n\t\t如果在堆栈已空时继续出栈，返回“堆栈下溢”", 12, _CMD_OS(__OS_WIN), SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_opengl_global_var + 0)\
    _MAKE(102, "设置观察坐标", gluLookAt, "新的视图空间以坐标中心位置为坐标原点，坐标中心位置到眼睛的矢量为纵向坐标轴的正方向，以正上方矢量为竖向坐标轴的正方向。", 2, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 9, g_argumentInfo_opengl_global_var + 205)\
    _MAKE(103, "载入3DS文件", glLoad3DS, "载入以3DS为后缀的三维模型文件，并编译为一个显示列表。如果3DS文件中包含有纹理图片，那么只有BMP位图格式的纹理才可以被使用。否则无法显示纹理。使用“调用列表”命令可以显示模型。成功返回真，失败返回假。", 12, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_opengl_global_var + 214)\
    _MAKE(104, "创建实心圆台", glutSolidtaper, "创建一个中心在坐标原点的实心圆台。", 11, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 5, g_argumentInfo_opengl_global_var + 217)\
    _MAKE(105, "创建空心圆台", glutWiretaper, "创建一个中心在坐标原点的空心圆台。", 11, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 5, g_argumentInfo_opengl_global_var + 222)\
    _MAKE(106, "设置线宽", glLineWidth, "为所画的线段设置线条宽度。如果操作系统支持指定的宽度大小，返回真，否则返回假。通过启用和停用“光滑线”功能，来控制线段的放样效果。", 3, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_opengl_global_var + 227)\
    _MAKE(107, "设置点大小", glPointSize, "为所画的点设置点的大小。通过启用和停用“光滑点”功能，来控制点的放样效果。", 3, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_opengl_global_var + 228)\
    _MAKE(108, "设置曲线控制点", glMap1, "为Bezier曲线定义控制点。定义了控制点后，才可以画曲线。成功返回有效的控制点数，失败返回0。始终保持这样的曲线，直到再次使用本命令改变控制点。\r\n\t本命令使用参数方程来定义曲线上的每个顶点。\r\n\t\tX = f （U ）\r\n\t\tY = f （U ）\r\n\t\tZ = f （U ）\r\n\t其中X、Y、Z分别代表曲线上的点的横向位置、竖向位置和纵向位置，U代表参数方程f（）的参数\r\n当参数 U 发生变化的时候，X、Y、Z也相应的改变。", 3, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_opengl_global_var + 229)\
    _MAKE(109, "设置曲线点", glEvalCoord1, "插入到“开始画”和“停止画”中来设置曲线上的点。先使用“设置曲线控制点”命令生成曲线，然后指定U参数的值，计算出曲线上点的坐标。必须启用“#曲线坐标”才有效，不使用的时候需要关闭。", 3, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_opengl_global_var + 232)\
    _MAKE(110, "设置曲线等分点", glMapGrid1, "先使用“设置曲线控制点”命令生成曲线，然后设置曲线的等分点数，让曲线光滑的过度。\r\n\t最终得到的每两个相邻U参数的间隔为：（U向等分下限 - U向等分上限）/ U向等分数。", 3, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_opengl_global_var + 233)\
    _MAKE(111, "画曲线", glEvalMesh1, "先使用“设置曲线控制点”命令生成曲线，然后才能画出曲线。显示范围在“曲线起点”和“曲线终点”之间。必须启用“#曲线坐标”才有效，不使用的时候需要关闭。如果需要使用法向量，必须启用“#自动法向量”才有效，不使用的时候需要关闭。", 3, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_opengl_global_var + 236)\
    _MAKE(112, "设置曲面控制点", glMap2, "为Bezier曲面定义控制点。定义了控制点后，才可以画曲面。成功返回有效的控制点数，失败返回0。必须启用“#曲面坐标”才有效，不使用的时候需要关闭。始终保持这样的曲面，直到再次使用本命令改变控制点。\r\n\t本命令使用参数方程来定义曲线上的每个顶点。\r\n\t\tX = f （U ，V）\r\n\t\tY = f （U ，V）\r\n\t\tZ = f （U ，V）\r\n\t其中X、Y、Z分别代表曲线上的点的横向位置、竖向位置和纵向位置，U、V代表参数方程f（）的参数\r\n当参数 U 、V发生变化的时候，X、Y、Z也相应的改变。", 3, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 6, g_argumentInfo_opengl_global_var + 239)\
    _MAKE(113, "设置曲面点", glEvalCoord2, "插入到“开始画”和“停止画”中来设置曲线上的点。先使用“设置曲面控制点”命令生成曲线，然后指定U参数的值，计算出曲线上点的坐标。必须启用“#曲面坐标”才有效，不使用的时候需要关闭。", 3, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_opengl_global_var + 245)\
    _MAKE(114, "设置曲面等分网格", glMapGrid2, "先使用“设置曲面控制点”命令生成曲面，然后设置曲面的等分网格，让曲面光滑的过度。\r\n\t最终得到的每两个相邻U参数的间隔为：（U向等分下限 - U向等分上限）/ U向等分数。\r\n\t最终得到的每两个相邻V参数的间隔为：（V向等分下限 - V向等分上限）/ V向等分数。", 3, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 6, g_argumentInfo_opengl_global_var + 247)\
    _MAKE(115, "画曲面", glEvalMesh2, "先使用“设置曲面控制点”命令生成曲面，然后才能画出曲面。U向显示范围在“曲面U向起点”和“曲面U向起点”之间。V向显示范围在“曲面V向起点”和“曲面V向起点”之间。必须启用“#曲面坐标”才有效，不使用的时候需要关闭。如果需要使用法向量，必须启用“#自动法向量”才有效，不使用的时候需要关闭。", 3, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 5, g_argumentInfo_opengl_global_var + 253)\
    _MAKE(116, "是否存在纹理号", glIsTexture, "如果该纹理号已经存在就返回真，否则返回假。", 7, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_opengl_global_var + 258)\
    _MAKE(117, "删除纹理号", glDeleteTextures, "删除已经创建的纹理号，与纹理号相应的纹理将无法使用。返回实际删除的纹理号的数量", 7, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_opengl_global_var + 259)\
    _MAKE(118, "清空名字栈", glInitNames, "清空名字栈中的所有名字。返回最大名字栈长度。必须在选择模式下使用。否则无效。", 13, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_HIGH, 0, 0, 0, g_argumentInfo_opengl_global_var + 0)\
    _MAKE(119, "名字入栈", glPushName, "设置接下来的图元的名称。并把指定名称压入栈顶,不能超出最大名字栈长度。每个图元的名字与当时出现在名字栈中所有的名字有关。因此一个图元可以拥有多个名字。连续重复使用本命令即可达到命名多级名字的效果。必须在选择模式下使用。否则无效。", 13, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_HIGH, 0, 0, 1, g_argumentInfo_opengl_global_var + 261)\
    _MAKE(120, "名字出栈", glPopName, "停止使用当前的名称。并把栈顶名称退出栈顶名称，不能在名字栈为空的时候出栈。必须在选择模式下使用。否则无效。", 13, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_HIGH, 0, 0, 0, g_argumentInfo_opengl_global_var + 0)\
    _MAKE(121, "置栈顶名字", glLoadName, "设置接下来的图元的名称。如果名字栈为空，将返回假。必须在选择模式下使用。否则无效。", 13, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_HIGH, 0, 0, 1, g_argumentInfo_opengl_global_var + 262)\
    _MAKE(122, "进入选择模式", glSelect, "进入选择模式后画出的任何图元都不会显示出来。只有在选择模式下对模型进行命名的操作才有效。成功后将所有在选择区域中包含的命中记录存放在数组参数中。并且返回命中记录的个数。否则清空数组，并且返回0。\r\n\t选择缓存内容的排列规则：\r\n\t\t1.命中级别： 如果为顶级记录，值为1，二级为2，依次类推。\r\n\t\t2.最近距离：只表示在屏幕内的深度，并不对应Z坐标，数值为VC中的 unsigned int 数据类型，需要转换到0~4294967295范围的值。不同的投影方式，会有不同的结果。\r\n\t\t3.最远距离：同上。\r\n\t\t4.名字列表：成员1表示顶级名字，成员2表示二级名字，依次类推，最后一个成员表示命中名字。\r\n\t\t5.如果存在其他命中记录就重复1-4步。", 13, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_HIGH, 0, 0, 6, g_argumentInfo_opengl_global_var + 263)\
    _MAKE(123, "到屏幕坐标", gluProject, "将OPenGL的空间坐标转换到组件的屏幕坐标。成功返回真，否则返回假。", 13, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 6, g_argumentInfo_opengl_global_var + 269)\
    _MAKE(124, "到空间坐标", gluUnProject, "将组件的屏幕坐标转换到OPenGL的空间坐标。成功返回真，否则返回假。", 13, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 6, g_argumentInfo_opengl_global_var + 275)\
    _MAKE(125, "载入TGA图片", glTGALoad, "成功载入返回真，失败返回假", 7, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 4, g_argumentInfo_opengl_global_var + 281)

