# -*- coding: utf-8 -*-
import re

code = open('parse_help_file.py', 'r', encoding='utf-8').read()
match_line = [line for line in code.split('\n') if '参数<' in line and 'match = re.search' in line][0]

print(f'代码行: {match_line}')
print()
print('查找引号字符:')
for i, c in enumerate(match_line):
    if c == '"':  # 普通引号
        print(f'  位置{i}: 普通引号 [{c}] = U+{ord(c):04X}')
    elif c == '"':  # 左引号
        print(f'  位置{i}: 左引号 [{c}] = U+{ord(c):04X}')
    elif c == '"':  # 右引号
        print(f'  位置{i}: 右引号 [{c}] = U+{ord(c):04X}')
