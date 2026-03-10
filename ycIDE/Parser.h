#pragma once
#include "Lexer.h"
#include <memory>
#include <vector>

// 前向声明
struct ASTNode;

// AST节点类型
enum class ASTNodeType {
    // 程序结构
    PROGRAM,           // 程序根节点
    SUBROUTINE,        // 子程序
    
    // 声明
    VAR_DECL,          // 变量声明
    CONST_DECL,        // 常量声明
    PARAM_DECL,        // 参数声明
    
    // 语句
    EXPR_STMT,         // 表达式语句
    ASSIGN_STMT,       // 赋值语句
    IF_STMT,           // 如果语句
    WHILE_STMT,        // 循环语句
    FOR_STMT,          // 计次循环语句
    RETURN_STMT,       // 返回语句
    BREAK_STMT,        // 跳出循环语句
    CONTINUE_STMT,     // 继续循环语句
    BLOCK_STMT,        // 语句块
    
    // 表达式
    BINARY_EXPR,       // 二元表达式
    UNARY_EXPR,        // 一元表达式
    LITERAL_EXPR,      // 字面量表达式
    IDENTIFIER_EXPR,   // 标识符表达式
    CALL_EXPR,         // 函数调用表达式
    ARRAY_ACCESS,      // 数组访问
    MEMBER_ACCESS,     // 成员访问
    
    // 其他
    ERROR_NODE         // 错误节点
};

// AST节点基类
struct ASTNode {
    ASTNodeType type;
    int line;
    int column;
    std::wstring errorMsg;  // 错误信息
    
    ASTNode(ASTNodeType t, int ln = 0, int col = 0)
        : type(t), line(ln), column(col) {}
    
    virtual ~ASTNode() {}
};

// 程序节点
struct ProgramNode : public ASTNode {
    std::vector<std::shared_ptr<ASTNode>> declarations;  // 全局声明
    std::vector<std::shared_ptr<ASTNode>> subroutines;   // 子程序
    
    ProgramNode() : ASTNode(ASTNodeType::PROGRAM) {}
};

// 变量声明节点
struct VarDeclNode : public ASTNode {
    std::wstring name;       // 变量名
    std::wstring dataType;   // 数据类型
    bool isArray;            // 是否是数组
    std::vector<std::shared_ptr<ASTNode>> arrayDimensions; // 数组维度
    std::shared_ptr<ASTNode> initialValue; // 初始值（可选）
    bool isPublic;           // 是否公开
    
    VarDeclNode() : ASTNode(ASTNodeType::VAR_DECL), isArray(false), isPublic(false) {}
};

// 常量声明节点
struct ConstDeclNode : public ASTNode {
    std::wstring name;       // 常量名
    std::wstring dataType;   // 数据类型
    std::shared_ptr<ASTNode> value; // 常量值
    bool isPublic;           // 是否公开
    
    ConstDeclNode() : ASTNode(ASTNodeType::CONST_DECL), isPublic(false) {}
};

// 子程序节点
struct SubroutineNode : public ASTNode {
    std::wstring name;                                // 子程序名
    std::wstring returnType;                          // 返回值类型
    std::vector<std::shared_ptr<VarDeclNode>> params; // 参数列表
    std::vector<std::shared_ptr<ASTNode>> localVars;  // 局部变量
    std::vector<std::shared_ptr<ASTNode>> statements; // 语句列表
    bool isPublic;                                    // 是否公开
    
    SubroutineNode() : ASTNode(ASTNodeType::SUBROUTINE), isPublic(false) {}
};

// 赋值语句节点
struct AssignStmtNode : public ASTNode {
    std::shared_ptr<ASTNode> target;  // 赋值目标（变量/数组元素等）
    std::shared_ptr<ASTNode> value;   // 赋值值
    
    AssignStmtNode() : ASTNode(ASTNodeType::ASSIGN_STMT) {}
};

// 如果语句节点
struct IfStmtNode : public ASTNode {
    std::shared_ptr<ASTNode> condition;              // 条件表达式
    std::vector<std::shared_ptr<ASTNode>> thenBlock; // 条件为真的语句块
    std::vector<std::shared_ptr<ASTNode>> elseBlock; // 条件为假的语句块（可选）
    
    IfStmtNode() : ASTNode(ASTNodeType::IF_STMT) {}
};

// 循环语句节点
struct WhileStmtNode : public ASTNode {
    std::shared_ptr<ASTNode> condition;              // 循环条件
    std::vector<std::shared_ptr<ASTNode>> body;      // 循环体
    
    WhileStmtNode() : ASTNode(ASTNodeType::WHILE_STMT) {}
};

// 计次循环语句节点
struct ForStmtNode : public ASTNode {
    std::shared_ptr<ASTNode> loopCount;              // 循环次数
    std::wstring loopVar;                            // 已循环次数记录变量（可选）
    std::vector<std::shared_ptr<ASTNode>> body;      // 循环体
    
    ForStmtNode() : ASTNode(ASTNodeType::FOR_STMT) {}
};

// 返回语句节点
struct ReturnStmtNode : public ASTNode {
    std::shared_ptr<ASTNode> returnValue;  // 返回值（可选）
    
    ReturnStmtNode() : ASTNode(ASTNodeType::RETURN_STMT) {}
};

