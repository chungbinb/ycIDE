# -*- coding: utf-8 -*-
import re

# 测试当前的正则表达式
current_pattern = r'参数<(\d+)>的名称为"([^"]+)"，类型为"([^"]+)"'
correct_pattern = r'参数<(\d+)>的名称为"([^"]+)"，类型为"([^"]+)"'

test_line = '参数<1>的名称为"条件"，类型为"逻辑型（bool）"。本条件值的结果决定下一步程序执行位置。'

print(f'测试行: {test_line[:50]}')
print()
print(f'当前正则 (普通引号): {current_pattern}')
match1 = re.search(current_pattern, test_line)
print(f'  匹配结果: {match1 is not None}')
if match1:
    print(f'  提取: {match1.groups()}')

print()
print(f'正确正则 (中文引号): {correct_pattern}')
match2 = re.search(correct_pattern, test_line)
print(f'  匹配结果: {match2 is not None}')
if match2:
    print(f'  提取: {match2.groups()}')

print()
print('引号编码对比:')
print(f'  普通引号 ": U+{ord('"'):04X}')
print(f'  左中文引号 ": U+{ord('"'):04X}')
print(f'  右中文引号 ": U+{ord('"'):04X}')
