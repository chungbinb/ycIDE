#pragma once

#ifndef NOMINMAX
#define NOMINMAX
#endif

#include <windows.h>
#include <string>
#include <vector>

#define WM_AI_STREAM_RESULT (WM_USER + 1)

// 文件编辑信息结构
struct FileEditInfo {
    std::wstring filePath;
    std::wstring fileName;
    int startLine;
    int endLine;
    std::vector<std::wstring> originalContent;  // 原始内容
    std::vector<std::wstring> newContent;       // 新内容
    int addedLines;
    int deletedLines;
    bool applied;  // 是否已应用到编辑器
    
    FileEditInfo() : startLine(0), endLine(0), addedLines(0), deletedLines(0), applied(false) {}
};

// 全局变量：待处理的编辑列表
extern std::vector<FileEditInfo> g_PendingEdits;

// 消息块类型
enum MessageBlockType {
    BLOCK_TEXT,      // 普通文本
    BLOCK_CODE,      // 代码块
    BLOCK_FILE_READ, // 文件读取信息
    BLOCK_EDIT_INFO  // 编辑信息块
};

// 消息块结构
struct MessageBlock {
    MessageBlockType type;
    std::wstring content;
    RECT rect;       // 绘制区域（代码块用于滚动条）
    int scrollX;     // 代码块横向滚动位置
    int contentWidth; // 代码块内容实际宽度
    RECT copyBtnRect;   // 代码块复制按钮区域
    RECT insertBtnRect; // 代码块插入按钮区域
    RECT undoBtnRect;   // 代码块撤销按钮区域
    
    // 文件读取信息（BLOCK_FILE_READ）
    std::wstring fileName;
    int readStartLine;
    int readEndLine;
    
    // 编辑信息（BLOCK_EDIT_INFO）
    int editIndex;      // 编辑索引，用于关联g_PendingEdits
    int addedLines;     // 增加的行数
    int deletedLines;   // 删除的行数
    
    MessageBlock(MessageBlockType t, const std::wstring& c) : type(t), content(c), scrollX(0), contentWidth(0),
                                                               readStartLine(0), readEndLine(0),
                                                               editIndex(-1), addedLines(0), deletedLines(0) {
        rect = {0, 0, 0, 0};
        copyBtnRect = {0, 0, 0, 0};
        insertBtnRect = {0, 0, 0, 0};
        undoBtnRect = {0, 0, 0, 0};
    }
};

// 聊天消息结构
struct ChatMessage {
    bool isUser;                         // 是否是用户消息
    std::vector<MessageBlock> blocks;    // 消息块列表
    RECT messageRect;                    // 整个消息的区域
    RECT retryButtonRect;                // 重试按钮区域
    RECT copyAllButtonRect;              // 复制全部按钮区域
    RECT undoButtonRect;                 // 撤销按钮区域
    
    ChatMessage(bool user) : isUser(user) {
        messageRect = {0, 0, 0, 0};
        retryButtonRect = {0, 0, 0, 0};
        copyAllButtonRect = {0, 0, 0, 0};
        undoButtonRect = {0, 0, 0, 0};
    }
};

// 聊天窗口数据
struct AIChatData {
    std::vector<ChatMessage> messages;   // 所有消息
    int scrollY;                         // 垂直滚动位置
    int fontSize;                        // 字体大小
    int contentHeight;                   // 内容总高度
    
    // 自定义滚动条相关
    int scrollbarWidth;                  // 滚动条宽度
    RECT vScrollbarRect;                 // 垂直滚动条区域
    RECT vScrollThumbRect;               // 垂直滚动条滑块区域
    bool vScrollHover;                   // 滚动条是否悬停
    bool isDraggingVScroll;              // 是否正在拖动垂直滚动条
    int dragStartY;                      // 拖动起始Y位置
    int dragStartScrollY;                // 拖动起始滚动位置    
    // 右边框拖动相关
    bool isRightBorderHover;             // 右边框是否悬停
    bool isDraggingRightBorder;          // 是否正在拖动右边框
    int borderDragStartX;                // 边框拖动起始位置    
    // 选中相关
    bool isSelecting;                    // 是否正在选中
    int selStartMsgIdx;                  // 选中起始消息索引
    int selStartBlockIdx;                // 选中起始块索引
    int selStartChar;                    // 选中起始字符
    int selEndMsgIdx;                    // 选中结束消息索引
    int selEndBlockIdx;                  // 选中结束块索引
    int selEndChar;                      // 选中结束字符
    
