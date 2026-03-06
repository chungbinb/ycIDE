"""
生成完整的 CJK 汉字拼音数据表 (U+4E00 ~ U+9FFF)
输出 PinyinData.h，包含：
1. 拼音音节表（约400个不重复音节）
2. 每个汉字对应的音节索引数组
"""
from pypinyin import pinyin, Style

# 收集所有汉字的拼音
CJK_START = 0x4E00
CJK_END = 0x9FFF
CJK_COUNT = CJK_END - CJK_START + 1

print(f"处理 {CJK_COUNT} 个汉字 (U+{CJK_START:04X} ~ U+{CJK_END:04X})...")

# 第一遍：收集所有不重复的拼音音节
syllable_set = set()
char_pinyin = {}

for code in range(CJK_START, CJK_END + 1):
    ch = chr(code)
    try:
        py_list = pinyin(ch, style=Style.NORMAL, heteronym=False)
        if py_list and py_list[0]:
            py = py_list[0][0].lower()
            char_pinyin[code] = py
            syllable_set.add(py)
    except:
        pass

# 给音节排序并编号
syllables = sorted(syllable_set)
syllable_to_index = {s: i + 1 for i, s in enumerate(syllables)}  # 0 表示无拼音

print(f"共 {len(syllables)} 个不重复拼音音节")

# 生成 C++ 头文件
with open("PinyinData.h", "w", encoding="utf-8") as f:
    f.write("// 自动生成的完整汉字拼音数据表，请勿手动编辑\n")
    f.write("// 覆盖 CJK 统一汉字基本区 U+4E00 ~ U+9FFF\n")
    f.write("#pragma once\n\n")
    f.write("#include <cstdint>\n\n")
    
    f.write(f"#define PINYIN_CJK_START 0x4E00\n")
    f.write(f"#define PINYIN_CJK_END   0x9FFF\n")
    f.write(f"#define PINYIN_CJK_COUNT {CJK_COUNT}\n")
    f.write(f"#define PINYIN_SYLLABLE_COUNT {len(syllables)}\n\n")
    
    # 写拼音音节表
    f.write("// 拼音音节表（索引从1开始，0表示无拼音）\n")
    f.write(f"static const char* const g_PinyinSyllables[{len(syllables) + 1}] = {{\n")
    f.write('    "",  // 0: 无拼音\n')
    for i, s in enumerate(syllables):
        comma = "," if i < len(syllables) - 1 else ""
        f.write(f'    "{s}"{comma}  // {i + 1}\n')
    f.write("};\n\n")
    
    # 写每个汉字的拼音索引（uint16_t数组）
    f.write("// 每个汉字对应的拼音音节索引\n")
    f.write(f"// 用法: g_PinyinSyllables[g_CharPinyinIndex[ch - PINYIN_CJK_START]]\n")
    f.write(f"static const uint16_t g_CharPinyinIndex[{CJK_COUNT}] = {{\n")
    
    line_items = []
    for code in range(CJK_START, CJK_END + 1):
        idx = syllable_to_index.get(char_pinyin.get(code, ""), 0)
        line_items.append(str(idx))
        
        if len(line_items) == 32 or code == CJK_END:
            comma = "," if code < CJK_END else ""
            f.write("    " + ",".join(line_items) + comma + "\n")
            line_items = []
    
    f.write("};\n")

print("已生成 PinyinData.h")
