# -*- coding: utf-8 -*-
# 修复parse_help_file.py中的引号问题

with open('parse_help_file.py', 'r', encoding='utf-8') as f:
    content = f.read()

# 将普通引号替换为中文引号 (只在正则表达式中)
# 旧的正则: r'参数<(\d+)>的名称为"([^"]+)"，类型为"([^"]+)"'
# 新的正则: r'参数<(\d+)>的名称为"([^"]+)"，类型为"([^"]+)"'

old_pattern = r'''match = re.search(r'参数<(\d+)>的名称为"([^"]+)"，类型为"([^"]+)"', line)'''
new_pattern = r'''match = re.search(r'参数<(\d+)>的名称为"([^"]+)"，类型为"([^"]+)"', line)'''

if old_pattern in content:
    print('找到需要修复的代码行')
    content = content.replace(old_pattern, new_pattern)
    
    with open('parse_help_file.py', 'w', encoding='utf-8') as f:
        f.write(content)
    
    print('修复完成!')
else:
    print('未找到需要修复的代码,可能已经修复过了')

# 同样修复默认值的引号
old_default = r'''default_match = re.search(r'初始值为"([^"]*)"', line)'''
new_default = r'''default_match = re.search(r'初始值为"([^"]*)"', line)'''

with open('parse_help_file.py', 'r', encoding='utf-8') as f:
    content = f.read()

if old_default in content:
    print('找到默认值的代码需要修复')
    content = content.replace(old_default, new_default)
    
    with open('parse_help_file.py', 'w', encoding='utf-8') as f:
        f.write(content)
    
    print('默认值修复完成!')
