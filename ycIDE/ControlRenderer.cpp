#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
#include "ControlRenderer.h"
#include "VisualDesigner.h"
#include "FneParser.h"
#include <fstream>
#include <sstream>
#include <regex>
#include <locale>
#include <codecvt>
#include <set>

ControlRenderer::ControlRenderer()
    : m_fneParser(nullptr)
{
    // 尝试加载krnln.fne
    if (!LoadControlsFromKrnln()) {
        // 如果加载失败，使用内置控件定义
        RegisterBuiltinControls();
    }
}

ControlRenderer::~ControlRenderer()
{
    if (m_fneParser) {
        delete m_fneParser;
    }
}

// 从krnln.fne加载窗口组件
bool ControlRenderer::LoadControlsFromKrnln()
{
    // 尝试多个可能的路径
    std::vector<std::wstring> paths = {
        L"krnln.fne",
        L"..\\krnln.fne",
        L"功能库\\krnln\\krnln.fne"
    };
    
    for (const auto& path : paths) {
        if (LoadControlsFromFne(path)) {
            return true;
        }
    }
    
    // 如果FNE文件加载失败，尝试从帮助文档解析
    std::vector<std::wstring> helpPaths = {
        L"系统核心支持库帮助说明文件.txt",
        L"..\\系统核心支持库帮助说明文件.txt"
    };
    
    for (const auto& path : helpPaths) {
        if (ParseControlsFromHelpFile(path)) {
            return true;
        }
    }
    
    return false;
}

bool ControlRenderer::LoadControlsFromFne(const std::wstring& fnePath)
{
    // TODO: 使用FneParser加载控件定义
    // 当前FneParser主要解析命令，需要扩展以支持数据类型（窗口组件）解析
    
    m_fneParser = new FneParser();
    if (!m_fneParser->LoadFneFile(fnePath)) {
        delete m_fneParser;
        m_fneParser = nullptr;
        return false;
    }
    
    // 暂时返回false，让它使用ParseControlsFromHelpFile
    return false;
}

