#ifndef KRNLN_PROPERTIES_H
#define KRNLN_PROPERTIES_H

#include "elib/lib2.h"

// 窗口组件属性定义
// 参考 lib2.h 中的 FIXED_WIN_UNIT_PROPERTY 定义

// 窗口组件固定属性（所有组件共有）
#define KRNLN_FIXED_PROPERTY \
    {"左边", "left", NULL, UD_INT, _PROP_OS(OS_ALL), NULL}, \
    {"顶边", "top", NULL, UD_INT, _PROP_OS(OS_ALL), NULL}, \
    {"宽度", "width", NULL, UD_INT, _PROP_OS(OS_ALL), NULL}, \
    {"高度", "height", NULL, UD_INT, _PROP_OS(OS_ALL), NULL}, \
    {"标记", "tag", NULL, UD_TEXT, _PROP_OS(OS_ALL), NULL}, \
    {"可视", "visible", NULL, UD_BOOL, _PROP_OS(OS_ALL), NULL}, \
    {"禁止", "disable", NULL, UD_BOOL, _PROP_OS(OS_ALL), NULL}, \
    {"鼠标指针", "MousePointer", NULL, UD_CURSOR, _PROP_OS(OS_ALL), NULL}

#define KRNLN_FIXED_PROPERTY_COUNT 8

// ==================== 窗口 属性 ====================
static UNIT_PROPERTY s_prop_Window[] = {
    KRNLN_FIXED_PROPERTY,
    {"标题", "caption", "窗口标题栏文字", UD_TEXT, _PROP_OS(OS_ALL), NULL},
    {"背景颜色", "backcolor", "窗口背景颜色", UD_COLOR_BACK, _PROP_OS(OS_ALL), NULL},
    {"边框", "border", "窗口边框样式", UD_PICK_INT, _PROP_OS(OS_ALL), "无边框\0" "单线边框\0" "可调边框\0" "对话框边框\0" "工具窗口边框\0" "可调工具窗口边框\0" "\0"},
    {"最大化按钮", "maxButton", "是否显示最大化按钮", UD_BOOL, _PROP_OS(OS_ALL), NULL},
    {"最小化按钮", "minButton", "是否显示最小化按钮", UD_BOOL, _PROP_OS(OS_ALL), NULL},
    {"控制按钮", "controlBox", "是否显示控制按钮", UD_BOOL, _PROP_OS(OS_ALL), NULL},
    {"位置", "startPosition", "窗口初始位置", UD_PICK_INT, _PROP_OS(OS_ALL), "通常\0" "居中\0" "\0"},
    {"图标", "icon", "窗口图标", UD_ICON, _PROP_OS(OS_ALL), NULL},
    {"背景图片", "bgImage", "窗口背景图片", UD_PIC, _PROP_OS(OS_ALL), NULL},
    {"进入许可", "enterKey", "是否允许按Enter键进入", UD_BOOL, _PROP_OS(OS_ALL), NULL},
    {"Esc关闭", "escClose", "是否允许按Esc键关闭", UD_BOOL, _PROP_OS(OS_ALL), NULL},
    {"总在最前", "topMost", "是否总在最前", UD_BOOL, _PROP_OS(OS_ALL), NULL},
    {"随意移动", "movable", "是否可以在客户区随意移动", UD_BOOL, _PROP_OS(OS_ALL), NULL},
    {"播放音乐", "playMusic", "窗口打开时播放的音乐", UD_MUSIC, _PROP_OS(OS_ALL), NULL},
    {"帮助按钮", "helpButton", "是否显示帮助按钮", UD_BOOL, _PROP_OS(OS_ALL), NULL},
    {"底图方式", "bgMode", "底图显示方式", UD_PICK_INT, _PROP_OS(OS_ALL), "居中\0" "平铺\0" "缩放\0" "\0"},
};
static const int s_prop_Window_count = sizeof(s_prop_Window) / sizeof(s_prop_Window[0]);