// 二元表达式节点
struct BinaryExprNode : public ASTNode {
    EYTokenType op;                     // 运算符
    std::shared_ptr<ASTNode> left;    // 左操作数
    std::shared_ptr<ASTNode> right;   // 右操作数
    
    BinaryExprNode() : ASTNode(ASTNodeType::BINARY_EXPR), op(EYTokenType::UNKNOWN) {}
};

// 一元表达式节点
struct UnaryExprNode : public ASTNode {
    EYTokenType op;                     // 运算符
    std::shared_ptr<ASTNode> operand; // 操作数
    
    UnaryExprNode() : ASTNode(ASTNodeType::UNARY_EXPR), op(EYTokenType::UNKNOWN) {}
};

// 字面量表达式节点
struct LiteralExprNode : public ASTNode {
    EYTokenType literalType;  // 字面量类型（NUMBER, STRING, TRUE, FALSE）
    std::wstring value;     // 字面量值
    
    LiteralExprNode() : ASTNode(ASTNodeType::LITERAL_EXPR), literalType(EYTokenType::UNKNOWN) {}
};

// 标识符表达式节点
struct IdentifierExprNode : public ASTNode {
    std::wstring name;  // 标识符名称
    
    IdentifierExprNode() : ASTNode(ASTNodeType::IDENTIFIER_EXPR) {}
};

// 函数调用表达式节点
struct CallExprNode : public ASTNode {
    std::wstring functionName;                        // 函数名
    std::vector<std::shared_ptr<ASTNode>> arguments;  // 参数列表
    
    CallExprNode() : ASTNode(ASTNodeType::CALL_EXPR) {}
};

// 数组访问节点
struct ArrayAccessNode : public ASTNode {
    std::wstring arrayName;                          // 数组名
    std::vector<std::shared_ptr<ASTNode>> indices;   // 索引表达式列表
    
    ArrayAccessNode() : ASTNode(ASTNodeType::ARRAY_ACCESS) {}
};

// 语法错误信息
struct SyntaxError {
    std::wstring message;  // 错误消息
    int line;              // 行号
    int column;            // 列号
    std::wstring context;  // 上下文（出错的代码片段）
    
    SyntaxError(const std::wstring& msg, int ln, int col, const std::wstring& ctx = L"")
        : message(msg), line(ln), column(col), context(ctx) {}
};

// 语法分析器
class Parser {
public:
    Parser();
    ~Parser();
    
    // 解析源代码，返回AST根节点
    std::shared_ptr<ProgramNode> Parse(const std::wstring& source);
    
    // 仅检查语法错误（不构建完整AST）
    std::vector<SyntaxError> CheckSyntax(const std::wstring& source);
    
    // 获取错误列表
    const std::vector<SyntaxError>& GetErrors() const { return errors_; }
    
    // 是否有错误
    bool HasErrors() const { return !errors_.empty(); }
    
private:
    Lexer lexer_;                        // 词法分析器
    Token currentToken_;                 // 当前Token
    Token previousToken_;                // 前一个Token
    std::vector<SyntaxError> errors_;    // 错误列表
    
    // Token操作
    void Advance();                      // 前进到下一个Token
    bool Check(EYTokenType type) const;    // 检查当前Token类型
    bool Match(EYTokenType type);          // 匹配并前进
    bool MatchMultiple(const std::vector<EYTokenType>& types); // 匹配多个类型之一
    Token Expect(EYTokenType type, const std::wstring& errorMsg); // 期望某个Token
    void Synchronize();                  // 错误恢复：跳到下一个语句开始
    
    // 错误处理
    void AddError(const std::wstring& message);
    void AddError(const std::wstring& message, const Token& token);
    
    // 解析方法（递归下降）
    std::shared_ptr<ProgramNode> ParseProgram();
    std::shared_ptr<ASTNode> ParseDeclaration();
    std::shared_ptr<VarDeclNode> ParseVarDecl();
    std::shared_ptr<ConstDeclNode> ParseConstDecl();
    std::shared_ptr<SubroutineNode> ParseSubroutine();
    
    std::shared_ptr<ASTNode> ParseStatement();
    std::shared_ptr<AssignStmtNode> ParseAssignment();
    std::shared_ptr<IfStmtNode> ParseIfStatement();
    std::shared_ptr<WhileStmtNode> ParseWhileStatement();
    std::shared_ptr<ForStmtNode> ParseForStatement();
    std::shared_ptr<ReturnStmtNode> ParseReturnStatement();
    
    std::shared_ptr<ASTNode> ParseExpression();
    std::shared_ptr<ASTNode> ParseLogicalOr();
    std::shared_ptr<ASTNode> ParseLogicalAnd();
    std::shared_ptr<ASTNode> ParseEquality();
    std::shared_ptr<ASTNode> ParseComparison();
    std::shared_ptr<ASTNode> ParseAddition();
    std::shared_ptr<ASTNode> ParseMultiplication();
    std::shared_ptr<ASTNode> ParseUnary();
    std::shared_ptr<ASTNode> ParsePrimary();
    std::shared_ptr<ASTNode> ParseCallOrAccess();
    
    // 辅助方法
    bool IsDataType(const Token& token) const;
    bool IsAtEnd() const;
    void SkipNewlines();  // 跳过换行符
};
