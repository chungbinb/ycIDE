# ycIDE - 易语言现代化 IDE

# 目前主要在更新基于HTML重构的ycIDE版，地址：https://github.com/chungbinb/ycIDE-electron

一个现代化的易语言 IDE 实现，基于 C++ 和 Windows API 开发，完全支持 x64 + Unicode 架构。
开发交流QQ群：767523155

## 版本简介
- **版本**：GDI版
  使用GDI绘制界面和代码编辑器

- **版本**：html版
  使用现代化的HTML和CSS绘制界面和代码编辑器，可以有效支持Accessibility（无障碍功能），并且界面更美观，功能更丰富

## 特性

- ✅ **x64 原生支持**：完全 64 位架构，抛弃 32 位兼容
- ✅ **Unicode 编码**：全面使用 UTF-8，告别 GBK 编码问题
- ✅ **现代化界面**：深色主题，流畅的编辑体验
- ✅ **智能补全**：支持中文/拼音/首字母输入的命令自动补全
- ✅ **图标化界面**：不同命令类型使用不同颜色图标
- ✅ **支持库系统**：兼容 .fne 格式支持库，系统核心支持库已实现
- ✅ **语法高亮**：易语言关键字和命令高亮显示
- ✅ **多文档支持**：标签页管理多个文件

## 技术栈

- **语言**：C++17
- **平台**：Windows x64
- **IDE 编译器**：Visual Studio 2022
- **项目编译器**：LLVM/Clang + MinGW-w64（用于编译用户项目）
- **架构**：原生 Windows API（无 MFC/Qt 依赖）

## 项目结构

```
ycIDE/
├── ycIDE/              # IDE 主程序
│   ├── YiEditor.cpp    # 编辑器核心
│   ├── FneParser.cpp   # .fne 支持库解析器
│   ├── LibraryParser.cpp # 文本支持库解析器
│   ├── Keyword.cpp     # 关键词和自动补全
│   └── ...
├── 功能库/             # 支持库实现
│   └── krnln/          # 系统核心支持库
│       ├── generate_code.py  # 代码生成器
│       └── ...
└── 易语言模块查看器/   # 辅助工具

```

## 构建

### 前置要求

- Visual Studio 2022 或更高版本
- Windows SDK
- Python 3.x（用于代码生成）
- LLVM/Clang（用于编译用户项目）
- MinGW-w64（提供 Windows 头文件和库）

### 编译步骤

1. **生成系统核心支持库**
```powershell
cd 功能库\krnln
python generate_code.py
```

2. **编译支持库**
```powershell
# 使用 Visual Studio 打开 krnln.sln
# 选择 x64 Debug 或 Release 配置
# 生成解决方案
```

3. **编译 IDE**
```powershell
# 使用 Visual Studio 打开 ycIDE.slnx
# 选择 x64 Debug 或 Release 配置
# 生成解决方案
```

4. **复制支持库**
```powershell
# 将编译好的 krnln.fne 复制到 ycIDE\x64\Debug\lib\ 目录
Copy-Item "功能库\krnln\x64\Debug\krnln.fne" "ycIDE\x64\Debug\lib\"
```

5. **配置编译器（LLVM + MinGW）**

由于文件较大，不包含在 Git 仓库中，需要手动下载：

```powershell
# 在 ycIDE\x64\Debug\ 目录下创建目录结构
mkdir compiler\llvm
mkdir compiler\mingw64
```

- 下载 LLVM/Clang: https://github.com/llvm/llvm-project/releases
  - 文件: `clang+llvm-22.1.0-x86_64-pc-windows-msvc.tar.xz`
  - 解压到 `compiler\llvm\`
- 下载 MinGW-w64: https://github.com/niXman/mingw-builds-binaries/releases
  - 文件: `x86_64-15.2.0-release-posix-seh-ucrt-rt_v13-rev1.7z`
  - 解压到 `compiler\mingw64\`

详细说明参见 [ycIDE/编译器配置说明.md](ycIDE/编译器配置说明.md)

## 使用说明

1. 启动 ycIDE.exe
2. 新建或打开源文件（.eyc）
3. 开始编写代码，支持：
   - 中文输入：直接输入"如果"
   - 拼音输入：输入"ruguo"
   - 首字母：输入"rg"
4. 使用 Tab 或 Enter 键确认补全
5. Ctrl+S 保存文件

## 自动补全功能

- **触发**：输入 1 个字符即可触发
- **匹配方式**：
  - 中文完全匹配/前缀匹配
  - 拼音完全匹配/前缀匹配
  - 首字母匹配
- **图标说明**：
  - 🔷 蓝色菱形：库命令（支持库提供）
  - ▶️ 紫色箭头：流程控制命令
  - ▣ 青色方块：数据类型命令
  - ＋ 黄色加号：运算符命令
  - ○ 浅蓝圆圈：其他命令

## 系统核心支持库

已实现 672 个系统核心库命令，包括：
- 流程控制：如果、判断、循环等
- 数学运算：四则运算、三角函数、随机数等
- 逻辑运算：与、或、非等
- 位运算：位与、位或、位异或等
- 文本处理
- 日期时间
- 等等...

## 编码规范

- **源代码**：UTF-8 编码
- **运行时字符串**：UTF-8（.fne 支持库中的字符串）
- **Windows API**：使用 Wide 字符版本（UTF-16）
- **字符串转换**：UTF-8 ↔ UTF-16

## 开发路线图

- [x] 基础编辑器功能
- [x] 语法高亮
- [x] 自动补全（中文/拼音/首字母）
- [x] 系统核心支持库
- [x] .fne 支持库加载
- [x] 项目管理
- [x] LLVM/Clang 编译后端
- [ ] 语法检查
- [ ] 代码调试
- [ ] 更多支持库
- [ ] 代码格式化
- [ ] 跨平台编译（Android / 鸿蒙 / Linux）

## 贡献

欢迎提交 Issue 和 Pull Request！

## 许可证

本项目采用 [MIT License](LICENSE) 开源协议。

## 鸣谢

- 易语言原作者
- 所有为易语言生态贡献的开发者

---

**注意**：本项目是易语言 IDE 的现代化重新实现，与原易语言无官方关联。