// ==================== 编辑框 属性 ====================
static UNIT_PROPERTY s_prop_EditBox[] = {
    KRNLN_FIXED_PROPERTY,
    {"内容", "text", "编辑框内容", UD_TEXT, _PROP_OS(OS_ALL), NULL},
    {"字体", "font", "文字字体", UD_FONT, _PROP_OS(OS_ALL), NULL},
    {"边框", "border", "边框样式", UD_PICK_INT, _PROP_OS(OS_ALL), "无边框\0" "凹入式\0" "凸出式\0" "浅凹入式\0" "镜框式\0" "单线边框式\0" "\0"},
    {"隐藏选择", "hideSelection", "失去焦点时是否隐藏选择", UD_BOOL, _PROP_OS(OS_ALL), NULL},
    {"最大允许长度", "maxLength", "允许输入的最大字符数", UD_INT, _PROP_OS(OS_ALL), NULL},
    {"是否允许多行", "multiLine", "是否允许输入多行文本", UD_BOOL, _PROP_OS(OS_ALL), NULL},
    {"滚动条", "scrollBars", "滚动条显示方式", UD_PICK_INT, _PROP_OS(OS_ALL), "无\0" "横向\0" "纵向\0" "横向及纵向\0" "\0"},
    {"对齐方式", "alignment", "文字对齐方式", UD_PICK_INT, _PROP_OS(OS_ALL), "左对齐\0" "居中\0" "右对齐\0" "\0"},
    {"输入方式", "inputMode", "输入方式", UD_PICK_INT, _PROP_OS(OS_ALL), "通常\0" "只读\0" "密码\0" "整数文本\0" "小数文本\0" "\0"},
    {"密码遮盖字符", "passwordChar", "密码模式下的遮盖字符", UD_TEXT, _PROP_OS(OS_ALL), NULL},
    {"起始选择位置", "selStart", "选择的起始位置", UD_INT, _PROP_OS(OS_ALL) | UW_ONLY_READ, NULL},
    {"被选择字符数", "selLength", "被选择的字符数", UD_INT, _PROP_OS(OS_ALL) | UW_ONLY_READ, NULL},
    {"被选择文本", "selText", "被选择的文本", UD_TEXT, _PROP_OS(OS_ALL) | UW_ONLY_READ, NULL},
    {"文本颜色", "foreColor", "文字颜色", UD_COLOR, _PROP_OS(OS_ALL), NULL},
    {"背景颜色", "backColor", "背景颜色", UD_COLOR_BACK, _PROP_OS(OS_ALL), NULL},
    {"调节器方式", "spinStyle", "调节器显示方式", UD_PICK_INT, _PROP_OS(OS_ALL), "无调节器\0" "左侧调节\0" "右侧调节\0" "\0"},
};
static const int s_prop_EditBox_count = sizeof(s_prop_EditBox) / sizeof(s_prop_EditBox[0]);

// ==================== 标签 属性 ====================
static UNIT_PROPERTY s_prop_Label[] = {
    KRNLN_FIXED_PROPERTY,
    {"标题", "caption", "显示文本", UD_TEXT, _PROP_OS(OS_ALL), NULL},
    {"字体", "font", "文字字体", UD_FONT, _PROP_OS(OS_ALL), NULL},
    {"边框", "border", "边框样式", UD_PICK_INT, _PROP_OS(OS_ALL), "无边框\0" "凹入式\0" "凸出式\0" "浅凹入式\0" "镜框式\0" "单线边框式\0" "\0"},
    {"对齐方式", "alignment", "文字对齐方式", UD_PICK_INT, _PROP_OS(OS_ALL), "左对齐\0" "居中\0" "右对齐\0" "\0"},
    {"自动折行", "wordWrap", "文字是否自动折行", UD_BOOL, _PROP_OS(OS_ALL), NULL},
    {"文本颜色", "foreColor", "文字颜色", UD_COLOR, _PROP_OS(OS_ALL), NULL},
    {"背景颜色", "backColor", "背景颜色", UD_COLOR_BACK, _PROP_OS(OS_ALL), NULL},
    {"透明", "transparent", "背景是否透明", UD_BOOL, _PROP_OS(OS_ALL), NULL},
};
static const int s_prop_Label_count = sizeof(s_prop_Label) / sizeof(s_prop_Label[0]);