// 从帮助文档解析控件信息
bool ControlRenderer::ParseControlsFromHelpFile(const std::wstring& helpFilePath)
{
    std::wifstream file(helpFilePath);
    if (!file.is_open()) {
        return false;
    }
    
    file.imbue(std::locale(std::locale(), new std::codecvt_utf8<wchar_t>));
    
    std::wstring line;
    ControlMetadata* currentControl = nullptr;
    ControlPropertyInfo* currentProperty = nullptr;
    
    while (std::getline(file, line)) {
        // 检测数据类型定义行：数据类型名称：按钮    英文名称：Button
        if (line.find(L"数据类型名称：") != std::wstring::npos) {
            size_t namePos = line.find(L"数据类型名称：");
            size_t engPos = line.find(L"英文名称：");
            
            if (namePos != std::wstring::npos && engPos != std::wstring::npos) {
                std::wstring typeName = line.substr(namePos + 7, engPos - namePos - 11);
                std::wstring engName = line.substr(engPos + 5);
                
                // 去除空白
                typeName.erase(0, typeName.find_first_not_of(L" \t"));
                typeName.erase(typeName.find_last_not_of(L" \t") + 1);
                engName.erase(0, engName.find_first_not_of(L" \t"));
                engName.erase(engName.find_last_not_of(L" \t") + 1);
                
                // 过滤非窗口组件（窗口、菜单、字体等）
                if (typeName != L"窗口" && typeName != L"菜单" && typeName != L"字体") {
                    ControlMetadata metadata;
                    metadata.type = typeName;
                    metadata.englishName = engName;
                    metadata.displayName = typeName;
                    metadata.category = L"基本组件";
                    
                    m_controlMetadata[typeName] = metadata;
                    currentControl = &m_controlMetadata[typeName];
                }
            }
        }
        // 检测容器组件型
        else if (currentControl && line.find(L"本数据类型为容器窗口组件型") != std::wstring::npos) {
            currentControl->isContainer = true;
        }
        // 检测是否可接受输入焦点
        else if (currentControl && line.find(L"不能接受输入焦点") != std::wstring::npos) {
            currentControl->canAcceptFocus = false;
        }
        // 检测成员属性
        else if (currentControl && line.find(L"成员属性<") != std::wstring::npos && line.find(L"的中文名称为") != std::wstring::npos) {
            size_t nameStartPos = line.find(L"的中文名称为\"");
            size_t nameEndPos = line.find(L"\"，英文名称为\"");
            size_t engStartPos = line.find(L"\"，英文名称为\"");
            size_t engEndPos = line.find(L"\"，类型为\"");
            size_t typeStartPos = line.find(L"\"，类型为\"");
            size_t typeEndPos = line.find(L"\"。");
            
            if (nameStartPos != std::wstring::npos && engStartPos != std::wstring::npos) {
                ControlPropertyInfo propInfo;
                propInfo.name = line.substr(nameStartPos + 7, nameEndPos - nameStartPos - 7);
                propInfo.englishName = line.substr(engStartPos + 8, engEndPos - engStartPos - 8);
                
                if (typeStartPos != std::wstring::npos && typeEndPos != std::wstring::npos) {
                    propInfo.dataType = line.substr(typeStartPos + 6, typeEndPos - typeStartPos - 6);
                }
                
                // 根据属性名称和数据类型确定编辑器类型
                if (propInfo.dataType.find(L"逻辑型") != std::wstring::npos) {
                    propInfo.editorType = PropertyEditorType::Boolean;
                    propInfo.defaultValue = L"假";
                } else if (propInfo.dataType.find(L"整数型") != std::wstring::npos) {
                    if (propInfo.name.find(L"颜色") != std::wstring::npos) {
                        propInfo.editorType = PropertyEditorType::Color;
                        propInfo.defaultValue = L"16777215";  // 白色
                    } else {
                        propInfo.editorType = PropertyEditorType::Number;
                        propInfo.defaultValue = L"0";
                    }
                } else {
                    propInfo.editorType = PropertyEditorType::Text;
                    propInfo.defaultValue = L"";
                }
                
                currentControl->properties.push_back(propInfo);
                currentProperty = &currentControl->properties.back();
            }
        }
        // 检测枚举值
        else if (currentProperty && line.find(L"可供选择的属性值：") != std::wstring::npos) {
            currentProperty->editorType = PropertyEditorType::Enum;
            currentProperty->enumValues.clear();
        }
        else if (currentProperty && currentProperty->editorType == PropertyEditorType::Enum) {
            // 检测枚举项：        0. 无边框
            size_t dotPos = line.find(L". ");
            if (dotPos != std::wstring::npos && dotPos < 20) {
                std::wstring enumValue = line.substr(dotPos + 2);
                enumValue.erase(0, enumValue.find_first_not_of(L" \t"));
                enumValue.erase(enumValue.find_last_not_of(L" \t") + 1);
                if (!enumValue.empty()) {
                    currentProperty->enumValues.push_back(enumValue);
                    if (currentProperty->enumValues.size() == 1) {
                        currentProperty->defaultValue = enumValue;
                    }
                }
            } else if (line.find(L"成员属性") != std::wstring::npos) {
                // 枚举结束
                currentProperty = nullptr;
            }
        }
    }
    
    file.close();
    
    // 设置默认尺寸
    for (auto& pair : m_controlMetadata) {
        auto& meta = pair.second;
        if (meta.type == L"按钮") {
            meta.defaultWidth = 75;
            meta.defaultHeight = 25;
        } else if (meta.type == L"标签") {
            meta.defaultWidth = 60;
            meta.defaultHeight = 20;
        } else if (meta.type == L"编辑框") {
            meta.defaultWidth = 120;
            meta.defaultHeight = 20;
        } else if (meta.type == L"选择框" || meta.type == L"单选框") {
            meta.defaultWidth = 80;
            meta.defaultHeight = 20;
        } else if (meta.type == L"图片框") {
            meta.defaultWidth = 100;
            meta.defaultHeight = 100;
        } else if (meta.type == L"分组框") {
            meta.defaultWidth = 150;
            meta.defaultHeight = 120;
        } else if (meta.type == L"列表框" || meta.type == L"组合框") {
            meta.defaultWidth = 120;
            meta.defaultHeight = 80;
        } else {
            meta.defaultWidth = 80;
            meta.defaultHeight = 24;
        }
        
        // 设置分类
        if (meta.type == L"按钮" || meta.type == L"标签" || meta.type == L"编辑框" || 
            meta.type == L"选择框" || meta.type == L"单选框") {
            meta.category = L"基本组件";
        } else if (meta.type == L"组合框" || meta.type == L"列表框" || meta.type == L"选择列表框") {
            meta.category = L"列表类组件";
        } else if (meta.type == L"图片框" || meta.type == L"外形框" || meta.type == L"画板") {
            meta.category = L"图形组件";
        } else if (meta.type == L"分组框") {
            meta.category = L"容器组件";
        } else if (meta.type == L"进度条" || meta.type == L"滑块条") {
            meta.category = L"指示器组件";
        } else {
            meta.category = L"其他组件";
        }
    }
    
    return !m_controlMetadata.empty();
}

