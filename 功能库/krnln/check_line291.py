# -*- coding: utf-8 -*-

with open('parse_help_file.py', 'r', encoding='utf-8') as f:
    lines = f.readlines()
    
line_291 = lines[290]
print(f'第291行: {line_291}')
print()
print('引号字符分析:')
for i, c in enumerate(line_291):
    if c in ['"', '"', '"']:
        print(f'  位置{i}: [{c}] = U+{ord(c):04X}')
