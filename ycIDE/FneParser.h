#pragma once
#include <windows.h>
#include <string>
#include <vector>
#include <map>

// 基于 Yi 语言官方 lib2.h 中的结构定义
typedef unsigned long DATA_TYPE;

// 系统基本数据类型定义
#define SDT_BYTE        (DATA_TYPE)MAKELONG(MAKEWORD(1, 1), 0x8000)
#define SDT_SHORT       (DATA_TYPE)MAKELONG(MAKEWORD(1, 2), 0x8000)
#define SDT_INT         (DATA_TYPE)MAKELONG(MAKEWORD(1, 3), 0x8000)
#define SDT_INT64       (DATA_TYPE)MAKELONG(MAKEWORD(1, 4), 0x8000)
#define SDT_FLOAT       (DATA_TYPE)MAKELONG(MAKEWORD(1, 5), 0x8000)
#define SDT_DOUBLE      (DATA_TYPE)MAKELONG(MAKEWORD(1, 6), 0x8000)
#define SDT_BOOL        (DATA_TYPE)MAKELONG(MAKEWORD(2, 0), 0x8000)
#define SDT_DATE_TIME   (DATA_TYPE)MAKELONG(MAKEWORD(3, 0), 0x8000)
#define SDT_TEXT        (DATA_TYPE)MAKELONG(MAKEWORD(4, 0), 0x8000)
#define SDT_BIN         (DATA_TYPE)MAKELONG(MAKEWORD(5, 0), 0x8000)
#define SDT_SUB_PTR     (DATA_TYPE)MAKELONG(MAKEWORD(6, 0), 0x8000)

// 参数标志
#define AS_HAS_DEFAULT_VALUE       (1 << 0)
#define AS_DEFAULT_VALUE_IS_EMPTY  (1 << 1)
#define AS_RECEIVE_VAR             (1 << 2)
#define AS_RECEIVE_VAR_ARRAY       (1 << 3)
#define AS_RECEIVE_VAR_OR_ARRAY    (1 << 4)
#define AS_RECEIVE_ARRAY_DATA      (1 << 5)
#define AS_RECEIVE_ALL_TYPE_DATA   (1 << 6)
#define AS_RECEIVE_VAR_OR_OTHER    (1 << 9)

// 命令标志
#define CT_IS_HIDED                (1 << 2)
#define CT_RETRUN_ARY_TYPE_DATA    (1 << 6)
#define CT_IS_OBJ_COPY_CMD         (1 << 7)
#define CT_IS_OBJ_FREE_CMD         (1 << 8)
#define CT_IS_OBJ_CONSTURCT_CMD    (1 << 9)

// 取执行平台
#define _CMD_OS(os)     ((os) >> 16)

// 参数信息结构
typedef struct {
    const char* m_szName;           // 参数名称
    const char* m_szExplain;        // 参数详细说明
    short       m_shtBitmapIndex;   // 指定图标索引
    short       m_shtBitmapCount;   // 图标数目
    DATA_TYPE   m_dtType;           // 参数数据类型
    int         m_nDefault;         // 默认值
    DWORD       m_dwState;          // 参数状态
} ARG_INFO, * PARG_INFO;

// 命令信息结构
typedef struct {
    const char* m_szName;           // 命令名称
    const char* m_szEgName;         // 命令英文名
    const char* m_szExplain;        // 详细说明
    short       m_shtCategory;      // 命令类别
    WORD        m_wState;           // 命令状态
    DATA_TYPE   m_dtRetValType;     // 返回值类型
    WORD        m_wReserved;        // 保留
    short       m_shtUserLevel;     // 用户级别
    short       m_shtBitmapIndex;   // 图标索引
    short       m_shtBitmapCount;   // 图标数目
    int         m_nArgCount;        // 参数数目
    PARG_INFO   m_pBeginArgInfo;    // 参数信息指针
} CMD_INFO, * PCMD_INFO;

// 数据类型标志
#define LDT_IS_HIDED            (1 << 0)    // 是否隐藏
#define LDT_IS_ERROR            (1 << 1)    // 是否错误
#define LDT_WIN_UNIT            (1 << 6)    // 是否窗口组件
#define LDT_IS_CONTAINER        (1 << 7)    // 是否容器组件
#define LDT_IS_TAB_UNIT         (1 << 8)    // 是否Tab控件
#define LDT_CANNOT_GET_FOCUS    (1 << 16)   // 不能获取焦点

// 属性标志
#define UW_HAS_INDENT           (1 << 0)    // 有缩进
#define UW_GROUP_LINE           (1 << 1)    // 分组线
#define UW_ONLY_READ            (1 << 2)    // 只读
#define UW_CANNOT_INIT          (1 << 3)    // 不能初始化
#define UW_IS_HIDED             (1 << 4)    // 隐藏