// 注册内置控件（备用方案）
void ControlRenderer::RegisterBuiltinControls()
{
    // 按钮
    ControlMetadata button;
    button.type = L"按钮";
    button.englishName = L"Button";
    button.displayName = L"按钮";
    button.category = L"基本组件";
    button.defaultWidth = 75;
    button.defaultHeight = 25;
    button.canAcceptFocus = true;
    RegisterControl(button);
    
    // 标签
    ControlMetadata label;
    label.type = L"标签";
    label.englishName = L"Label";
    label.displayName = L"标签";
    label.category = L"基本组件";
    label.defaultWidth = 60;
    label.defaultHeight = 20;
    label.canAcceptFocus = false;
    RegisterControl(label);
    
    // 编辑框
    ControlMetadata editBox;
    editBox.type = L"编辑框";
    editBox.englishName = L"EditBox";
    editBox.displayName = L"编辑框";
    editBox.category = L"基本组件";
    editBox.defaultWidth = 120;
    editBox.defaultHeight = 20;
    editBox.canAcceptFocus = true;
    RegisterControl(editBox);
    
    // 可以继续添加更多内置控件...
}

void ControlRenderer::RegisterControl(const ControlMetadata& metadata)
{
    m_controlMetadata[metadata.type] = metadata;
}

// 元数据访问
ControlMetadata ControlRenderer::GetMetadata(const std::wstring& type) const
{
    auto it = m_controlMetadata.find(type);
    if (it != m_controlMetadata.end()) {
        return it->second;
    }
    return ControlMetadata();
}

std::vector<std::wstring> ControlRenderer::GetControlTypes() const
{
    std::vector<std::wstring> types;
    for (const auto& pair : m_controlMetadata) {
        types.push_back(pair.first);
    }
    return types;
}

std::vector<std::wstring> ControlRenderer::GetControlCategories() const
{
    std::set<std::wstring> categories;
    for (const auto& pair : m_controlMetadata) {
        categories.insert(pair.second.category);
    }
    return std::vector<std::wstring>(categories.begin(), categories.end());
}

std::vector<std::wstring> ControlRenderer::GetControlsInCategory(const std::wstring& category) const
{
    std::vector<std::wstring> controls;
    for (const auto& pair : m_controlMetadata) {
        if (pair.second.category == category) {
            controls.push_back(pair.first);
        }
    }
    return controls;
}

bool ControlRenderer::HasControl(const std::wstring& type) const
{
    return m_controlMetadata.find(type) != m_controlMetadata.end();
}

// 获取属性定义
std::vector<PropertyDef> ControlRenderer::GetPropertyDefs(const std::wstring& type) const
{
    std::vector<PropertyDef> defs;
    
    auto it = m_controlMetadata.find(type);
    if (it == m_controlMetadata.end()) {
        return defs;
    }
    
    const auto& metadata = it->second;
    for (const auto& propInfo : metadata.properties) {
        PropertyDef def;
        def.name = propInfo.englishName.empty() ? propInfo.name : propInfo.englishName;
        def.displayName = propInfo.name;
        def.editorType = propInfo.editorType;
        def.defaultValue = propInfo.defaultValue;
        def.enumValues = propInfo.enumValues;
        def.readOnly = false;
        
        // 分类
        if (propInfo.name == L"左边" || propInfo.name == L"顶边" || propInfo.name == L"宽度" || propInfo.name == L"高度") {
            def.category = L"位置和大小";
        } else if (propInfo.name == L"可视" || propInfo.name == L"禁止") {
            def.category = L"状态";
        } else if (propInfo.name.find(L"颜色") != std::wstring::npos) {
            def.category = L"外观";
        } else {
            def.category = L"属性";
        }
        
        defs.push_back(def);
    }
    
    return defs;
}