    // 流式输出相关
    bool isStreaming;                    // 是否正在流式输出
    int streamingMsgIdx;                 // 正在流式输出的消息索引
    std::wstring streamBuffer;           // 流式输出缓冲区
    
    // 动画相关
    int animationFrame;                  // 动画帧索引
    
    // Tooltip 相关
    HWND hTooltip;                       // Tooltip 窗口句柄
    int hoveredButton;                   // 当前悬停的按钮索引 (0=无, 1=重试, 2=复制, 3=撤销)
    size_t hoveredMsgIdx;                // 当前悬停按钮所属的消息索引
    
    // 代码块按钮悬停状态
    int hoveredCodeButton;               // 当前悬停的代码块按钮 (0=无, 1=复制, 2=插入)
    size_t hoveredCodeMsgIdx;            // 悬停按钮所属的消息索引
    size_t hoveredCodeBlockIdx;          // 悬停按钮所属的代码块索引
    
    bool isSendButtonHovered;            // 发送按钮是否悬停
    
    AIChatData() : scrollY(0), fontSize(16), contentHeight(0), 
                   scrollbarWidth(12), vScrollHover(false), isDraggingVScroll(false),
                   dragStartY(0), dragStartScrollY(0),
                   isRightBorderHover(false), isDraggingRightBorder(false), borderDragStartX(0),
                   isSelecting(false),
                   selStartMsgIdx(-1), selStartBlockIdx(-1), selStartChar(0),
                   selEndMsgIdx(-1), selEndBlockIdx(-1), selEndChar(0),
                   isStreaming(false), streamingMsgIdx(-1), animationFrame(0), hTooltip(nullptr),
                   hoveredButton(0), hoveredMsgIdx(0),
                   hoveredCodeButton(0), hoveredCodeMsgIdx(0), hoveredCodeBlockIdx(0),
                   isSendButtonHovered(false) {
        vScrollbarRect = {0, 0, 0, 0};
        vScrollThumbRect = {0, 0, 0, 0};
    }
};

extern int g_CurrentChatMode;
extern int g_CurrentModelIndex;

extern HWND hChatWindow;      // AI聊天窗口
extern HWND hChatInput;       // 输入框
extern HWND hChatSendBtn;     // 发送按钮
extern HWND hChatModeCombo;   // 模式选择
extern HWND hChatModelCombo;  // 模型选择
extern HFONT hChatControlFont;

// 输入框边框颜色（默认蓝色，支持自定义）
extern COLORREF g_InputBorderColor;

// 图标句柄
extern HBITMAP g_hSendIcon;
extern HBITMAP g_hPauseIcon;
extern HBITMAP g_hRetryIcon;
extern HBITMAP g_hCopyIcon;
extern HBITMAP g_hUndoIcon;
extern HBITMAP g_hCodeCopyIcon;   // 代码块复制按钮
extern HBITMAP g_hCodeInsertIcon; // 代码块插入按钮

// 注册AI聊天窗口类
ATOM RegisterAIChatWindowClass(HINSTANCE hInstance);

// 窗口过程
LRESULT CALLBACK AIChatWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK ChatInputSubclassProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK AIChatWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

// 辅助函数
void ParseMessageBlocks(const std::wstring& content, std::vector<MessageBlock>& blocks, bool skipCodeBlocks = false);
void AddUserMessage(AIChatData* data, const std::wstring& content);
void StartAIMessage(AIChatData* data);
void AppendAIMessageStream(AIChatData* data, const std::wstring& chunk);
void FinishAIMessage(AIChatData* data);
