# -*- coding: utf-8 -*-
from parse_help_file import HelpFileParser

p = HelpFileParser('../../系统核心支持库帮助说明文件.txt')
p.load_file()

# 找到"如果真"命令
lines = p.lines
for i, line in enumerate(lines):
    if '调用格式：' in line and '如果真' in line:
        print(f'找到如果真命令定义在第{i}行')
        print(f'{i}: {line[:80]}')
        
        # 打印后续20行
        for j in range(1, 20):
            if i + j < len(lines):
                next_line = lines[i + j]
                # 检查是否符合停止条件
                is_stop = (next_line.strip().startswith("调用格式：") or 
                          next_line.strip().startswith("--- 命令类别：") or 
                          next_line.strip().startswith("---------------------------------------------------"))
                           
                marker = " [STOP]" if is_stop else ""
                print(f'{i+j}: {next_line[:80]}{marker}')
                
                if is_stop:
                    break
        break