// 渲染控件
void ControlRenderer::RenderControl(Graphics& g, const ControlInfo& control, bool selected)
{
    if (control.type == L"按钮") {
        RenderButton(g, control);
    } else if (control.type == L"标签") {
        RenderLabel(g, control);
    } else if (control.type == L"编辑框") {
        RenderEditBox(g, control);
    } else if (control.type == L"选择框") {
        RenderCheckBox(g, control);
    } else if (control.type == L"单选框") {
        RenderRadioButton(g, control);
    } else if (control.type == L"组合框") {
        RenderComboBox(g, control);
    } else if (control.type == L"列表框") {
        RenderListBox(g, control);
    } else if (control.type == L"分组框") {
        RenderGroupBox(g, control);
    } else if (control.type == L"图片框") {
        RenderPictureBox(g, control);
    } else if (control.type == L"进度条") {
        RenderProgressBar(g, control);
    } else if (control.type == L"滑块条") {
        RenderSlider(g, control);
    } else if (control.type == L"选择夹") {
        RenderTabControl(g, control);
    } else {
        RenderDefault(g, control);
    }
}

void ControlRenderer::RenderControlPreview(Graphics& g, const std::wstring& type, const Rect& bounds)
{
    ControlInfo preview;
    preview.type = type;
    preview.bounds = bounds;
    preview.properties[L"标题"] = type;
    RenderControl(g, preview, false);
}

// 渲染方法实现
void ControlRenderer::RenderButton(Graphics& g, const ControlInfo& control)
{
    LinearGradientBrush bgBrush(
        Point(control.bounds.X, control.bounds.Y),
        Point(control.bounds.X, control.bounds.Y + control.bounds.Height),
        Color(245, 245, 245),
        Color(225, 225, 225)
    );
    
    Pen borderPen(Color(160, 160, 160), 1.0f);
    
    g.FillRectangle(&bgBrush, control.bounds);
    g.DrawRectangle(&borderPen, control.bounds);
    
    std::wstring title = GetPropertyValue(control, L"标题", L"按钮");
    
    FontFamily fontFamily(L"微软雅黑");
    Font font(&fontFamily, 9, FontStyleRegular, UnitPoint);
    SolidBrush textBrush(Color(0, 0, 0));
    
    StringFormat format;
    format.SetAlignment(StringAlignmentCenter);
    format.SetLineAlignment(StringAlignmentCenter);
    
    RectF textRect((REAL)control.bounds.X, (REAL)control.bounds.Y, 
                   (REAL)control.bounds.Width, (REAL)control.bounds.Height);
    g.DrawString(title.c_str(), -1, &font, textRect, &format, &textBrush);
}

void ControlRenderer::RenderLabel(Graphics& g, const ControlInfo& control)
{
    std::wstring title = GetPropertyValue(control, L"标题", L"标签");
    
    FontFamily fontFamily(L"微软雅黑");
    Font font(&fontFamily, 9, FontStyleRegular, UnitPoint);
    SolidBrush textBrush(Color(0, 0, 0));
    
    StringFormat format;
    format.SetAlignment(StringAlignmentNear);
    format.SetLineAlignment(StringAlignmentCenter);
    
    RectF textRect((REAL)control.bounds.X, (REAL)control.bounds.Y, 
                   (REAL)control.bounds.Width, (REAL)control.bounds.Height);
    g.DrawString(title.c_str(), -1, &font, textRect, &format, &textBrush);
}

void ControlRenderer::RenderEditBox(Graphics& g, const ControlInfo& control)
{
    SolidBrush bgBrush(Color(255, 255, 255));
    Pen borderPen(Color(122, 122, 122), 1.0f);
    
    g.FillRectangle(&bgBrush, control.bounds);
    g.DrawRectangle(&borderPen, control.bounds);
}

