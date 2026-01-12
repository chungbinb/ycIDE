# -*- coding: utf-8 -*-
# 使用Unicode字符码修复parse_help_file.py中的引号问题

with open('parse_help_file.py', 'r', encoding='utf-8') as f:
    lines = f.readlines()

# 找到并修复第291行的正则表达式
# 使用 chr() 来插入正确的中文引号
left_quote = chr(0x201C)   # "
right_quote = chr(0x201D)  # "

# 构建新的正则表达式
new_pattern_line = f"        match = re.search(r'参数<(\\d+)>的名称为{left_quote}([^{right_quote}]+){right_quote}，类型为{left_quote}([^{right_quote}]+){right_quote}', line)\n"

# 替换第291行 (索引290)
if len(lines) > 290:
    print(f'旧的第291行: {lines[290][:80]}')
    lines[290] = new_pattern_line
    print(f'新的第291行: {lines[290][:80]}')
    
    # 写回文件
    with open('parse_help_file.py', 'w', encoding='utf-8') as f:
        f.writelines(lines)
    
    print('修复完成!')

# 同样修复默认值的引号 (大约在第318行)
with open('parse_help_file.py', 'r', encoding='utf-8') as f:
    lines = f.readlines()

for i, line in enumerate(lines):
    if '初始值为' in line and 'default_match' in line:
        print(f'\n找到默认值行在第{i+1}行: {line[:80]}')
        new_line = f"        default_match = re.search(r'初始值为{left_quote}([^{right_quote}]*){right_quote}', line)\n"
        lines[i] = new_line
        print(f'新的行: {new_line[:80]}')
        
        with open('parse_help_file.py', 'w', encoding='utf-8') as f:
            f.writelines(lines)
        
        print('默认值修复完成!')
        break
