# -*- coding: utf-8 -*-
from parse_help_file import HelpFileParser
import re

p = HelpFileParser('../../系统核心支持库帮助说明文件.txt')
p.load_file()

# 查找"如果真"命令
lines = p.lines
for i, line in enumerate(lines):
    if '如果真' in line and '调用格式' in line:
        print(f'找到如果真命令定义在第{i}行: {line[:80]}')
        # 打印后续10行
        for j in range(1, 11):
            if i + j < len(lines):
                next_line = lines[i + j].strip()
                if next_line:
                    print(f'  第{i+j}行: {next_line[:100]}')
                    if '参数<' in next_line:
                        print('    -> 包含参数定义!')
                        # 打印每个字符
                        line_part = next_line[:30]
                        print(f'      前30个字符: {[f"{c}(U+{ord(c):04X})" for c in line_part]}')
        print()
        break