void ControlRenderer::RenderCheckBox(Graphics& g, const ControlInfo& control)
{
    Rect boxRect(control.bounds.X + 2, control.bounds.Y + (control.bounds.Height - 13) / 2, 13, 13);
    
    SolidBrush bgBrush(Color(255, 255, 255));
    Pen borderPen(Color(160, 160, 160), 1.0f);
    
    g.FillRectangle(&bgBrush, boxRect);
    g.DrawRectangle(&borderPen, boxRect);
    
    std::wstring title = GetPropertyValue(control, L"标题", L"选择框");
    
    FontFamily fontFamily(L"微软雅黑");
    Font font(&fontFamily, 9, FontStyleRegular, UnitPoint);
    SolidBrush textBrush(Color(0, 0, 0));
    
    RectF textRect((REAL)(control.bounds.X + 20), (REAL)control.bounds.Y, 
                   (REAL)(control.bounds.Width - 20), (REAL)control.bounds.Height);
    
    StringFormat format;
    format.SetAlignment(StringAlignmentNear);
    format.SetLineAlignment(StringAlignmentCenter);
    
    g.DrawString(title.c_str(), -1, &font, textRect, &format, &textBrush);
}

void ControlRenderer::RenderRadioButton(Graphics& g, const ControlInfo& control)
{
    int centerX = control.bounds.X + 8;
    int centerY = control.bounds.Y + control.bounds.Height / 2;
    
    SolidBrush bgBrush(Color(255, 255, 255));
    Pen borderPen(Color(160, 160, 160), 1.0f);
    
    g.FillEllipse(&bgBrush, centerX - 6, centerY - 6, 12, 12);
    g.DrawEllipse(&borderPen, centerX - 6, centerY - 6, 12, 12);
    
    std::wstring title = GetPropertyValue(control, L"标题", L"单选框");
    
    FontFamily fontFamily(L"微软雅黑");
    Font font(&fontFamily, 9, FontStyleRegular, UnitPoint);
    SolidBrush textBrush(Color(0, 0, 0));
    
    RectF textRect((REAL)(control.bounds.X + 20), (REAL)control.bounds.Y, 
                   (REAL)(control.bounds.Width - 20), (REAL)control.bounds.Height);
    
    StringFormat format;
    format.SetAlignment(StringAlignmentNear);
    format.SetLineAlignment(StringAlignmentCenter);
    
    g.DrawString(title.c_str(), -1, &font, textRect, &format, &textBrush);
}

void ControlRenderer::RenderComboBox(Graphics& g, const ControlInfo& control)
{
    SolidBrush bgBrush(Color(255, 255, 255));
    Pen borderPen(Color(122, 122, 122), 1.0f);
    
    g.FillRectangle(&bgBrush, control.bounds);
    g.DrawRectangle(&borderPen, control.bounds);
    
    // 绘制下拉按钮
    int btnWidth = 17;
    Rect btnRect(control.bounds.X + control.bounds.Width - btnWidth, control.bounds.Y, 
                 btnWidth, control.bounds.Height);
    
    LinearGradientBrush btnBrush(
        Point(btnRect.X, btnRect.Y),
        Point(btnRect.X, btnRect.Y + btnRect.Height),
        Color(245, 245, 245),
        Color(225, 225, 225)
    );
    
    g.FillRectangle(&btnBrush, btnRect);
    g.DrawLine(&borderPen, btnRect.X, btnRect.Y, btnRect.X, btnRect.Y + btnRect.Height);
    
    // 绘制箭头
    Point arrowPoints[3] = {
        Point(btnRect.X + 5, btnRect.Y + btnRect.Height / 2 - 2),
        Point(btnRect.X + 12, btnRect.Y + btnRect.Height / 2 - 2),
        Point(btnRect.X + 8, btnRect.Y + btnRect.Height / 2 + 2)
    };
    SolidBrush arrowBrush(Color(0, 0, 0));
    g.FillPolygon(&arrowBrush, arrowPoints, 3);
}

void ControlRenderer::RenderListBox(Graphics& g, const ControlInfo& control)
{
    SolidBrush bgBrush(Color(255, 255, 255));
    Pen borderPen(Color(122, 122, 122), 1.0f);
    
    g.FillRectangle(&bgBrush, control.bounds);
    g.DrawRectangle(&borderPen, control.bounds);
}