// ==================== 按钮 属性 ====================
static UNIT_PROPERTY s_prop_Button[] = {
    KRNLN_FIXED_PROPERTY,
    {"标题", "caption", "按钮上显示的文字", UD_TEXT, _PROP_OS(OS_ALL), NULL},
    {"字体", "font", "文字字体", UD_FONT, _PROP_OS(OS_ALL), NULL},
    {"图片", "picture", "按钮上显示的图片", UD_PIC, _PROP_OS(OS_ALL), NULL},
    {"选中图片", "pictureDown", "按钮按下时显示的图片", UD_PIC, _PROP_OS(OS_ALL), NULL},
    {"底图方式", "pictureMode", "图片显示方式", UD_PICK_INT, _PROP_OS(OS_ALL), "图片在上\0" "图片在下\0" "图片在左\0" "图片在右\0" "图片居中\0" "\0"},
    {"默认", "default", "是否为默认按钮", UD_BOOL, _PROP_OS(OS_ALL), NULL},
    {"取消", "cancel", "是否为取消按钮", UD_BOOL, _PROP_OS(OS_ALL), NULL},
    {"平面", "flat", "是否为平面样式", UD_BOOL, _PROP_OS(OS_ALL), NULL},
    {"文本颜色", "foreColor", "文字颜色", UD_COLOR, _PROP_OS(OS_ALL), NULL},
};
static const int s_prop_Button_count = sizeof(s_prop_Button) / sizeof(s_prop_Button[0]);

// ==================== 选择框 属性 ====================
static UNIT_PROPERTY s_prop_CheckBox[] = {
    KRNLN_FIXED_PROPERTY,
    {"标题", "caption", "选择框文字", UD_TEXT, _PROP_OS(OS_ALL), NULL},
    {"字体", "font", "文字字体", UD_FONT, _PROP_OS(OS_ALL), NULL},
    {"选中", "checked", "是否选中", UD_BOOL, _PROP_OS(OS_ALL), NULL},
    {"对齐方式", "alignment", "文字对齐方式", UD_PICK_INT, _PROP_OS(OS_ALL), "左对齐\0" "右对齐\0" "\0"},
    {"文本颜色", "foreColor", "文字颜色", UD_COLOR, _PROP_OS(OS_ALL), NULL},
    {"背景颜色", "backColor", "背景颜色", UD_COLOR_BACK, _PROP_OS(OS_ALL), NULL},
    {"三态", "triState", "是否支持三态", UD_BOOL, _PROP_OS(OS_ALL), NULL},
};
static const int s_prop_CheckBox_count = sizeof(s_prop_CheckBox) / sizeof(s_prop_CheckBox[0]);

// ==================== 单选框 属性 ====================
static UNIT_PROPERTY s_prop_RadioBox[] = {
    KRNLN_FIXED_PROPERTY,
    {"标题", "caption", "单选框文字", UD_TEXT, _PROP_OS(OS_ALL), NULL},
    {"字体", "font", "文字字体", UD_FONT, _PROP_OS(OS_ALL), NULL},
    {"选中", "checked", "是否选中", UD_BOOL, _PROP_OS(OS_ALL), NULL},
    {"对齐方式", "alignment", "文字对齐方式", UD_PICK_INT, _PROP_OS(OS_ALL), "左对齐\0" "右对齐\0" "\0"},
    {"文本颜色", "foreColor", "文字颜色", UD_COLOR, _PROP_OS(OS_ALL), NULL},
    {"背景颜色", "backColor", "背景颜色", UD_COLOR_BACK, _PROP_OS(OS_ALL), NULL},
};
static const int s_prop_RadioBox_count = sizeof(s_prop_RadioBox) / sizeof(s_prop_RadioBox[0]);

