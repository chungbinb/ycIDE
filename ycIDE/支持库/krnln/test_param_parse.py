# -*- coding: utf-8 -*-
from parse_help_file import HelpFileParser
import re

p = HelpFileParser('../../系统核心支持库帮助说明文件.txt')
p.load_file()

# 测试参数解析
test_line = '参数<1>的名称为"条件"，类型为"逻辑型（bool）"。本条件值的结果决定下一步程序执行位置。'
print(f'测试行: {test_line}')
print()

# 测试正则匹配
pattern = r'参数<(\d+)>的名称为"([^"]+)"，类型为"([^"]+)"'
print(f'正则表达式: {pattern}')
match = re.search(pattern, test_line)
if match:
    print(f'匹配成功!')
    print(f'  参数编号: {match.group(1)}')
    print(f'  参数名称: {match.group(2)}')
    print(f'  参数类型: {match.group(3)}')
else:
    print(f'匹配失败!')
    print(f'测试行字符编码:')
    for i, c in enumerate(test_line[:40]):
        print(f'  {i:2d}: {c} = U+{ord(c):04X}')

# 测试_parse_parameter函数
print()
print('调用_parse_parameter:')
param = p._parse_parameter(test_line, 0)
if param:
    print(f'解析成功: {param.name} ({param.type_cn})')
else:
    print('解析失败，返回None')
