# -*- coding: utf-8 -*-
"""
系统核心支持库代码生成器
基于解析结果生成C++代码文件
"""

import os
import re
from datetime import datetime
from parse_help_file import HelpFileParser, Command, DataType, Constant, Parameter

class CodeGenerator:
    """代码生成器"""
    
    # C++关键字安全转换
    RESERVED_WORDS = {
        'if', 'else', 'for', 'while', 'do', 'switch', 'case', 'break', 
        'continue', 'return', 'goto', 'default', 'class', 'struct', 
        'union', 'enum', 'typedef', 'static', 'extern', 'const', 
        'volatile', 'register', 'auto', 'sizeof', 'new', 'delete',
        'true', 'false', 'void', 'int', 'char', 'float', 'double',
        'long', 'short', 'signed', 'unsigned', 'bool', 'wchar_t'
    }
    
    # 类型映射到SDK类型
    TYPE_TO_SDT = {
        "整数型": "SDT_INT",
        "短整数型": "SDT_SHORT", 
        "字节型": "SDT_BYTE",
        "长整数型": "SDT_INT64",
        "小数型": "SDT_FLOAT",
        "双精度小数型": "SDT_DOUBLE",
        "逻辑型": "SDT_BOOL",
        "文本型": "SDT_TEXT",
        "字节集型": "SDT_BIN",
        "字节集": "SDT_BIN",
        "日期时间型": "SDT_DATE_TIME",
        "子程序指针型": "SDT_SUB_PTR",
        "通用型": "_SDT_ALL",
        "无返回值": "_SDT_NULL",
    }
    
    def __init__(self, parser: HelpFileParser, output_dir: str):
        self.parser = parser
        self.output_dir = output_dir
        self.lib_name = "krnln"
        self.lib_name_upper = "KRNLN"
        
    def safe_identifier(self, name: str) -> str:
        """生成安全的C++标识符"""
        # 移除非法字符
        safe = re.sub(r'[^a-zA-Z0-9_]', '', name)
        # 确保不以数字开头
        if safe and safe[0].isdigit():
            safe = '_' + safe
        # 处理保留字
        if safe.lower() in self.RESERVED_WORDS:
            safe = safe + '_'
        return safe or 'unnamed'
    
    def get_sdt_type(self, type_cn: str) -> str:
        """获取SDT类型常量"""
        for cn, sdt in self.TYPE_TO_SDT.items():
            if cn in type_cn:
                return sdt
        return "_SDT_ALL"
    
    def escape_string(self, s: str) -> str:
        """转义字符串用于C++"""
        if not s:
            return ""
        s = s.replace('\\', '\\\\')
        s = s.replace('"', '\\"')
        s = s.replace('\n', '\\n')
        s = s.replace('\r', '\\r')
        s = s.replace('\t', '\\t')
        return s
    
    def generate_header(self) -> str:
        """生成头文件 include_krnln_header.h"""
        lines = []
        lines.append(f"// 系统核心支持库头文件")
        lines.append(f"// 自动生成于 {datetime.now().strftime('%Y-%m-%d %H:%M:%S')}")
        lines.append(f"// 支持 Unicode 和 x64")
        lines.append("")
        lines.append("#ifndef __INCLUDE_KRNLN_HEADER_H__")
        lines.append("#define __INCLUDE_KRNLN_HEADER_H__")
        lines.append("")
        lines.append("// 定义库名称宏，必须在包含 lib2.h 之前定义")
        lines.append("#define __E_FNENAME krnln")
        lines.append("")
        lines.append('#include "elib/lib2.h"')
        lines.append('#include "elib/lang.h"')
        lines.append('#include "elib/krnllib.h"')
        lines.append('#include "krnln_cmd_typedef.h"')
        lines.append("")
        lines.append("#define KRNLN_EXTERN_C EXTERN_C")
        lines.append("")
        lines.append("// 控件基类结构")
        lines.append("typedef struct CONTROL_STRUCT_BASE")
        lines.append("{")
        lines.append("    WNDPROC             oldProc;")
        lines.append("    HFONT               hFont;")
        lines.append("    HWND                hDesignWnd;")
        lines.append("    HWND                hWndParent;")
        lines.append("    HWND                hWnd;")
        lines.append("    DWORD               hUnit;")
        lines.append("    UINT                id;")
        lines.append("    DWORD               dwWinFormID;")
        lines.append("    DWORD               dwUnitID;")
        lines.append("    BOOL                blInDesignMode;")
        lines.append("    LONG                flags;")
        lines.append("    DWORD               style;")
        lines.append("    DWORD               styleEx;")
        lines.append("    DWORD               styleCtl;")
        lines.append("}*PCONTROL_STRUCT_BASE;")
        lines.append("")
        lines.append("#ifndef __E_STATIC_LIB")
        lines.append("extern LIB_CONST_INFO g_ConstInfo_krnln_global_var[];")
        lines.append("extern int g_ConstInfo_krnln_global_var_count;")
        lines.append("extern CMD_INFO g_cmdInfo_krnln_global_var[];")
        lines.append("extern PFN_EXECUTE_CMD g_cmdInfo_krnln_global_var_fun[];")
        lines.append("extern int g_cmdInfo_krnln_global_var_count;")
        lines.append("extern ARG_INFO g_argumentInfo_krnln_global_var[];")
        lines.append("extern LIB_DATA_TYPE_INFO g_DataType_krnln_global_var[];")
        lines.append("extern int g_DataType_krnln_global_var_count;")
        lines.append("#endif")
        lines.append("")
        lines.append("// 命令函数声明宏 - _szFuncId用于生成函数名(避免C++关键字), _szEgName用于显示")
        lines.append("#define KRNLN_DEF_CMD(_index, _szName, _szEgName, _szFuncId, _szExplain, _shtCategory, _wState, _dtRetValType, _wReserved, _shtUserLevel, _shtBitmapIndex, _shtBitmapCount, _nArgCount, _pBeginArgInfo) \\")
        lines.append("    EXTERN_C void KRNLN_NAME(_index, _szFuncId)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf);")
        lines.append("KRNLN_DEF(KRNLN_DEF_CMD)")
        lines.append("")
        lines.append("#endif // __INCLUDE_KRNLN_HEADER_H__")
        
        return "\n".join(lines)
    
    def generate_cmd_typedef(self) -> str:
        """生成命令类型定义文件 krnln_cmd_typedef.h"""
        lines = []
        lines.append("#pragma once")
        lines.append("")
        lines.append(f"// 系统核心支持库命令类型定义")
        lines.append(f"// 自动生成于 {datetime.now().strftime('%Y-%m-%d %H:%M:%S')}")
        lines.append(f"// 命令总数: {len(self.parser.commands)}")
        lines.append("")
        lines.append("#define __KRNLN_NAME(_index, _name) __LIB2_FNE_NAME_LEFT(__E_FNENAME)##_##_name##_##_index##_")
        lines.append("")
        lines.append("// 根据函数索引和名称拼接完整函数名")
        lines.append("#define KRNLN_NAME(_index, _name) __LIB2_FNE_NAME_LEFT(__KRNLN_NAME(_index, _name))__LIB2_FNE_NAME_LEFT(__E_FNENAME)")
        lines.append("")
        lines.append("// 生成函数名字符串")
        lines.append('#define KRNLN_NAME_STR(_index, _name) ______E_FNENAME(__KRNLN_NAME(_index, _name))')
        lines.append("")
        lines.append("// 命令定义宏")
        lines.append("// 参数说明:")
        lines.append("//  0 _index = 命令索引")
        lines.append("//  1 _szName = 命令中文名称")
        lines.append("//  2 _szEgName = 命令英文名称(显示用)")
        lines.append("//  3 _szFuncId = 函数标识符(用于生成C++函数名,避免关键字)")
        lines.append("//  4 _szExplain = 命令详细说明")
        lines.append("//  5 _shtCategory = 命令类别索引(从1开始)")
        lines.append("//  6 _wState = 命令状态标志")
        lines.append("//  7 _dtRetValType = 返回值类型")
        lines.append("//  8 _wReserved = 保留字段")
        lines.append("//  9 _shtUserLevel = 用户级别")
        lines.append("// 10 _shtBitmapIndex = 图标索引")
        lines.append("// 11 _shtBitmapCount = 图标数目")
        lines.append("// 12 _nArgCount = 参数数目")
        lines.append("// 13 _pBeginArgInfo = 参数信息起始地址")
        lines.append("")
        lines.append("#define KRNLN_DEF(_MAKE) \\")
        
        # 生成所有命令定义
        for i, cmd in enumerate(self.parser.commands):
            # 原始英文名(用于显示)
            orig_eng_name = cmd.eng_name if cmd.eng_name else f"cmd_{i}"
            # 安全的函数标识符(避免C++关键字)
            safe_name = self.safe_identifier(cmd.eng_name) if cmd.eng_name else f"cmd_{i}"
            desc = self.escape_string(cmd.description[:80]) if cmd.description else ""
            
            # 计算状态标志
            state_flags = ["_CMD_OS(__OS_WIN)"]
            if cmd.is_hidden:
                state_flags.append("CT_IS_HIDED")
            if cmd.allow_append:
                state_flags.append("CT_ALLOW_APPEND_NEW_ARG")
            state = " | ".join(state_flags)
            
            # 返回类型
            ret_type = self.get_sdt_type(cmd.return_type)
            
            # 参数信息
            param_count = len(cmd.params)
            param_ptr = "NULL" if param_count == 0 else f"g_argumentInfo_krnln_global_var + {self._get_arg_offset(i)}"
            
            # 类别索引
            cat_idx = cmd.category_index if cmd.category_index > 0 else 1
            
            # 转义反斜杠 - 添加原始英文名和安全标识符两个参数
            line = f'    _MAKE({i:4d}, "{cmd.name}", "{orig_eng_name}", {safe_name}, "{desc}", {cat_idx}, {state}, {ret_type}, 0, LVL_SIMPLE, 0, 0, {param_count}, {param_ptr})'
            
            if i < len(self.parser.commands) - 1:
                line += "\\"
            
            lines.append(line)
        
        lines.append("")
        
        return "\n".join(lines)
    
    def _get_arg_offset(self, cmd_index: int) -> int:
        """计算参数在全局参数数组中的偏移"""
        offset = 0
        for i in range(cmd_index):
            offset += len(self.parser.commands[i].params)
        return offset
    
    def generate_cmdInfo(self) -> str:
        """生成命令信息文件 krnln_cmdInfo.cpp"""
        lines = []
        lines.append("#if !defined(__E_STATIC_LIB)")
        lines.append('#include "include_krnln_header.h"')
        lines.append("")
        lines.append(f"// 系统核心支持库命令信息")
        lines.append(f"// 自动生成于 {datetime.now().strftime('%Y-%m-%d %H:%M:%S')}")
        lines.append("")
        
        # 生成参数信息数组
        lines.append("// 参数信息数组")
        lines.append("static ARG_INFO g_argumentInfo_krnln_global_var[] = ")
        lines.append("{")
        lines.append("    // 1=参数名称, 2=参数详细说明, 3=指定图标索引, 4=图标数目")
        lines.append("    // 5=参数类型, 6=参数默认值, 7=参数标志")
        
        arg_index = 0
        for cmd in self.parser.commands:
            if cmd.params:
                lines.append(f"    // 命令 {cmd.index}: {cmd.name}")
                for param in cmd.params:
                    param_type = self.get_sdt_type(param.type_cn)
                    
                    # 计算参数标志
                    flags = []
                    if param.optional:
                        flags.append("AS_DEFAULT_VALUE_IS_EMPTY")
                    if param.only_var:
                        flags.append("AS_RECEIVE_VAR")
                    if param.can_be_array:
                        flags.append("AS_RECEIVE_ALL_TYPE_DATA")
                    
                    flag_str = " | ".join(flags) if flags else "NULL"
                    
                    desc = self.escape_string(param.description[:60]) if param.description else ""
                    # 默认值字段是INT类型，需要转换字符串为整数，如果无法转换则使用0
                    default_val = "0"
                    if param.default_value:
                        try:
                            default_val = str(int(param.default_value))
                        except ValueError:
                            default_val = "0"
                    
                    lines.append(f'    /*{arg_index:04d}*/ {{"{param.name}", "{desc}", 0, 0, {param_type}, {default_val}, {flag_str}}},')
                    arg_index += 1
        
        # 添加一个空项防止数组为空
        if arg_index == 0:
            lines.append('    /*0000*/ {"", NULL, 0, 0, _SDT_ALL, 0, NULL},')
        
        lines.append("};")
        lines.append("")
        lines.append("#ifdef _DEBUG")
        lines.append("const int dbg_cmd_arg_count__ = sizeof(g_argumentInfo_krnln_global_var) / sizeof(g_argumentInfo_krnln_global_var[0]);")
        lines.append("#endif")
        lines.append("")
        
        # 生成命令信息定义宏 - 使用原始英文名_szEgName用于显示, _szFuncId被忽略(只用于函数名生成)
        lines.append("#define KRNLN_DEF_CMDINFO(_index, _szName, _szEgName, _szFuncId, _szExplain, _shtCategory, _wState, _dtRetValType, _wReserved, _shtUserLevel, _shtBitmapIndex, _shtBitmapCount, _nArgCount, _pBeginArgInfo) \\")
        lines.append('    { _szName, _szEgName, _szExplain, _shtCategory, _wState, _dtRetValType, _wReserved, _shtUserLevel, _shtBitmapIndex, _shtBitmapCount, _nArgCount, _pBeginArgInfo },')
        lines.append("")
        lines.append("// 命令信息数组")
        lines.append("CMD_INFO g_cmdInfo_krnln_global_var[] = ")
        lines.append("{")
        lines.append("    KRNLN_DEF(KRNLN_DEF_CMDINFO)")
        lines.append("};")
        lines.append("")
        lines.append("int g_cmdInfo_krnln_global_var_count = sizeof(g_cmdInfo_krnln_global_var) / sizeof(g_cmdInfo_krnln_global_var[0]);")
        lines.append("")
        lines.append("#endif")
        lines.append("")
        
        return "\n".join(lines)
    
    def generate_const(self) -> str:
        """生成常量定义文件 krnln_const.cpp"""
        lines = []
        lines.append('#include "include_krnln_header.h"')
        lines.append("")
        lines.append(f"// 系统核心支持库常量定义")
        lines.append(f"// 自动生成于 {datetime.now().strftime('%Y-%m-%d %H:%M:%S')}")
        lines.append(f"// 常量总数: {len(self.parser.constants)}")
        lines.append("")
        lines.append("// 常量类型: CT_NULL=空常量, CT_NUM=数值型, CT_BOOL=逻辑型, CT_TEXT=文本型")
        lines.append("")
        lines.append("#ifndef __E_STATIC_LIB")
        lines.append("")
        lines.append("LIB_CONST_INFO g_ConstInfo_krnln_global_var[] = ")
        lines.append("{")
        
        for const in self.parser.constants:
            # 确定常量类型
            if "文本" in const.type_cn:
                const_type = "CT_TEXT"
                value_str = f'"{self.escape_string(const.value)}", 0.0'
            elif "逻辑" in const.type_cn:
                const_type = "CT_BOOL"
                val = 1.0 if const.value.lower() in ['真', 'true', '1'] else 0.0
                value_str = f'NULL, {val}'
            else:
                const_type = "CT_NUM"
                try:
                    val = float(const.value) if const.value else 0.0
                except ValueError:
                    val = 0.0
                value_str = f'NULL, {val}'
            
            eng_name = const.eng_name if const.eng_name else ""
            desc = self.escape_string(const.description[:50]) if const.description else ""
            
            lines.append(f'    {{"{const.name}", "{eng_name}", "{desc}", 1, {const_type}, {value_str}}},')
        
        # 添加空项防止数组为空
        if not self.parser.constants:
            lines.append('    {"", "", "", 1, CT_NULL, NULL, 0.0},')
        
        lines.append("};")
        lines.append("")
        lines.append(f"int g_ConstInfo_krnln_global_var_count = {len(self.parser.constants) if self.parser.constants else 0};")
        lines.append("")
        lines.append("#endif")
        lines.append("")
        
        return "\n".join(lines)
    
    def generate_dtType(self) -> str:
        """生成数据类型定义文件 krnln_dtType.cpp"""
        lines = []
        lines.append("#ifndef __E_STATIC_LIB")
        lines.append('#include "include_krnln_header.h"')
        lines.append("")
        lines.append(f"// 系统核心支持库数据类型定义")
        lines.append(f"// 自动生成于 {datetime.now().strftime('%Y-%m-%d %H:%M:%S')}")
        lines.append(f"// 数据类型总数: {len(self.parser.data_types)}")
        lines.append("")
        lines.append("// 数据类型标志: LDT_WIN_UNIT=窗口组件, LDT_IS_CYCLEREF=支持循环引用")
        lines.append("")
        
        # 如果有数据类型，生成成员信息
        if self.parser.data_types:
            lines.append("// 数据类型成员属性信息")
            for dt in self.parser.data_types:
                if dt.members:
                    lines.append(f"static LIB_DATA_TYPE_ELEMENT g_dtElements_{dt.index}[] = ")
                    lines.append("{")
                    for mem in dt.members:
                        mem_type = self.get_sdt_type(mem.type_cn)
                        desc = self.escape_string(mem.description[:50]) if mem.description else ""
                        lines.append(f'    {{"{mem.name}", "{mem.eng_name}", "{desc}", {mem_type}, 0, NULL, 0}},')
                    lines.append("};")
                    lines.append("")
        
        lines.append("// 数据类型信息数组")
        lines.append("LIB_DATA_TYPE_INFO g_DataType_krnln_global_var[] = ")
        lines.append("{")
        
        for dt in self.parser.data_types:
            # 数据类型标志
            flags = []
            if dt.type_kind == "功能窗口组件型":
                flags.append("LDT_WIN_UNIT")
            flag_str = " | ".join(flags) if flags else "0"
            
            desc = self.escape_string(dt.description[:60]) if dt.description else ""
            
            # 字段顺序: 名称, 英文名, 说明, 命令数, 命令索引指针, 状态标志, 位图ID,
            # 对于窗口组件: 事件数, 事件指针, 属性数, 属性指针, 接口函数
            # 对于普通类型: 成员数, 成员指针
            
            member_count = len(dt.members)
            member_ptr = f"g_dtElements_{dt.index}" if dt.members else "NULL"
            event_count = len(dt.events)
            
            # m_szName, m_szEgName, m_szExplain, m_nCmdCount, m_pnCmdsIndex, m_dwState, m_dwUnitBmpID, 
            # m_nEventCount, m_pEventBegin, m_nPropertyCount, m_pPropertyBegin, m_pfnGetInterface
            # 或: m_nElementCount, m_pElementBegin (对于非窗口组件)
            
            lines.append(f'    {{"{dt.name}", "{dt.eng_name}", "{desc}", 0, NULL, {flag_str}, 0, {event_count}, (PEVENT_INFO2)NULL, 0, NULL, NULL}},')
        
        # 添加空项防止数组为空
        if not self.parser.data_types:
            lines.append('    {"", "", "", 0, NULL, 0, 0, 0, NULL, 0, NULL, NULL},')
        
        lines.append("};")
        lines.append("")
        lines.append(f"int g_DataType_krnln_global_var_count = {len(self.parser.data_types) if self.parser.data_types else 0};")
        lines.append("")
        lines.append("#endif")
        lines.append("")
        
        return "\n".join(lines)
    
    def generate_dllMain(self) -> str:
        """生成DLL主入口文件 krnln_dllMain.cpp"""
        lines = []
        lines.append('#include "include_krnln_header.h"')
        lines.append('#include "elib/fnshare.h"')
        lines.append('#include "elib/lang.h"')
        lines.append('#pragma comment(lib, "legacy_stdio_definitions.lib")')
        lines.append("")
        lines.append(f"// 系统核心支持库主入口")
        lines.append(f"// 自动生成于 {datetime.now().strftime('%Y-%m-%d %H:%M:%S')}")
        lines.append("")
        lines.append("EXTERN_C INT_PTR WINAPI krnln_ProcessNotifyLib(INT nMsg, DWORD_PTR dwParam1, DWORD_PTR dwParam2);")
        lines.append("")
        lines.append("#ifndef __E_STATIC_LIB")
        lines.append("BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)")
        lines.append("{")
        lines.append("    switch (ul_reason_for_call)")
        lines.append("    {")
        lines.append("    case DLL_PROCESS_ATTACH:")
        lines.append("        break;")
        lines.append("    case DLL_PROCESS_DETACH:")
        lines.append("        break;")
        lines.append("    case DLL_THREAD_ATTACH:")
        lines.append("        break;")
        lines.append("    case DLL_THREAD_DETACH:")
        lines.append("        break;")
        lines.append("    }")
        lines.append("    return TRUE;")
        lines.append("}")
        lines.append("")
        
        # 生成命令函数指针数组 - 使用_szFuncId生成函数名
        lines.append("#define KRNLN_DEF_CMD_PTR(_index, _szName, _szEgName, _szFuncId, _szExplain, _shtCategory, _wState, _dtRetValType, _wReserved, _shtUserLevel, _shtBitmapIndex, _shtBitmapCount, _nArgCount, _pBeginArgInfo) \\")
        lines.append("    KRNLN_NAME(_index, _szFuncId),")
        lines.append("")
        lines.append("PFN_EXECUTE_CMD g_cmdInfo_krnln_global_var_fun[] = { KRNLN_DEF(KRNLN_DEF_CMD_PTR) };")
        lines.append("")
        
        # 生成库信息结构
        lines.append("static LIB_INFO g_LibInfo_krnln_global_var = ")
        lines.append("{")
        lines.append('    /*m_dwLibFormatVer*/        LIB_FORMAT_VER,')
        lines.append('    /*m_szGuid*/                "D09F2340818511D396F6AE4C17150413",  // 系统核心支持库GUID')
        lines.append('    /*m_nMajorVersion*/         5,')
        lines.append('    /*m_nMinorVersion*/         7,')
        lines.append('    /*m_nBuildNumber*/          0,')
        lines.append("")
        lines.append('    /*m_nRqSysMajorVer*/        3,')
        lines.append('    /*m_nRqSysMinorVer*/        7,')
        lines.append('    /*m_nRqSysKrnlLibMajorVer*/ 3,')
        lines.append('    /*m_nRqSysKrnlLibMinorVer*/ 0,')
        lines.append("")
        lines.append('    /*m_szName*/                "系统核心支持库",')
        lines.append('    /*m_nLanguage*/             __GBK_LANG_VER,')
        lines.append('    /*m_szExplain*/             "本支持库是易语言的核心库，为系统本身和每个易程序提供必需的功能支持。支持Unicode和x64。",')
        lines.append('    /*m_dwState*/               _LIB_OS(OS_ALL),')
        lines.append("")
        lines.append('    /*m_szAuthor*/              "大有吴涛易语言软件公司",')
        lines.append('    /*m_szZipCode*/             "116001",')
        lines.append('    /*m_szAddress*/             "辽宁省大连市中山区人民路55号亚太国际金融中心",')
        lines.append('    /*m_szPhoto*/               "+86(0411)39895831",')
        lines.append('    /*m_szFax*/                 "+86(0411)39895834",')
        lines.append('    /*m_szEmail*/               "service@dywt.com.cn",')
        lines.append('    /*m_szHomePage*/            "http://www.eyuyan.com",')
        lines.append('    /*m_szOther*/               "支持Unicode和x64位重编译版本",')
        lines.append("")
        lines.append('    /*m_nDataTypeCount*/        g_DataType_krnln_global_var_count,')
        lines.append('    /*g_DataType_xxx*/          g_DataType_krnln_global_var,')
        lines.append("")
        
        # 生成类别字符串
        cat_str = ""
        for cat in self.parser.categories:
            cat_str += f'"0000{cat}\\0"'
        if not cat_str:
            cat_str = '"0000默认\\0"'
        cat_str += '"\\0"'
        
        lines.append(f'    /*m_nCategoryCount*/        {len(self.parser.categories) if self.parser.categories else 1},')
        lines.append(f'    /*m_szzCategory*/           {cat_str},')
        lines.append("")
        lines.append('    /*m_nCmdCount*/             g_cmdInfo_krnln_global_var_count,')
        lines.append('    /*m_pBeginCmdInfo*/         g_cmdInfo_krnln_global_var,')
        lines.append('    /*m_pCmdsFunc*/             g_cmdInfo_krnln_global_var_fun,')
        lines.append("")
        lines.append('    /*m_pfnRunAddInFn*/         NULL,')
        lines.append('    /*m_szzAddInFnInfo*/        NULL,')
        lines.append("")
        lines.append('    /*m_pfnNotify*/             krnln_ProcessNotifyLib,')
        lines.append("")
        lines.append('    /*m_pfnSuperTemplate*/      NULL,')
        lines.append('    /*m_szzSuperTemplateInfo*/  NULL,')
        lines.append("")
        lines.append('    /*m_nLibConstCount*/        g_ConstInfo_krnln_global_var_count,')
        lines.append('    /*m_pLibConst*/             g_ConstInfo_krnln_global_var,')
        lines.append("")
        lines.append('    /*m_szzDependFiles*/        NULL')
        lines.append("};")
        lines.append("")
        
        lines.append("EXTERN_C PLIB_INFO WINAPI GetNewInf()")
        lines.append("{")
        lines.append("    return &g_LibInfo_krnln_global_var;")
        lines.append("}")
        lines.append("")
        
        # 生成命令名称字符串数组
        lines.append("#define KRNLN_DEF_CMDNAME_STR(_index, _szName, _szEgName, _szFuncId, _szExplain, _shtCategory, _wState, _dtRetValType, _wReserved, _shtUserLevel, _shtBitmapIndex, _shtBitmapCount, _nArgCount, _pBeginArgInfo) \\")
        lines.append('    ______E_FNENAME(KRNLN_NAME(_index, _szFuncId)),')
        lines.append("")
        lines.append("static const LPCSTR g_cmdNamesKrnln[] = { KRNLN_DEF(KRNLN_DEF_CMDNAME_STR) };")
        lines.append("")
        lines.append("#endif")
        lines.append("")
        
        # 生成通知处理函数
        lines.append("EXTERN_C INT_PTR WINAPI krnln_ProcessNotifyLib(INT nMsg, DWORD_PTR dwParam1, DWORD_PTR dwParam2)")
        lines.append("{")
        lines.append("    INT_PTR nRet = NR_OK;")
        lines.append("    switch (nMsg)")
        lines.append("    {")
        lines.append("#ifndef __E_STATIC_LIB")
        lines.append("    case NL_GET_CMD_FUNC_NAMES:")
        lines.append("        return (INT_PTR)g_cmdNamesKrnln;")
        lines.append("    case NL_GET_NOTIFY_LIB_FUNC_NAME:")
        lines.append('        return (INT_PTR)"krnln_ProcessNotifyLib";')
        lines.append("    case NL_GET_DEPENDENT_LIBS:")
        lines.append('        return (INT_PTR)"\\0\\0";')
        lines.append("#endif")
        lines.append("    case NL_SYS_NOTIFY_FUNCTION:")
        lines.append("        ProcessNotifyLib(nMsg, dwParam1, dwParam2);")
        lines.append("        break;")
        lines.append("    case NL_FREE_LIB_DATA:")
        lines.append("        break;")
        lines.append("    case NL_UNLOAD_FROM_IDE:")
        lines.append("        break;")
        lines.append("    default:")
        lines.append("        nRet = NR_ERR;")
        lines.append("        break;")
        lines.append("    }")
        lines.append("    return nRet;")
        lines.append("}")
        lines.append("")
        
        return "\n".join(lines)
    
    def generate_cmd_impl(self) -> str:
        """生成命令实现文件 krnln_cmd_impl.cpp"""
        lines = []
        lines.append('#include "include_krnln_header.h"')
        lines.append("")
        lines.append(f"// 系统核心支持库命令实现")
        lines.append(f"// 自动生成于 {datetime.now().strftime('%Y-%m-%d %H:%M:%S')}")
        lines.append(f"// 命令总数: {len(self.parser.commands)}")
        lines.append("")
        lines.append("// 注意: 此文件包含所有命令的占位实现")
        lines.append("// 需要根据实际功能逐个实现")
        lines.append("")
        
        # 为每个命令生成占位实现
        for cmd in self.parser.commands:
            safe_name = self.safe_identifier(cmd.eng_name) if cmd.eng_name else f"cmd_{cmd.index}"
            
            lines.append(f"// 命令 {cmd.index}: {cmd.name}")
            lines.append(f"// 类别: {cmd.category}")
            if cmd.description:
                lines.append(f"// 说明: {cmd.description[:80]}...")
            
            lines.append(f"EXTERN_C void KRNLN_NAME({cmd.index}, {safe_name})(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)")
            lines.append("{")
            lines.append("    // TODO: 实现此命令")
            
            # 根据返回类型生成默认返回值
            if "整数" in cmd.return_type:
                lines.append("    pRetData->m_int = 0;")
            elif "小数" in cmd.return_type or "双精度" in cmd.return_type:
                lines.append("    pRetData->m_double = 0.0;")
            elif "文本" in cmd.return_type:
                lines.append('    pRetData->m_pText = NULL;')
            elif "逻辑" in cmd.return_type:
                lines.append("    pRetData->m_bool = FALSE;")
            elif "字节集" in cmd.return_type:
                lines.append("    pRetData->m_pBin = NULL;")
            
            lines.append("}")
            lines.append("")
        
        return "\n".join(lines)
    
    def generate_def_file(self) -> str:
        """生成模块定义文件 Source_krnln.def"""
        lines = []
        lines.append("; 系统核心支持库模块定义文件")
        lines.append(f"; 自动生成于 {datetime.now().strftime('%Y-%m-%d %H:%M:%S')}")
        lines.append("")
        lines.append("LIBRARY krnln")
        lines.append("EXPORTS")
        lines.append("    GetNewInf @1")
        
        return "\n".join(lines)
    
    def generate_vcxproj(self) -> str:
        """生成Visual Studio项目文件"""
        return '''<?xml version="1.0" encoding="utf-8"?>
<Project DefaultTargets="Build" xmlns="http://schemas.microsoft.com/developer/msbuild/2003">
  <ItemGroup Label="ProjectConfigurations">
    <ProjectConfiguration Include="Debug|Win32">
      <Configuration>Debug</Configuration>
      <Platform>Win32</Platform>
    </ProjectConfiguration>
    <ProjectConfiguration Include="Release|Win32">
      <Configuration>Release</Configuration>
      <Platform>Win32</Platform>
    </ProjectConfiguration>
    <ProjectConfiguration Include="Debug|x64">
      <Configuration>Debug</Configuration>
      <Platform>x64</Platform>
    </ProjectConfiguration>
    <ProjectConfiguration Include="Release|x64">
      <Configuration>Release</Configuration>
      <Platform>x64</Platform>
    </ProjectConfiguration>
  </ItemGroup>
  <PropertyGroup Label="Globals">
    <VCProjectVersion>16.0</VCProjectVersion>
    <Keyword>Win32Proj</Keyword>
    <ProjectGuid>{D09F2340-8185-11D3-96F6-AE4C17150413}</ProjectGuid>
    <RootNamespace>krnln</RootNamespace>
    <WindowsTargetPlatformVersion>10.0</WindowsTargetPlatformVersion>
    <ProjectName>krnln</ProjectName>
  </PropertyGroup>
  <Import Project="$(VCTargetsPath)\\Microsoft.Cpp.Default.props" />
  <PropertyGroup Condition="'$(Configuration)|$(Platform)'=='Debug|Win32'" Label="Configuration">
    <ConfigurationType>DynamicLibrary</ConfigurationType>
    <UseDebugLibraries>true</UseDebugLibraries>
    <PlatformToolset>v143</PlatformToolset>
    <CharacterSet>Unicode</CharacterSet>
  </PropertyGroup>
  <PropertyGroup Condition="'$(Configuration)|$(Platform)'=='Release|Win32'" Label="Configuration">
    <ConfigurationType>DynamicLibrary</ConfigurationType>
    <UseDebugLibraries>false</UseDebugLibraries>
    <PlatformToolset>v143</PlatformToolset>
    <WholeProgramOptimization>true</WholeProgramOptimization>
    <CharacterSet>Unicode</CharacterSet>
  </PropertyGroup>
  <PropertyGroup Condition="'$(Configuration)|$(Platform)'=='Debug|x64'" Label="Configuration">
    <ConfigurationType>DynamicLibrary</ConfigurationType>
    <UseDebugLibraries>true</UseDebugLibraries>
    <PlatformToolset>v143</PlatformToolset>
    <CharacterSet>Unicode</CharacterSet>
  </PropertyGroup>
  <PropertyGroup Condition="'$(Configuration)|$(Platform)'=='Release|x64'" Label="Configuration">
    <ConfigurationType>DynamicLibrary</ConfigurationType>
    <UseDebugLibraries>false</UseDebugLibraries>
    <PlatformToolset>v143</PlatformToolset>
    <WholeProgramOptimization>true</WholeProgramOptimization>
    <CharacterSet>Unicode</CharacterSet>
  </PropertyGroup>
  <Import Project="$(VCTargetsPath)\\Microsoft.Cpp.props" />
  <ImportGroup Label="ExtensionSettings">
  </ImportGroup>
  <ImportGroup Label="Shared">
  </ImportGroup>
  <PropertyGroup Label="UserMacros" />
  <PropertyGroup Condition="'$(Configuration)|$(Platform)'=='Debug|Win32'">
    <LinkIncremental>true</LinkIncremental>
    <OutDir>$(SolutionDir)$(Platform)\\$(Configuration)\\</OutDir>
    <IntDir>$(Platform)\\$(Configuration)\\</IntDir>
    <TargetExt>.fne</TargetExt>
  </PropertyGroup>
  <PropertyGroup Condition="'$(Configuration)|$(Platform)'=='Release|Win32'">
    <LinkIncremental>false</LinkIncremental>
    <OutDir>$(SolutionDir)$(Platform)\\$(Configuration)\\</OutDir>
    <IntDir>$(Platform)\\$(Configuration)\\</IntDir>
    <TargetExt>.fne</TargetExt>
  </PropertyGroup>
  <PropertyGroup Condition="'$(Configuration)|$(Platform)'=='Debug|x64'">
    <LinkIncremental>true</LinkIncremental>
    <OutDir>$(SolutionDir)$(Platform)\\$(Configuration)\\</OutDir>
    <IntDir>$(Platform)\\$(Configuration)\\</IntDir>
    <TargetExt>.fne</TargetExt>
  </PropertyGroup>
  <PropertyGroup Condition="'$(Configuration)|$(Platform)'=='Release|x64'">
    <LinkIncremental>false</LinkIncremental>
    <OutDir>$(SolutionDir)$(Platform)\\$(Configuration)\\</OutDir>
    <IntDir>$(Platform)\\$(Configuration)\\</IntDir>
    <TargetExt>.fne</TargetExt>
  </PropertyGroup>
  <ItemDefinitionGroup Condition="'$(Configuration)|$(Platform)'=='Debug|Win32'">
    <ClCompile>
      <WarningLevel>Level3</WarningLevel>
      <SDLCheck>true</SDLCheck>
      <PreprocessorDefinitions>WIN32;_DEBUG;_WINDOWS;_USRDLL;KRNLN_EXPORTS;%(PreprocessorDefinitions)</PreprocessorDefinitions>
      <ConformanceMode>true</ConformanceMode>
      <PrecompiledHeader>NotUsing</PrecompiledHeader>
      <RuntimeLibrary>MultiThreadedDebugDLL</RuntimeLibrary>
      <AdditionalIncludeDirectories>$(ProjectDir);%(AdditionalIncludeDirectories)</AdditionalIncludeDirectories>
      <AdditionalOptions>/utf-8 %(AdditionalOptions)</AdditionalOptions>
    </ClCompile>
    <Link>
      <SubSystem>Windows</SubSystem>
      <GenerateDebugInformation>true</GenerateDebugInformation>
      <ModuleDefinitionFile>Source_krnln.def</ModuleDefinitionFile>
    </Link>
  </ItemDefinitionGroup>
  <ItemDefinitionGroup Condition="'$(Configuration)|$(Platform)'=='Release|Win32'">
    <ClCompile>
      <WarningLevel>Level3</WarningLevel>
      <FunctionLevelLinking>true</FunctionLevelLinking>
      <IntrinsicFunctions>true</IntrinsicFunctions>
      <SDLCheck>true</SDLCheck>
      <PreprocessorDefinitions>WIN32;NDEBUG;_WINDOWS;_USRDLL;KRNLN_EXPORTS;%(PreprocessorDefinitions)</PreprocessorDefinitions>
      <ConformanceMode>true</ConformanceMode>
      <PrecompiledHeader>NotUsing</PrecompiledHeader>
      <RuntimeLibrary>MultiThreadedDLL</RuntimeLibrary>
      <AdditionalIncludeDirectories>$(ProjectDir);%(AdditionalIncludeDirectories)</AdditionalIncludeDirectories>
      <AdditionalOptions>/utf-8 %(AdditionalOptions)</AdditionalOptions>
    </ClCompile>
    <Link>
      <SubSystem>Windows</SubSystem>
      <EnableCOMDATFolding>true</EnableCOMDATFolding>
      <OptimizeReferences>true</OptimizeReferences>
      <GenerateDebugInformation>true</GenerateDebugInformation>
      <ModuleDefinitionFile>Source_krnln.def</ModuleDefinitionFile>
    </Link>
  </ItemDefinitionGroup>
  <ItemDefinitionGroup Condition="'$(Configuration)|$(Platform)'=='Debug|x64'">
    <ClCompile>
      <WarningLevel>Level3</WarningLevel>
      <SDLCheck>true</SDLCheck>
      <PreprocessorDefinitions>_DEBUG;_WINDOWS;_USRDLL;KRNLN_EXPORTS;%(PreprocessorDefinitions)</PreprocessorDefinitions>
      <ConformanceMode>true</ConformanceMode>
      <PrecompiledHeader>NotUsing</PrecompiledHeader>
      <RuntimeLibrary>MultiThreadedDebugDLL</RuntimeLibrary>
      <AdditionalIncludeDirectories>$(ProjectDir);%(AdditionalIncludeDirectories)</AdditionalIncludeDirectories>
      <AdditionalOptions>/utf-8 %(AdditionalOptions)</AdditionalOptions>
    </ClCompile>
    <Link>
      <SubSystem>Windows</SubSystem>
      <GenerateDebugInformation>true</GenerateDebugInformation>
      <ModuleDefinitionFile>Source_krnln.def</ModuleDefinitionFile>
    </Link>
  </ItemDefinitionGroup>
  <ItemDefinitionGroup Condition="'$(Configuration)|$(Platform)'=='Release|x64'">
    <ClCompile>
      <WarningLevel>Level3</WarningLevel>
      <FunctionLevelLinking>true</FunctionLevelLinking>
      <IntrinsicFunctions>true</IntrinsicFunctions>
      <SDLCheck>true</SDLCheck>
      <PreprocessorDefinitions>NDEBUG;_WINDOWS;_USRDLL;KRNLN_EXPORTS;%(PreprocessorDefinitions)</PreprocessorDefinitions>
      <ConformanceMode>true</ConformanceMode>
      <PrecompiledHeader>NotUsing</PrecompiledHeader>
      <RuntimeLibrary>MultiThreadedDLL</RuntimeLibrary>
      <AdditionalIncludeDirectories>$(ProjectDir);%(AdditionalIncludeDirectories)</AdditionalIncludeDirectories>
      <AdditionalOptions>/utf-8 %(AdditionalOptions)</AdditionalOptions>
    </ClCompile>
    <Link>
      <SubSystem>Windows</SubSystem>
      <EnableCOMDATFolding>true</EnableCOMDATFolding>
      <OptimizeReferences>true</OptimizeReferences>
      <GenerateDebugInformation>true</GenerateDebugInformation>
      <ModuleDefinitionFile>Source_krnln.def</ModuleDefinitionFile>
    </Link>
  </ItemDefinitionGroup>
  <ItemGroup>
    <ClCompile Include="krnln_cmdInfo.cpp" />
    <ClCompile Include="krnln_const.cpp" />
    <ClCompile Include="krnln_dllMain.cpp" />
    <ClCompile Include="krnln_dtType.cpp" />
    <ClCompile Include="cppCode\\krnln_cmd_impl.cpp" />
    <ClCompile Include="elib\\fnshare.cpp" />
  </ItemGroup>
  <ItemGroup>
    <ClInclude Include="include_krnln_header.h" />
    <ClInclude Include="krnln_cmd_typedef.h" />
  </ItemGroup>
  <ItemGroup>
    <None Include="Source_krnln.def" />
  </ItemGroup>
  <Import Project="$(VCTargetsPath)\\Microsoft.Cpp.targets" />
  <ImportGroup Label="ExtensionTargets">
  </ImportGroup>
</Project>'''
    
    def generate_sln(self) -> str:
        """生成Visual Studio解决方案文件"""
        return '''Microsoft Visual Studio Solution File, Format Version 12.00
# Visual Studio Version 17
VisualStudioVersion = 17.0.31903.59
MinimumVisualStudioVersion = 10.0.40219.1
Project("{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}") = "krnln", "krnln.vcxproj", "{D09F2340-8185-11D3-96F6-AE4C17150413}"
EndProject
Global
	GlobalSection(SolutionConfigurationPlatforms) = preSolution
		Debug|Win32 = Debug|Win32
		Debug|x64 = Debug|x64
		Release|Win32 = Release|Win32
		Release|x64 = Release|x64
	EndGlobalSection
	GlobalSection(ProjectConfigurationPlatforms) = postSolution
		{D09F2340-8185-11D3-96F6-AE4C17150413}.Debug|Win32.ActiveCfg = Debug|Win32
		{D09F2340-8185-11D3-96F6-AE4C17150413}.Debug|Win32.Build.0 = Debug|Win32
		{D09F2340-8185-11D3-96F6-AE4C17150413}.Debug|x64.ActiveCfg = Debug|x64
		{D09F2340-8185-11D3-96F6-AE4C17150413}.Debug|x64.Build.0 = Debug|x64
		{D09F2340-8185-11D3-96F6-AE4C17150413}.Release|Win32.ActiveCfg = Release|Win32
		{D09F2340-8185-11D3-96F6-AE4C17150413}.Release|Win32.Build.0 = Release|Win32
		{D09F2340-8185-11D3-96F6-AE4C17150413}.Release|x64.ActiveCfg = Release|x64
		{D09F2340-8185-11D3-96F6-AE4C17150413}.Release|x64.Build.0 = Release|x64
	EndGlobalSection
	GlobalSection(SolutionProperties) = preSolution
		HideSolutionNode = FALSE
	EndGlobalSection
EndGlobal
'''
    
    def generate_all(self):
        """生成所有文件"""
        files = {
            "include_krnln_header.h": self.generate_header(),
            "krnln_cmd_typedef.h": self.generate_cmd_typedef(),
            "krnln_cmdInfo.cpp": self.generate_cmdInfo(),
            "krnln_const.cpp": self.generate_const(),
            "krnln_dtType.cpp": self.generate_dtType(),
            "krnln_dllMain.cpp": self.generate_dllMain(),
            "cppCode/krnln_cmd_impl.cpp": self.generate_cmd_impl(),
            "Source_krnln.def": self.generate_def_file(),
            "krnln.vcxproj": self.generate_vcxproj(),
            "krnln.sln": self.generate_sln(),
        }
        
        for filename, content in files.items():
            filepath = os.path.join(self.output_dir, filename)
            # 确保目录存在
            os.makedirs(os.path.dirname(filepath) if os.path.dirname(filepath) else self.output_dir, exist_ok=True)
            
            with open(filepath, 'w', encoding='utf-8') as f:
                f.write(content)
            print(f"已生成: {filepath}")


def main():
    """主函数"""
    help_file = r"d:\chungbin\ycide\系统核心支持库帮助说明文件.txt"
    output_dir = r"d:\chungbin\ycide\功能库\krnln"
    
    if not os.path.exists(help_file):
        print(f"帮助文件不存在: {help_file}")
        return
    
    # 解析帮助文件
    parser = HelpFileParser(help_file)
    if not parser.parse_all():
        print("解析失败")
        return
    
    print(parser.get_summary())
    
    # 生成代码
    generator = CodeGenerator(parser, output_dir)
    generator.generate_all()
    
    print("\n代码生成完成!")
    print(f"输出目录: {output_dir}")


if __name__ == "__main__":
    main()