// ==================== 图片框 属性 ====================
static UNIT_PROPERTY s_prop_PicBox[] = {
    KRNLN_FIXED_PROPERTY,
    {"图片", "picture", "显示的图片", UD_PIC, _PROP_OS(OS_ALL), NULL},
    {"边框", "border", "边框样式", UD_PICK_INT, _PROP_OS(OS_ALL), "无边框\0" "凹入式\0" "凸出式\0" "浅凹入式\0" "镜框式\0" "单线边框式\0" "\0"},
    {"显示方式", "displayMode", "图片显示方式", UD_PICK_INT, _PROP_OS(OS_ALL), "图片居左上\0" "缩放图片\0" "调整组件\0" "图片居中\0" "平铺图片\0" "\0"},
    {"背景颜色", "backColor", "背景颜色", UD_COLOR_BACK, _PROP_OS(OS_ALL), NULL},
};
static const int s_prop_PicBox_count = sizeof(s_prop_PicBox) / sizeof(s_prop_PicBox[0]);

// ==================== 外形框 属性 ====================
static UNIT_PROPERTY s_prop_ShapeBox[] = {
    KRNLN_FIXED_PROPERTY,
    {"形状", "shape", "外形形状", UD_PICK_INT, _PROP_OS(OS_ALL), "矩形\0" "正方形\0" "椭圆形\0" "圆形\0" "圆角矩形\0" "圆角正方形\0" "\0"},
    {"边框样式", "borderStyle", "边框线条样式", UD_PICK_INT, _PROP_OS(OS_ALL), "透明\0" "实线\0" "点线\0" "短划线\0" "点划线\0" "点点划线\0" "内收实线\0" "\0"},
    {"填充样式", "fillStyle", "内部填充样式", UD_PICK_INT, _PROP_OS(OS_ALL), "透明\0" "实心\0" "横线\0" "竖线\0" "下斜线\0" "上斜线\0" "十字线\0" "对角线\0" "\0"},
    {"边框宽度", "borderWidth", "边框线条宽度", UD_INT, _PROP_OS(OS_ALL), NULL},
    {"边框颜色", "borderColor", "边框颜色", UD_COLOR, _PROP_OS(OS_ALL), NULL},
    {"填充颜色", "fillColor", "填充颜色", UD_COLOR, _PROP_OS(OS_ALL), NULL},
    {"背景颜色", "backColor", "背景颜色", UD_COLOR_BACK, _PROP_OS(OS_ALL), NULL},
};
static const int s_prop_ShapeBox_count = sizeof(s_prop_ShapeBox) / sizeof(s_prop_ShapeBox[0]);

// ==================== 画板 属性 ====================
static UNIT_PROPERTY s_prop_DrawPanel[] = {
    KRNLN_FIXED_PROPERTY,
    {"边框", "border", "边框样式", UD_PICK_INT, _PROP_OS(OS_ALL), "无边框\0" "凹入式\0" "凸出式\0" "浅凹入式\0" "镜框式\0" "单线边框式\0" "\0"},
    {"背景颜色", "backColor", "背景颜色", UD_COLOR_BACK, _PROP_OS(OS_ALL), NULL},
    {"填充颜色", "fillColor", "绘图填充颜色", UD_COLOR, _PROP_OS(OS_ALL), NULL},
    {"线条颜色", "lineColor", "绘图线条颜色", UD_COLOR, _PROP_OS(OS_ALL), NULL},
    {"字体", "font", "绘图字体", UD_FONT, _PROP_OS(OS_ALL), NULL},
};
static const int s_prop_DrawPanel_count = sizeof(s_prop_DrawPanel) / sizeof(s_prop_DrawPanel[0]);

// ==================== 分组框 属性 ====================
static UNIT_PROPERTY s_prop_GroupBox[] = {
    KRNLN_FIXED_PROPERTY,
    {"标题", "caption", "分组框标题", UD_TEXT, _PROP_OS(OS_ALL), NULL},
    {"字体", "font", "标题字体", UD_FONT, _PROP_OS(OS_ALL), NULL},
    {"文本颜色", "foreColor", "标题颜色", UD_COLOR, _PROP_OS(OS_ALL), NULL},
    {"背景颜色", "backColor", "背景颜色", UD_COLOR_BACK, _PROP_OS(OS_ALL), NULL},
};
static const int s_prop_GroupBox_count = sizeof(s_prop_GroupBox) / sizeof(s_prop_GroupBox[0]);

