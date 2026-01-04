# -*- coding: utf-8 -*-
"""
系统核心支持库帮助文件解析器
从txt帮助文件中提取命令、数据类型、常量信息
生成支持Unicode和x64的C++代码
"""

import re
import os
from dataclasses import dataclass, field
from typing import List, Dict, Optional, Tuple

@dataclass
class Parameter:
    """命令参数"""
    index: int
    name: str
    type_cn: str  # 中文类型名
    type_en: str  # 英文类型标识
    description: str = ""
    optional: bool = False  # 可省略
    can_be_array: bool = False  # 可以是数组
    only_var: bool = False  # 只能提供变量
    default_value: str = ""  # 默认值

@dataclass 
class Command:
    """命令定义"""
    index: int  # 命令索引
    name: str  # 中文名称
    eng_name: str  # 英文名称
    description: str  # 详细说明
    category: str  # 所属类别
    category_index: int  # 类别索引
    return_type: str  # 返回类型
    return_type_en: str  # 返回类型英文
    params: List[Parameter] = field(default_factory=list)
    os_support: str = "Windows"  # 操作系统支持
    is_hidden: bool = False  # 是否隐藏
    allow_append: bool = False  # 最后参数可重复添加
    is_object_method: bool = False  # 是否为对象成员命令
    object_type: str = ""  # 所属对象类型

@dataclass
class DataTypeMember:
    """数据类型成员属性"""
    index: int
    name: str
    eng_name: str
    type_cn: str
    type_en: str
    description: str = ""

@dataclass
class DataTypeEvent:
    """数据类型事件"""
    index: int
    name: str
    description: str = ""
    return_type: str = ""

@dataclass
class DataType:
    """数据类型定义"""
    index: int
    name: str
    eng_name: str
    description: str = ""
    type_kind: str = "普通类型"  # 普通类型/功能窗口组件型
    os_support: str = "Windows"
    members: List[DataTypeMember] = field(default_factory=list)
    events: List[DataTypeEvent] = field(default_factory=list)
    methods: List[Command] = field(default_factory=list)

@dataclass
class Constant:
    """常量定义"""
    name: str
    eng_name: str = ""
    type_cn: str = "数值型"
    value: str = ""
    description: str = ""