// 事件标志
#define EV_IS_HIDED             (1 << 0)    // 隐藏事件
#define EV_IS_KEY_EVENT         (1 << 2)    // 键盘事件
#define EV_RETURN_INT           (1 << 3)    // 返回整数
#define EV_RETURN_BOOL          (1 << 4)    // 返回逻辑值
#define EV_IS_VER2              (1 << 31)   // 第二版事件结构

// 事件参数标志
#define EAS_BY_REF              (1 << 1)    // 引用传递

// 窗口组件属性结构（原始）
typedef struct {
    const char* m_szName;       // 属性名称
    const char* m_szEgName;     // 英文名称
    const char* m_szExplain;    // 属性说明
    short       m_shtType;      // 属性类型
    WORD        m_wState;       // 状态标志
    const char* m_szzPickStr;   // 选择字符串
} UNIT_PROPERTY_RAW, * PUNIT_PROPERTY_RAW;

// 事件参数结构（第二版）
typedef struct {
    const char* m_szName;       // 参数名称
    const char* m_szExplain;    // 参数说明
    DWORD       m_dwState;      // 状态
    DATA_TYPE   m_dtDataType;   // 数据类型
} EVENT_ARG_INFO2_RAW, * PEVENT_ARG_INFO2_RAW;

// 事件结构（第二版）
typedef struct {
    const char* m_szName;       // 事件名称
    const char* m_szExplain;    // 事件说明
    DWORD       m_dwState;      // 状态
    int         m_nArgCount;    // 参数数目
    PEVENT_ARG_INFO2_RAW m_pEventArgInfo;  // 参数信息
    DATA_TYPE   m_dtRetDataType;  // 返回类型
} EVENT_INFO2_RAW, * PEVENT_INFO2_RAW;

// 自定义数据类型结构
typedef struct {
    const char* m_szName;           // 数据类型名称
    const char* m_szEgName;         // 英文名称
    const char* m_szExplain;        // 说明
    int         m_nCmdCount;        // 成员命令数目
    int*        m_pnCmdsIndex;      // 成员命令索引
    DWORD       m_dwState;          // 状态标志
    DWORD       m_dwUnitBmpID;      // 图标ID
    int         m_nEventCount;      // 事件数目
    void*       m_pEventBegin;      // 事件信息
    int         m_nPropertyCount;   // 属性数目
    void*       m_pPropertyBegin;   // 属性信息
    void*       m_pfnGetInterface;  // 接口函数
    int         m_nElementCount;    // 成员数目
    void*       m_pElementBegin;    // 成员信息
} LIB_DATA_TYPE_INFO, * PLIB_DATA_TYPE_INFO;

// 支持库信息结构
typedef struct {
    DWORD       m_dwLibFormatVer;   // 库格式版本
    const char* m_szGuid;           // GUID
    int         m_nMajorVersion;    // 主版本号
    int         m_nMinorVersion;    // 次版本号
    int         m_nBuildNumber;     // 构建版本号
    int         m_nRqSysMajorVer;   // 需要系统主版本
    int         m_nRqSysMinorVer;   // 需要系统次版本
    int         m_nRqSysKrnlLibMajorVer;
    int         m_nRqSysKrnlLibMinorVer;
    const char* m_szName;           // 支持库名称
    int         m_nLanguage;        // 语言
    const char* m_szExplain;        // 说明
    DWORD       m_dwState;          // 状态
    const char* m_szAuthor;         // 作者
    const char* m_szZipCode;        // 邮编
    const char* m_szAddress;        // 地址
    const char* m_szPhoto;          // 电话
    const char* m_szFax;            // 传真
    const char* m_szEmail;          // 邮箱
    const char* m_szHomePage;       // 主页
    const char* m_szOther;          // 其它信息
    int         m_nDataTypeCount;   // 自定义数据类型数目
    PLIB_DATA_TYPE_INFO m_pDataType; // 数据类型
    int         m_nCategoryCount;   // 命令类别数目
    const char* m_szzCategory;      // 命令类别名称
    int         m_nCmdCount;        // 命令数目
    PCMD_INFO   m_pBeginCmdInfo;    // 命令信息数组
    void**      m_pCmdsFunc;        // 命令执行函数
    void*       m_pfnRunAddInFn;
    const char* m_szzAddInFnInfo;
    void*       m_pfnNotify;
    void*       m_pfnSuperTemplate;
    const char* m_szzSuperTemplateInfo;
    int         m_nLibConstCount;
    void*       m_pLibConst;
    const char* m_szzDependFiles;
} LIB_INFO, * PLIB_INFO;

// GetNewInf 函数原型
typedef PLIB_INFO(WINAPI* PFN_GET_LIB_INFO)();