// ==================== 组合框 属性 ====================
static UNIT_PROPERTY s_prop_ComboBox[] = {
    KRNLN_FIXED_PROPERTY,
    {"内容", "text", "组合框内容", UD_TEXT, _PROP_OS(OS_ALL), NULL},
    {"字体", "font", "文字字体", UD_FONT, _PROP_OS(OS_ALL), NULL},
    {"列表项目", "list", "下拉列表项目", UD_TEXT, _PROP_OS(OS_ALL), NULL},
    {"类型", "style", "组合框类型", UD_PICK_INT, _PROP_OS(OS_ALL), "可编辑\0" "下拉式\0" "列表式\0" "\0"},
    {"现行选中项", "listIndex", "当前选中项索引", UD_INT, _PROP_OS(OS_ALL) | UW_ONLY_READ, NULL},
    {"项目数", "listCount", "列表项目数", UD_INT, _PROP_OS(OS_ALL) | UW_ONLY_READ, NULL},
    {"最大允许长度", "maxLength", "允许输入的最大字符数", UD_INT, _PROP_OS(OS_ALL), NULL},
    {"文本颜色", "foreColor", "文字颜色", UD_COLOR, _PROP_OS(OS_ALL), NULL},
    {"背景颜色", "backColor", "背景颜色", UD_COLOR_BACK, _PROP_OS(OS_ALL), NULL},
};
static const int s_prop_ComboBox_count = sizeof(s_prop_ComboBox) / sizeof(s_prop_ComboBox[0]);

// ==================== 列表框 属性 ====================
static UNIT_PROPERTY s_prop_ListBox[] = {
    KRNLN_FIXED_PROPERTY,
    {"字体", "font", "文字字体", UD_FONT, _PROP_OS(OS_ALL), NULL},
    {"列表项目", "list", "列表项目", UD_TEXT, _PROP_OS(OS_ALL), NULL},
    {"边框", "border", "边框样式", UD_PICK_INT, _PROP_OS(OS_ALL), "无边框\0" "凹入式\0" "凸出式\0" "浅凹入式\0" "镜框式\0" "单线边框式\0" "\0"},
    {"选择方式", "selectionMode", "选择方式", UD_PICK_INT, _PROP_OS(OS_ALL), "单选\0" "复选\0" "\0"},
    {"排序", "sorted", "是否按字母排序", UD_BOOL, _PROP_OS(OS_ALL), NULL},
    {"现行选中项", "listIndex", "当前选中项索引", UD_INT, _PROP_OS(OS_ALL) | UW_ONLY_READ, NULL},
    {"项目数", "listCount", "列表项目数", UD_INT, _PROP_OS(OS_ALL) | UW_ONLY_READ, NULL},
    {"文本颜色", "foreColor", "文字颜色", UD_COLOR, _PROP_OS(OS_ALL), NULL},
    {"背景颜色", "backColor", "背景颜色", UD_COLOR_BACK, _PROP_OS(OS_ALL), NULL},
    {"允许拖放", "allowDrag", "是否允许拖放项目", UD_BOOL, _PROP_OS(OS_ALL), NULL},
};
static const int s_prop_ListBox_count = sizeof(s_prop_ListBox) / sizeof(s_prop_ListBox[0]);