void ControlRenderer::RenderGroupBox(Graphics& g, const ControlInfo& control)
{
    std::wstring title = GetPropertyValue(control, L"标题", L"分组框");
    
    FontFamily fontFamily(L"微软雅黑");
    Font font(&fontFamily, 9, FontStyleRegular, UnitPoint);
    
    RectF titleRect;
    g.MeasureString(title.c_str(), -1, &font, PointF(0, 0), &titleRect);
    
    int titleWidth = (int)titleRect.Width + 8;
    int titleHeight = (int)titleRect.Height;
    
    Pen borderPen(Color(160, 160, 160), 1.0f);
    
    // 绘制上边框（分段）
    g.DrawLine(&borderPen, control.bounds.X, control.bounds.Y + titleHeight / 2,
               control.bounds.X + 8, control.bounds.Y + titleHeight / 2);
    g.DrawLine(&borderPen, control.bounds.X + 8 + titleWidth, control.bounds.Y + titleHeight / 2,
               control.bounds.X + control.bounds.Width, control.bounds.Y + titleHeight / 2);
    
    // 其他边框
    g.DrawLine(&borderPen, control.bounds.X, control.bounds.Y + titleHeight / 2,
               control.bounds.X, control.bounds.Y + control.bounds.Height);
    g.DrawLine(&borderPen, control.bounds.X, control.bounds.Y + control.bounds.Height,
               control.bounds.X + control.bounds.Width, control.bounds.Y + control.bounds.Height);
    g.DrawLine(&borderPen, control.bounds.X + control.bounds.Width, control.bounds.Y + control.bounds.Height,
               control.bounds.X + control.bounds.Width, control.bounds.Y + titleHeight / 2);
    
    // 绘制标题
    SolidBrush textBrush(Color(0, 0, 0));
    PointF titlePoint((REAL)(control.bounds.X + 12), (REAL)control.bounds.Y);
    g.DrawString(title.c_str(), -1, &font, titlePoint, &textBrush);
}

void ControlRenderer::RenderPictureBox(Graphics& g, const ControlInfo& control)
{
    SolidBrush bgBrush(Color(255, 255, 255));
    Pen borderPen(Color(160, 160, 160), 1.0f);
    
    g.FillRectangle(&bgBrush, control.bounds);
    g.DrawRectangle(&borderPen, control.bounds);
    
    // 绘制占位符图标
    Pen crossPen(Color(200, 200, 200), 1.0f);
    g.DrawLine(&crossPen, control.bounds.X, control.bounds.Y,
               control.bounds.X + control.bounds.Width, control.bounds.Y + control.bounds.Height);
    g.DrawLine(&crossPen, control.bounds.X + control.bounds.Width, control.bounds.Y,
               control.bounds.X, control.bounds.Y + control.bounds.Height);
}

void ControlRenderer::RenderProgressBar(Graphics& g, const ControlInfo& control)
{
    SolidBrush bgBrush(Color(240, 240, 240));
    Pen borderPen(Color(160, 160, 160), 1.0f);
    
    g.FillRectangle(&bgBrush, control.bounds);
    g.DrawRectangle(&borderPen, control.bounds);
    
    // 绘制进度（50%）
    int progressWidth = control.bounds.Width / 2;
    if (progressWidth > 2) {
        Rect progressRect(control.bounds.X + 2, control.bounds.Y + 2,
                         progressWidth - 2, control.bounds.Height - 4);
        LinearGradientBrush progressBrush(
            Point(progressRect.X, progressRect.Y),
            Point(progressRect.X, progressRect.Y + progressRect.Height),
            Color(104, 214, 104),
            Color(67, 181, 67)
        );
        g.FillRectangle(&progressBrush, progressRect);
    }
}

