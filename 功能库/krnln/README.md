# 系统核心支持库 (krnln) 

## 项目说明

本项目是从易语言导出的帮助说明文件自动生成的系统核心支持库C++源码，支持Unicode和x64位编译。

## 生成信息

- **生成日期**: 2026-01-04
- **源文件**: 系统核心支持库帮助说明文件.txt
- **命令数量**: 672个
- **数据类型**: 48个
- **常量数量**: 335个

## 命令类别统计

| 类别 | 命令数 |
|------|--------|
| 流程控制 | 15 |
| 算术运算 | 23 |
| 逻辑比较 | 10 |
| 位运算 | 8 |
| 变量操作 | 2 |
| 数组操作 | 10 |
| 环境存取 | 5 |
| 拼音处理 | 7 |
| 文本操作 | 28 |
| 时间操作 | 18 |
| 数值转换 | 14 |
| 字节集操作 | 19 |
| 磁盘操作 | 22 |
| 文件读写 | 25 |
| 系统处理 | 40 |
| 媒体播放 | 8 |
| 程序调试 | 4 |
| 其他 | 29 |
| 数据库 | 81 |
| 网络通信 | 4 |
| 控制台操作 | 2 |
| 易包 | 298 |

## 文件结构

```
krnln/
├── include_krnln_header.h    # 主头文件
├── krnln_cmd_typedef.h       # 命令类型定义(KRNLN_DEF宏)
├── krnln_cmdInfo.cpp         # 命令信息和参数定义
├── krnln_const.cpp           # 库常量定义
├── krnln_dtType.cpp          # 数据类型定义
├── krnln_dllMain.cpp         # DLL入口和库信息
├── Source_krnln.def          # 模块定义文件
├── krnln.vcxproj             # Visual Studio项目文件
├── krnln.sln                 # Visual Studio解决方案
├── cppCode/
│   └── krnln_cmd_impl.cpp    # 命令实现(占位代码)
├── elib/                     # 易语言SDK头文件
├── control/                  # 控件相关代码(待实现)
└── krnln_static/             # 静态库相关(待实现)
```

## 编译说明

### 环境要求
- Visual Studio 2019/2022
- Windows SDK 10.0
- 支持C++17

### 编译步骤
1. 打开 `krnln.sln`
2. 选择配置：
   - Debug/Release
   - Win32/x64
3. 生成项目
4. 输出文件为 `krnln.fne`

### 编译配置
- 字符集: Unicode
- 运行时库: 
  - Debug: /MDd
  - Release: /MD
- 输出扩展名: .fne

## 开发说明

### 命令实现
所有命令的占位实现位于 `cppCode/krnln_cmd_impl.cpp`，每个命令函数签名为：

```cpp
EXTERN_C void KRNLN_NAME(索引, 英文名)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // TODO: 实现此命令
}
```

### 参数访问
```cpp
// 获取整数参数
int val = pArgInf[0].m_nVal;

// 获取文本参数  
char* text = pArgInf[0].m_pText;

// 获取逻辑参数
BOOL bVal = pArgInf[0].m_bVal;

// 设置返回值
pRetData->m_nVal = 123;
pRetData->m_pText = AllocTextBuffer("返回文本");
```

### 添加新命令
1. 在 `krnln_cmd_typedef.h` 的 `KRNLN_DEF` 宏中添加命令定义
2. 在 `krnln_cmdInfo.cpp` 中添加参数信息
3. 在 `cppCode/krnln_cmd_impl.cpp` 中实现命令功能

## 注意事项

1. **流程控制命令**: 如"如果"、"循环"等命令由易语言编译器处理，实际运行时不会调用库中的实现函数

2. **Unicode支持**: 所有文本操作需要支持Unicode，使用宽字符版本的API

3. **64位兼容**: 
   - 使用 `INT_PTR`/`UINT_PTR` 代替指针大小的整数
   - 检查所有指针和句柄的大小

4. **内存管理**: 使用易语言SDK提供的内存分配函数

## 解析工具

项目包含两个Python脚本用于解析和生成代码：

- `parse_help_file.py` - 帮助文件解析器
- `generate_code.py` - 代码生成器

如需重新生成或修改，运行：
```bash
python generate_code.py
```

## 授权说明

本项目仅供学习研究使用，系统核心支持库的原始版权归大有吴涛易语言软件公司所有。