class HelpFileParser:
    """帮助文件解析器"""
    
    # 类型映射
    TYPE_MAP = {
        "整数型": ("SDT_INT", "int"),
        "短整数型": ("SDT_SHORT", "short"),
        "字节型": ("SDT_BYTE", "BYTE"),
        "长整数型": ("SDT_INT64", "INT64"),
        "小数型": ("SDT_FLOAT", "float"),
        "双精度小数型": ("SDT_DOUBLE", "double"),
        "逻辑型": ("SDT_BOOL", "bool"),
        "文本型": ("SDT_TEXT", "text"),
        "字节集型": ("SDT_BIN", "bin"),
        "字节集": ("SDT_BIN", "bin"),
        "日期时间型": ("SDT_DATE_TIME", "datetime"),
        "子程序指针型": ("SDT_SUB_PTR", "subptr"),
        "通用型": ("_SDT_ALL", "all"),
        "无返回值": ("_SDT_NULL", "void"),
        "窗口": ("DT_WINDOW", "window"),
        "对象": ("DT_COMOBJECT", "ComObject"),
        "变体型": ("DT_VARIANT", "variant"),
    }
    
    def __init__(self, filepath: str):
        self.filepath = filepath
        self.content = ""
        self.lines: List[str] = []
        self.commands: List[Command] = []
        self.data_types: List[DataType] = []
        self.constants: List[Constant] = []
        self.categories: List[str] = []  # 命令类别列表
        
    def load_file(self):
        """加载文件内容"""
        # 尝试不同编码
        for encoding in ['utf-8', 'gbk', 'gb2312', 'gb18030']:
            try:
                with open(self.filepath, 'r', encoding=encoding) as f:
                    self.content = f.read()
                    self.lines = self.content.split('\n')
                print(f"成功使用 {encoding} 编码读取文件")
                return True
            except UnicodeDecodeError:
                continue
        print("无法读取文件，请检查编码")
        return False
    
    def parse_type(self, type_str: str) -> Tuple[str, str]:
        """解析类型字符串，返回(SDT常量, 英文名)"""
        # 清理类型字符串
        type_str = type_str.strip()
        # 移除括号中的英文标识
        if '（' in type_str:
            type_str = type_str.split('（')[0].strip()
        
        for cn, (sdt, en) in self.TYPE_MAP.items():
            if cn in type_str:
                return sdt, en
        
        # 未知类型，可能是自定义数据类型
        return "_SDT_ALL", "all"
    
    def parse_commands(self):
        """解析所有命令"""
        i = 0
        current_category = ""
        category_index = -1
        cmd_index = 0
        current_object_type = ""
        
        while i < len(self.lines):
            line = self.lines[i].strip()
            
            # 检测命令类别
            if line.startswith("--- 命令类别："):
                current_category = line.replace("--- 命令类别：", "").replace(" ---", "").strip()
                if current_category not in self.categories:
                    self.categories.append(current_category)
                category_index = self.categories.index(current_category) + 1
                current_object_type = ""
                i += 1
                continue
            
            # 检测数据类型命令（对象成员命令）
            if line.startswith("---------------------------------------------------"):
                # 可能是数据类型定义的开始
                i += 1
                continue
            
            # 检测调用格式行
            if line.startswith("调用格式："):
                cmd = self._parse_single_command(i, current_category, category_index, cmd_index)
                if cmd:
                    self.commands.append(cmd)
                    cmd_index += 1
                i += 1
                continue
                
            i += 1
    
    def _parse_single_command(self, start_line: int, category: str, cat_index: int, cmd_index: int) -> Optional[Command]:
        """解析单个命令"""
        line = self.lines[start_line].strip()
        
        # 解析调用格式行
        # 格式：调用格式： 〈返回类型〉 命令名 （参数...） - 库名->类别
        # 或：调用格式： 〈返回类型〉 对象．方法名 （参数...） - 库名->类别
        
        match = re.search(r'调用格式：\s*〈([^〉]+)〉\s*(.+?)\s*（([^）]*)）', line)
        if not match:
            return None
            
        return_type_cn = match.group(1).strip()
        cmd_name_full = match.group(2).strip()
        params_str = match.group(3).strip()
        
        # 检查是否为对象成员命令
        is_object_method = "．" in cmd_name_full or "对象．" in line
        object_type = ""
        cmd_name = cmd_name_full
        
        if is_object_method and "．" in cmd_name_full:
            parts = cmd_name_full.split("．")
            object_type = parts[0]
            cmd_name = parts[1] if len(parts) > 1 else cmd_name_full
        
        # 解析返回类型
        return_type_sdt, return_type_en = self.parse_type(return_type_cn)
        
        # 获取英文名称
        eng_name = ""
        description = ""
        is_hidden = False
        allow_append = False
        os_support = "Windows"
        
        # 继续读取后续行获取更多信息
        i = start_line + 1
        params = []
        param_index = 0
        
        while i < len(self.lines):
            next_line = self.lines[i].strip()
            
            # 遇到下一个命令或类别定义时停止
            if next_line.startswith("调用格式：") or next_line.startswith("--- 命令类别：") or next_line.startswith("---------------------------------------------------"):
                break
            
            # 解析英文名称
            if next_line.startswith("英文名称："):
                eng_name = next_line.replace("英文名称：", "").strip()
            
            # 解析隐藏状态
            if "本命令已经被隐藏" in next_line:
                is_hidden = True
            
            # 解析参数可重复
            if "最后一个参数可以被重复添加" in next_line:
                allow_append = True
            
            # 解析操作系统需求
            if next_line.startswith("操作系统需求："):
                os_support = next_line.replace("操作系统需求：", "").strip()
            
            # 解析描述（非参数行的说明文本）
            if not next_line.startswith("参数<") and not next_line.startswith("英文名称") and not next_line.startswith("操作系统需求") and next_line and not next_line.startswith("〈对应命令〉"):
                if not description:
                    description = next_line
                elif len(description) < 500:  # 限制描述长度
                    description += " " + next_line
            
            # 解析参数
            if next_line.startswith("参数<"):
                param = self._parse_parameter(next_line, param_index)
                if param:
                    params.append(param)
                    param_index += 1
            
            i += 1
        
        # 如果没有英文名称，生成一个
        if not eng_name:
            eng_name = f"cmd_{cmd_index}"
        
        return Command(
            index=cmd_index,
            name=cmd_name,
            eng_name=eng_name,
            description=description[:200] if description else "",  # 限制描述长度
            category=category,
            category_index=cat_index,
            return_type=return_type_cn,
            return_type_en=return_type_sdt,
            params=params,
            os_support=os_support,
            is_hidden=is_hidden,
            allow_append=allow_append,
            is_object_method=is_object_method,
            object_type=object_type
        )
    
    def _parse_parameter(self, line: str, index: int) -> Optional[Parameter]:
        """解析参数定义行"""
        # 格式：参数<n>的名称为"xxx"，类型为"xxx（英文）"，可以被省略。说明...
        
        match = re.search(r'参数<(\d+)>的名称为"([^"]+)"，类型为"([^"]+)"', line)
        if not match:
            return None
        
        param_num = int(match.group(1))
        param_name = match.group(2)
        param_type_full = match.group(3)
        
        # 解析类型
        type_sdt, type_en = self.parse_type(param_type_full)
        
        # 提取英文类型标识
        type_cn = param_type_full
        if '（' in param_type_full:
            parts = param_type_full.split('（')
            type_cn = parts[0].strip()
        
        # 检查是否可省略
        optional = "可以被省略" in line
        
        # 检查是否只能提供变量
        only_var = "只能提供变量" in line
        
        # 检查是否可以是数组
        can_be_array = "数组" in line
        
        # 提取默认值
        default_value = ""
        default_match = re.search(r'初始值为"([^"]*)"', line)
        if default_match:
            default_value = default_match.group(1)
        
        # 提取参数说明（在基本信息之后的部分）
        description = ""
        desc_start = line.find("。", line.find("类型为"))
        if desc_start > 0:
            description = line[desc_start+1:].strip()
        
        return Parameter(
            index=index,
            name=param_name,
            type_cn=type_cn,
            type_en=type_sdt,
            description=description[:100] if description else "",
            optional=optional,
            can_be_array=can_be_array,
            only_var=only_var,
            default_value=default_value
        )
    
    def parse_data_types(self):
        """解析数据类型定义"""
        i = 0
        dt_index = 0
        
        while i < len(self.lines):
            line = self.lines[i].strip()
            
            # 检测数据类型定义开始
            if line.startswith("数据类型名称："):
                dt = self._parse_single_datatype(i, dt_index)
                if dt:
                    self.data_types.append(dt)
                    dt_index += 1
            
            i += 1
    
    def _parse_single_datatype(self, start_line: int, dt_index: int) -> Optional[DataType]:
        """解析单个数据类型"""
        line = self.lines[start_line].strip()
        
        # 解析数据类型名称行
        # 格式：数据类型名称：窗口    英文名称：window
        match = re.search(r'数据类型名称：(\S+)\s+英文名称：(\S+)', line)
        if not match:
            # 可能没有英文名称
            match = re.search(r'数据类型名称：(\S+)', line)
            if not match:
                return None
            name = match.group(1)
            eng_name = f"dt_{dt_index}"
        else:
            name = match.group(1)
            eng_name = match.group(2)
        
        dt = DataType(
            index=dt_index,
            name=name,
            eng_name=eng_name
        )
        
        # 继续解析成员属性和事件
        i = start_line + 1
        member_index = 0
        event_index = 0
        
        while i < len(self.lines):
            next_line = self.lines[i].strip()
            
            # 遇到下一个数据类型或命令类别时停止
            if next_line.startswith("数据类型名称：") or next_line.startswith("--- 命令类别："):
                break
            
            # 解析类型种类
            if "本数据类型为" in next_line:
                if "功能窗口组件型" in next_line:
                    dt.type_kind = "功能窗口组件型"
                elif "普通类型" in next_line:
                    dt.type_kind = "普通类型"
            
            # 解析操作系统需求
            if next_line.startswith("操作系统需求："):
                dt.os_support = next_line.replace("操作系统需求：", "").strip()
            
            # 解析成员属性
            if next_line.startswith("成员属性<"):
                member = self._parse_datatype_member(next_line, member_index)
                if member:
                    dt.members.append(member)
                    member_index += 1
            
            # 解析事件
            if next_line.startswith("事件名称："):
                event = DataTypeEvent(
                    index=event_index,
                    name=next_line.replace("事件名称：", "").strip()
                )
                dt.events.append(event)
                event_index += 1
            
            # 解析描述
            if not dt.description and not next_line.startswith("成员属性") and not next_line.startswith("事件") and next_line:
                if "本数据类型为" not in next_line and "操作系统需求" not in next_line:
                    dt.description = next_line[:200]
            
            i += 1
        
        return dt
    
    def _parse_datatype_member(self, line: str, index: int) -> Optional[DataTypeMember]:
        """解析数据类型成员属性"""
        # 格式：成员属性<n>的中文名称为"xxx"，英文名称为"xxx"，类型为"xxx（英文）"。
        match = re.search(r'成员属性<(\d+)>的中文名称为"([^"]+)"，英文名称为"([^"]+)"，类型为"([^"]+)"', line)
        if not match:
            return None
        
        mem_num = int(match.group(1))
        name = match.group(2)
        eng_name = match.group(3)
        type_full = match.group(4)
        
        type_cn = type_full
        if '（' in type_full:
            type_cn = type_full.split('（')[0].strip()
        
        type_sdt, type_en = self.parse_type(type_full)
        
        # 提取描述
        description = ""
        desc_start = line.rfind("。")
        if desc_start > 0 and desc_start < len(line) - 1:
            description = line[desc_start+1:].strip()
        
        return DataTypeMember(
            index=index,
            name=name,
            eng_name=eng_name,
            type_cn=type_cn,
            type_en=type_sdt,
            description=description
        )
    
    def parse_constants(self):
        """解析常量定义"""
        # 查找常量列表部分
        i = 0
        in_const_section = False
        
        while i < len(self.lines):
            line = self.lines[i].strip()
            
            # 检测常量列表开始
            if "--- 库定义常量列表 ---" in line or "库定义常量列表" in line:
                in_const_section = True
                i += 1
                continue
            
            if in_const_section:
                # 解析常量行
                # 格式：名称：xxx    英文名称：xxx
                # 或：名称：xxx
                # 类型：数值型    值：xxx
                if line.startswith("名称："):
                    const = self._parse_single_constant(i)
                    if const:
                        self.constants.append(const)
            
            i += 1
    
    def _parse_single_constant(self, start_line: int) -> Optional[Constant]:
        """解析单个常量"""
        line = self.lines[start_line].strip()
        
        # 解析名称行
        name = ""
        eng_name = ""
        
        # 检查是否有英文名称
        if "英文名称：" in line:
            match = re.search(r'名称：(\S+)\s+英文名称：(\S+)', line)
            if match:
                name = match.group(1)
                eng_name = match.group(2)
        else:
            match = re.search(r'名称：(\S+)', line)
            if match:
                name = match.group(1)
        
        if not name:
            return None
        
        # 查找类型和值
        type_cn = "数值型"
        value = ""
        description = ""
        
        # 检查同一行或下一行
        if "类型：" in line:
            type_match = re.search(r'类型：(\S+)', line)
            if type_match:
                type_cn = type_match.group(1)
            value_match = re.search(r'值：(\S+)', line)
            if value_match:
                value = value_match.group(1)
        
        # 检查下一行
        if start_line + 1 < len(self.lines):
            next_line = self.lines[start_line + 1].strip()
            if next_line.startswith("类型："):
                type_match = re.search(r'类型：(\S+)', next_line)
                if type_match:
                    type_cn = type_match.group(1)
                value_match = re.search(r'值：(\S+)', next_line)
                if value_match:
                    value = value_match.group(1)
            elif not next_line.startswith("名称：") and next_line:
                # 可能是描述
                description = next_line
        
        return Constant(
            name=name,
            eng_name=eng_name,
            type_cn=type_cn,
            value=value,
            description=description[:100] if description else ""
        )
    
    def parse_all(self):
        """解析所有内容"""
        if not self.load_file():
            return False
        
        print("开始解析命令...")
        self.parse_commands()
        print(f"解析到 {len(self.commands)} 个命令")
        print(f"解析到 {len(self.categories)} 个命令类别")
        
        print("开始解析数据类型...")
        self.parse_data_types()
        print(f"解析到 {len(self.data_types)} 个数据类型")
        
        print("开始解析常量...")
        self.parse_constants()
        print(f"解析到 {len(self.constants)} 个常量")
        
        return True
    
    def get_summary(self) -> str:
        """获取解析摘要"""
        summary = []
        summary.append("=" * 60)
        summary.append("系统核心支持库解析摘要")
        summary.append("=" * 60)
        summary.append(f"命令总数: {len(self.commands)}")
        summary.append(f"命令类别: {len(self.categories)}")
        for i, cat in enumerate(self.categories):
            cat_cmds = [c for c in self.commands if c.category == cat]
            summary.append(f"  {i+1}. {cat}: {len(cat_cmds)} 个命令")
        summary.append(f"数据类型总数: {len(self.data_types)}")
        summary.append(f"常量总数: {len(self.constants)}")
        summary.append("=" * 60)
        return "\n".join(summary)


def main():
    """主函数"""
    # 帮助文件路径
    help_file = r"d:\chungbin\ycide\系统核心支持库帮助说明文件.txt"
    
    if not os.path.exists(help_file):
        print(f"文件不存在: {help_file}")
        return
    
    parser = HelpFileParser(help_file)
    if parser.parse_all():
        print(parser.get_summary())
        
        # 输出一些示例
        print("\n前5个命令示例:")
        for cmd in parser.commands[:5]:
            print(f"  [{cmd.index}] {cmd.name} ({cmd.eng_name}) - {cmd.category}")
            print(f"      返回: {cmd.return_type}, 参数数: {len(cmd.params)}")
        
        print("\n前5个数据类型示例:")
        for dt in parser.data_types[:5]:
            print(f"  [{dt.index}] {dt.name} ({dt.eng_name})")
            print(f"      成员数: {len(dt.members)}, 事件数: {len(dt.events)}")
        
        print("\n前5个常量示例:")
        for const in parser.constants[:5]:
            print(f"  {const.name} = {const.value} ({const.type_cn})")

if __name__ == "__main__":
    main()