// ==================== 选择列表框 属性 ====================
static UNIT_PROPERTY s_prop_ChkListBox[] = {
    KRNLN_FIXED_PROPERTY,
    {"字体", "font", "文字字体", UD_FONT, _PROP_OS(OS_ALL), NULL},
    {"列表项目", "list", "列表项目", UD_TEXT, _PROP_OS(OS_ALL), NULL},
    {"边框", "border", "边框样式", UD_PICK_INT, _PROP_OS(OS_ALL), "无边框\0" "凹入式\0" "凸出式\0" "浅凹入式\0" "镜框式\0" "单线边框式\0" "\0"},
    {"排序", "sorted", "是否按字母排序", UD_BOOL, _PROP_OS(OS_ALL), NULL},
    {"现行选中项", "listIndex", "当前选中项索引", UD_INT, _PROP_OS(OS_ALL) | UW_ONLY_READ, NULL},
    {"项目数", "listCount", "列表项目数", UD_INT, _PROP_OS(OS_ALL) | UW_ONLY_READ, NULL},
    {"文本颜色", "foreColor", "文字颜色", UD_COLOR, _PROP_OS(OS_ALL), NULL},
    {"背景颜色", "backColor", "背景颜色", UD_COLOR_BACK, _PROP_OS(OS_ALL), NULL},
};
static const int s_prop_ChkListBox_count = sizeof(s_prop_ChkListBox) / sizeof(s_prop_ChkListBox[0]);

// ==================== 横向滚动条 属性 ====================
static UNIT_PROPERTY s_prop_HScrollBar[] = {
    KRNLN_FIXED_PROPERTY,
    {"位置", "value", "当前位置", UD_INT, _PROP_OS(OS_ALL), NULL},
    {"最小位置", "min", "最小位置值", UD_INT, _PROP_OS(OS_ALL), NULL},
    {"最大位置", "max", "最大位置值", UD_INT, _PROP_OS(OS_ALL), NULL},
    {"行改变量", "smallChange", "点击按钮时的改变量", UD_INT, _PROP_OS(OS_ALL), NULL},
    {"页改变量", "largeChange", "点击空白处时的改变量", UD_INT, _PROP_OS(OS_ALL), NULL},
};
static const int s_prop_HScrollBar_count = sizeof(s_prop_HScrollBar) / sizeof(s_prop_HScrollBar[0]);

// ==================== 纵向滚动条 属性 ====================
static UNIT_PROPERTY s_prop_VScrollBar[] = {
    KRNLN_FIXED_PROPERTY,
    {"位置", "value", "当前位置", UD_INT, _PROP_OS(OS_ALL), NULL},
    {"最小位置", "min", "最小位置值", UD_INT, _PROP_OS(OS_ALL), NULL},
    {"最大位置", "max", "最大位置值", UD_INT, _PROP_OS(OS_ALL), NULL},
    {"行改变量", "smallChange", "点击按钮时的改变量", UD_INT, _PROP_OS(OS_ALL), NULL},
    {"页改变量", "largeChange", "点击空白处时的改变量", UD_INT, _PROP_OS(OS_ALL), NULL},
};
static const int s_prop_VScrollBar_count = sizeof(s_prop_VScrollBar) / sizeof(s_prop_VScrollBar[0]);

// ==================== 进度条 属性 ====================
static UNIT_PROPERTY s_prop_ProcessBar[] = {
    KRNLN_FIXED_PROPERTY,
    {"位置", "value", "当前位置", UD_INT, _PROP_OS(OS_ALL), NULL},
    {"最小位置", "min", "最小位置值", UD_INT, _PROP_OS(OS_ALL), NULL},
    {"最大位置", "max", "最大位置值", UD_INT, _PROP_OS(OS_ALL), NULL},
    {"方向", "orientation", "进度条方向", UD_PICK_INT, _PROP_OS(OS_ALL), "横向\0" "纵向\0" "\0"},
    {"平滑", "smooth", "是否平滑显示", UD_BOOL, _PROP_OS(OS_ALL), NULL},
};
static const int s_prop_ProcessBar_count = sizeof(s_prop_ProcessBar) / sizeof(s_prop_ProcessBar[0]);

