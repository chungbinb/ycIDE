#include "ControlToolbox.h"
#include "EditorContext.h"
#include "ControlRenderer.h"

// ControlToolbox 构造函数
ControlToolbox::ControlToolbox(HWND hWnd)
    : m_hWnd(hWnd)
{
}

ControlToolbox::~ControlToolbox()
{
}

// 暂未实现的占位方法
void ControlToolbox::OnPaint(HDC hdc)
{
    // TODO: 实现绘制
}

void ControlToolbox::OnLButtonDown(int x, int y)
{
    // TODO: 实现鼠标点击
}
