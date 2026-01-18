# -*- coding: utf-8 -*-
from parse_help_file import HelpFileParser

class DebugParser(HelpFileParser):
    def _parse_command(self, line: str, start_line: int, cmd_index: int, category: str, cat_index: int):
        """重写解析命令函数以添加调试输出"""
        result = super()._parse_command(line, start_line, cmd_index, category, cat_index)
        if result and result.name == '如果真':
            print(f'解析到"如果真"命令:')
            print(f'  参数数量: {len(result.params)}')
            for i, param in enumerate(result.params):
                print(f'    参数{i+1}: {param.name} ({param.type_cn})')
        return result

p = DebugParser('../../系统核心支持库帮助说明文件.txt')
p.load_file()
p.parse_commands()

cmd = next((c for c in p.commands if c.name == '如果真'), None)
if cmd:
    print(f'\n最终结果:')
    print(f'命令名称: {cmd.name}')
    print(f'参数数量: {len(cmd.params)}')