// 窗口组件属性类型
enum class PropertyType {
    PickSpecInt = 1000,  // UD_PICK_SPEC_INT
    Int = 1001,          // UD_INT
    Double = 1002,       // UD_DOUBLE
    Bool = 1003,         // UD_BOOL
    DateTime = 1004,     // UD_DATE_TIME
    Text = 1005,         // UD_TEXT
    PickInt = 1006,      // UD_PICK_INT
    PickText = 1007,     // UD_PICK_TEXT
    EditPickText = 1008, // UD_EDIT_PICK_TEXT
    Picture = 1009,      // UD_PIC
    Icon = 1010,         // UD_ICON
    Cursor = 1011,       // UD_CURSOR
    Music = 1012,        // UD_MUSIC
    Font = 1013,         // UD_FONT
    Color = 1014,        // UD_COLOR
    ColorTrans = 1015,   // UD_COLOR_TRANS
    FileName = 1016,     // UD_FILE_NAME
    ColorBack = 1017,    // UD_COLOR_BACK
    ImageList = 1023,    // UD_IMAGE_LIST
    Customize = 1024     // UD_CUSTOMIZE
};

// 窗口组件属性信息
struct FnePropertyInfo {
    std::wstring name;           // 属性名称
    std::wstring englishName;    // 英文名称
    std::wstring description;    // 属性说明
    PropertyType type;           // 属性类型
    bool isReadOnly;             // 是否只读
    bool isHidden;               // 是否隐藏
    std::vector<std::wstring> pickOptions;  // 选择选项（用于PickInt等类型）
};

// 事件参数信息
struct FneEventArgInfo {
    std::wstring name;           // 参数名称
    std::wstring description;    // 参数说明
    std::wstring dataType;       // 参数数据类型
    bool isByRef;                // 是否引用传递
};

// 窗口组件事件信息
struct FneEventInfo {
    std::wstring name;           // 事件名称
    std::wstring description;    // 事件说明
    bool isHidden;               // 是否隐藏
    bool isKeyEvent;             // 是否键盘事件
    bool returnsInt;             // 是否返回整数
    bool returnsBool;            // 是否返回逻辑值
    std::vector<FneEventArgInfo> arguments;  // 事件参数
};

// 窗口组件信息
struct FneWindowUnitInfo {
    std::wstring name;           // 组件名称
    std::wstring englishName;    // 英文名称
    std::wstring description;    // 组件说明
    std::wstring libraryName;    // 所属支持库名称
    bool isContainer;            // 是否容器组件
    bool isTabUnit;              // 是否Tab控件
    bool canGetFocus;            // 是否可获取焦点
    DWORD bitmapId;              // 图标资源ID
    std::vector<FnePropertyInfo> properties;  // 属性列表
    std::vector<FneEventInfo> events;         // 事件列表
    std::vector<int> memberCommands;          // 成员命令索引
};

// FNE 数据类型信息
struct FneDataTypeInfo {
    std::wstring name;          // 数据类型名称
    std::wstring englishName;   // 英文名称
    std::wstring description;   // 说明
    bool isHidden;              // 是否隐藏
    bool isWindowUnit;          // 是否窗口组件
};

// FNE 命令信息类
struct FneCommandInfo {
    std::wstring name;          // 命令名称
    std::wstring description;   // 命令说明
    std::wstring returnType;    // 返回值类型
    std::wstring category;      // 命令类别（如：流程控制、算术运算等）
    std::vector<std::wstring> parameters;  // 参数列表
    bool isHidden;              // 是否隐藏命令
};

// FNE 解析器
class FneParser {
public:
    FneParser();
    ~FneParser();

    // 加载 .fne 文件
    bool LoadFneFile(const std::wstring& filePath);

    // 获取所有命令信息
    const std::vector<FneCommandInfo>& GetCommands() const { return commands; }

    // 获取所有数据类型
    const std::vector<FneDataTypeInfo>& GetDataTypes() const { return dataTypes; }

    // 获取所有窗口组件
    const std::vector<FneWindowUnitInfo>& GetWindowUnits() const { return windowUnits; }

    // 获取支持库名称
    std::wstring GetLibraryName() const { return libraryName; }

    // 获取支持库说明
    std::wstring GetLibraryDescription() const { return libraryDescription; }

    // 根据名称查找窗口组件
    const FneWindowUnitInfo* FindWindowUnit(const std::wstring& name) const;

private:
    HMODULE hModule;                    // DLL 句柄
    std::vector<FneCommandInfo> commands;  // 命令信息
    std::vector<FneDataTypeInfo> dataTypes; // 数据类型信息
    std::vector<FneWindowUnitInfo> windowUnits; // 窗口组件信息
    std::wstring libraryName;           // 支持库名称
    std::wstring libraryDescription;    // 支持库说明

    // 数据类型转换为字符串
    std::wstring DataTypeToString(DATA_TYPE dataType);

    // UTF-8 转 UTF-16
    std::wstring UTF8ToUTF16(const char* utf8Str);
    
    // 解析窗口组件属性
    void ParseWindowUnitProperties(PLIB_DATA_TYPE_INFO pDataTypeInfo, FneWindowUnitInfo& unitInfo);
    
    // 解析窗口组件事件
    void ParseWindowUnitEvents(PLIB_DATA_TYPE_INFO pDataTypeInfo, FneWindowUnitInfo& unitInfo);
};
