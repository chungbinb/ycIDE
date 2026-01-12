# -*- coding: utf-8 -*-
import re
from parse_help_file import HelpFileParser

p = HelpFileParser('../../系统核心支持库帮助说明文件.txt')
p.load_file()

# 手动调用 _parse_single_command 解析"如果真"命令
start_line = 25  # 从前面的调试得知"如果真"在第25行

print(f'开始解析第{start_line}行的命令:')
print(f'{p.lines[start_line]}')
print()

cmd = p._parse_single_command(start_line, "流程控制", 1, 1)
if cmd:
    print(f'解析成功:')
    print(f'  命令名称: {cmd.name}')
    print(f'  英文名称: {cmd.eng_name}')
    print(f'  参数数量: {len(cmd.params)}')
    for i, param in enumerate(cmd.params):
        print(f'    参数{i+1}: {param.name} ({param.type_cn})')
else:
    print('解析失败')

# 同时手动测试参数行是否会被处理
print()
print('测试参数行检测:')
for i in range(start_line + 1, start_line + 10):
    if i < len(p.lines):
        line = p.lines[i].strip()
        if line.startswith("参数<"):
            print(f'  第{i}行 startswith("参数<"): True')
            print(f'    内容: {line[:80]}')
            param = p._parse_parameter(line, 0)
            if param:
                print(f'    解析成功: {param.name} ({param.type_cn})')
            else:
                print(f'    解析失败')