// ==================== 滑块条 属性 ====================
static UNIT_PROPERTY s_prop_SliderBar[] = {
    KRNLN_FIXED_PROPERTY,
    {"位置", "value", "当前位置", UD_INT, _PROP_OS(OS_ALL), NULL},
    {"最小位置", "min", "最小位置值", UD_INT, _PROP_OS(OS_ALL), NULL},
    {"最大位置", "max", "最大位置值", UD_INT, _PROP_OS(OS_ALL), NULL},
    {"方向", "orientation", "滑块条方向", UD_PICK_INT, _PROP_OS(OS_ALL), "横向\0" "纵向\0" "\0"},
    {"刻度频率", "tickFrequency", "刻度线频率", UD_INT, _PROP_OS(OS_ALL), NULL},
    {"刻度位置", "tickStyle", "刻度线位置", UD_PICK_INT, _PROP_OS(OS_ALL), "两边\0" "上/左\0" "下/右\0" "无\0" "\0"},
};
static const int s_prop_SliderBar_count = sizeof(s_prop_SliderBar) / sizeof(s_prop_SliderBar[0]);

// ==================== 选择夹 属性 ====================
static UNIT_PROPERTY s_prop_Tab[] = {
    KRNLN_FIXED_PROPERTY,
    {"字体", "font", "标签字体", UD_FONT, _PROP_OS(OS_ALL), NULL},
    {"子夹", "tabs", "选择夹页面", UD_TEXT, _PROP_OS(OS_ALL), NULL},
    {"现行子夹", "tabIndex", "当前选中的子夹索引", UD_INT, _PROP_OS(OS_ALL), NULL},
    {"位置", "tabPosition", "标签位置", UD_PICK_INT, _PROP_OS(OS_ALL), "顶部\0" "底部\0" "左侧\0" "右侧\0" "\0"},
    {"多行", "multiLine", "是否显示多行标签", UD_BOOL, _PROP_OS(OS_ALL), NULL},
    {"按钮样式", "buttonStyle", "是否使用按钮样式", UD_BOOL, _PROP_OS(OS_ALL), NULL},
};
static const int s_prop_Tab_count = sizeof(s_prop_Tab) / sizeof(s_prop_Tab[0]);

// ==================== 时钟 属性 ====================
static UNIT_PROPERTY s_prop_Timer[] = {
    KRNLN_FIXED_PROPERTY,
    {"时钟周期", "interval", "时钟事件产生周期（毫秒）", UD_INT, _PROP_OS(OS_ALL), NULL},
};
static const int s_prop_Timer_count = sizeof(s_prop_Timer) / sizeof(s_prop_Timer[0]);

// ==================== 表格 属性 ====================
static UNIT_PROPERTY s_prop_Grid[] = {
    KRNLN_FIXED_PROPERTY,
    {"字体", "font", "文字字体", UD_FONT, _PROP_OS(OS_ALL), NULL},
    {"边框", "border", "边框样式", UD_PICK_INT, _PROP_OS(OS_ALL), "无边框\0" "凹入式\0" "凸出式\0" "浅凹入式\0" "镜框式\0" "单线边框式\0" "\0"},
    {"标题行数", "headerRows", "标题行数", UD_INT, _PROP_OS(OS_ALL), NULL},
    {"标题列数", "headerCols", "标题列数", UD_INT, _PROP_OS(OS_ALL), NULL},
    {"行数", "rows", "总行数", UD_INT, _PROP_OS(OS_ALL), NULL},
    {"列数", "cols", "总列数", UD_INT, _PROP_OS(OS_ALL), NULL},
    {"默认行高", "rowHeight", "默认行高", UD_INT, _PROP_OS(OS_ALL), NULL},
    {"默认列宽", "colWidth", "默认列宽", UD_INT, _PROP_OS(OS_ALL), NULL},
    {"选择方式", "selectionMode", "选择方式", UD_PICK_INT, _PROP_OS(OS_ALL), "单选\0" "复选\0" "\0"},
    {"允许编辑", "editable", "是否允许编辑单元格", UD_BOOL, _PROP_OS(OS_ALL), NULL},
    {"显示网格", "gridLines", "是否显示网格线", UD_BOOL, _PROP_OS(OS_ALL), NULL},
};
static const int s_prop_Grid_count = sizeof(s_prop_Grid) / sizeof(s_prop_Grid[0]);

#endif // KRNLN_PROPERTIES_H