void ControlRenderer::RenderSlider(Graphics& g, const ControlInfo& control)
{
    int centerY = control.bounds.Y + control.bounds.Height / 2;
    
    // 绘制轨道
    Rect trackRect(control.bounds.X + 10, centerY - 2, control.bounds.Width - 20, 4);
    SolidBrush trackBrush(Color(200, 200, 200));
    g.FillRectangle(&trackBrush, trackRect);
    
    // 绘制滑块（居中位置）
    int thumbX = control.bounds.X + control.bounds.Width / 2;
    Rect thumbRect(thumbX - 5, control.bounds.Y + 2, 10, control.bounds.Height - 4);
    
    LinearGradientBrush thumbBrush(
        Point(thumbRect.X, thumbRect.Y),
        Point(thumbRect.X, thumbRect.Y + thumbRect.Height),
        Color(245, 245, 245),
        Color(215, 215, 215)
    );
    Pen thumbPen(Color(160, 160, 160), 1.0f);
    
    g.FillRectangle(&thumbBrush, thumbRect);
    g.DrawRectangle(&thumbPen, thumbRect);
}

void ControlRenderer::RenderTabControl(Graphics& g, const ControlInfo& control)
{
    Pen borderPen(Color(160, 160, 160), 1.0f);
    SolidBrush bgBrush(Color(240, 240, 240));
    
    // 绘制标签页头
    int tabHeight = 24;
    Rect tabRect(control.bounds.X, control.bounds.Y, 60, tabHeight);
    
    LinearGradientBrush tabBrush(
        Point(tabRect.X, tabRect.Y),
        Point(tabRect.X, tabRect.Y + tabRect.Height),
        Color(255, 255, 255),
        Color(235, 235, 235)
    );
    
    g.FillRectangle(&tabBrush, tabRect);
    g.DrawRectangle(&borderPen, tabRect);
    
    FontFamily fontFamily(L"微软雅黑");
    Font font(&fontFamily, 8, FontStyleRegular, UnitPoint);
    SolidBrush textBrush(Color(0, 0, 0));
    
    StringFormat format;
    format.SetAlignment(StringAlignmentCenter);
    format.SetLineAlignment(StringAlignmentCenter);
    
    RectF textRect((REAL)tabRect.X, (REAL)tabRect.Y, (REAL)tabRect.Width, (REAL)tabRect.Height);
    g.DrawString(L"标签1", -1, &font, textRect, &format, &textBrush);
    
    // 绘制内容区域
    Rect contentRect(control.bounds.X, control.bounds.Y + tabHeight,
                    control.bounds.Width, control.bounds.Height - tabHeight);
    g.FillRectangle(&bgBrush, contentRect);
    g.DrawRectangle(&borderPen, contentRect);
}

void ControlRenderer::RenderDefault(Graphics& g, const ControlInfo& control)
{
    SolidBrush bgBrush(Color(240, 240, 240));
    Pen borderPen(Color(128, 128, 128), 1.0f);
    Pen dashedPen(Color(180, 180, 180), 1.0f);
    dashedPen.SetDashStyle(DashStyleDash);
    
    g.FillRectangle(&bgBrush, control.bounds);
    g.DrawRectangle(&dashedPen, control.bounds);
    
    FontFamily fontFamily(L"微软雅黑");
    Font font(&fontFamily, 8, FontStyleRegular, UnitPoint);
    SolidBrush textBrush(Color(100, 100, 100));
    
    StringFormat format;
    format.SetAlignment(StringAlignmentCenter);
    format.SetLineAlignment(StringAlignmentCenter);
    
    RectF textRect((REAL)control.bounds.X, (REAL)control.bounds.Y, 
                   (REAL)control.bounds.Width, (REAL)control.bounds.Height);
    g.DrawString(control.type.c_str(), -1, &font, textRect, &format, &textBrush);
}

// 辅助方法
std::wstring ControlRenderer::GetPropertyValue(const ControlInfo& control, const std::wstring& propName, const std::wstring& defaultValue) const
{
    auto it = control.properties.find(propName);
    if (it != control.properties.end()) {
        return it->second;
    }
    return defaultValue;
}

Color ControlRenderer::ParseColor(const std::wstring& colorStr) const
{
    // 简单的颜色解析（整数RGB值）
    int colorValue = std::stoi(colorStr);
    int r = colorValue & 0xFF;
    int g = (colorValue >> 8) & 0xFF;
    int b = (colorValue >> 16) & 0xFF;
    return Color(255, (BYTE)r, (BYTE)g, (BYTE)b);
}

Font* ControlRenderer::CreateFontFromString(const std::wstring& fontStr) const
{
    // TODO: 解析字体字符串
    return new Font(L"微软雅黑", 9);
}
