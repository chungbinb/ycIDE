// 词法和语法分析器测试程序
#include "Lexer.h"
#include "Parser.h"
#include <iostream>
#include <io.h>
#include <fcntl.h>

// 设置控制台输出为Unicode
void SetupConsole() {
    _setmode(_fileno(stdout), _O_U16TEXT);
    _setmode(_fileno(stdin), _O_U16TEXT);
}

// 打印Token
void PrintToken(const Token& token) {
    std::wcout << L"[" << token.line << L":" << token.column << L"] ";
    std::wcout << token.GetTypeName() << L" ";
    
    if (!token.value.empty()) {
        std::wcout << L"'" << token.value << L"'";
    }
    
    if (token.type == TokenType::ERROR) {
        std::wcout << L" ERROR: " << token.errorMsg;
    }
    
    std::wcout << std::endl;
}

// 测试词法分析器
void TestLexer() {
    std::wcout << L"\n========== 测试词法分析器 ==========\n" << std::endl;
    
    std::wstring testCode = LR"(
变量 计数器 整数型 = 0
常量 最大值 整数型 = 100

子程序 计算总和 整数型
    变量 总和 整数型
    变量 i 整数型
    
    总和 = 0
    计次循环首(i, 1, 最大值, 1)
        总和 = 总和 + i
    到循环尾
    
    返回(总和)
)";
    
    Lexer lexer;
    lexer.SetSource(testCode);
    
    std::wcout << L"源代码:\n" << testCode << std::endl;
    std::wcout << L"\n扫描Token:\n" << std::endl;
    
    auto tokens = lexer.ScanAll();
    for (const auto& token : tokens) {
        if (token.type != TokenType::WHITESPACE && 
            token.type != TokenType::NEWLINE) {
            PrintToken(token);
        }
    }
}

// 打印AST节点（简化版）
void PrintAST(const std::shared_ptr<ASTNode>& node, int indent = 0) {
    if (!node) return;
    
    std::wstring indentStr(indent * 2, L' ');
    
    switch (node->type) {
        case ASTNodeType::PROGRAM:
        {
            std::wcout << indentStr << L"Program" << std::endl;
            auto program = std::static_pointer_cast<ProgramNode>(node);
            for (const auto& decl : program->declarations) {
                PrintAST(decl, indent + 1);
            }
            for (const auto& sub : program->subroutines) {
                PrintAST(sub, indent + 1);
            }
            break;
        }
        
        case ASTNodeType::VAR_DECL:
        {
            auto varDecl = std::static_pointer_cast<VarDeclNode>(node);
            std::wcout << indentStr << L"VarDecl: " << varDecl->name;
            if (!varDecl->dataType.empty()) {
                std::wcout << L" (" << varDecl->dataType << L")";
            }
            std::wcout << std::endl;
            break;
        }
        
        case ASTNodeType::CONST_DECL:
        {
            auto constDecl = std::static_pointer_cast<ConstDeclNode>(node);
            std::wcout << indentStr << L"ConstDecl: " << constDecl->name;
            if (!constDecl->dataType.empty()) {
                std::wcout << L" (" << constDecl->dataType << L")";
            }
            std::wcout << std::endl;
            break;
        }
        
        case ASTNodeType::SUBROUTINE:
        {
            auto sub = std::static_pointer_cast<SubroutineNode>(node);
            std::wcout << indentStr << L"Subroutine: " << sub->name;
            if (!sub->returnType.empty()) {
                std::wcout << L" -> " << sub->returnType;
            }
            std::wcout << std::endl;
            
            for (const auto& var : sub->localVars) {
                PrintAST(var, indent + 1);
            }
            for (const auto& stmt : sub->statements) {
                PrintAST(stmt, indent + 1);
            }
            break;
        }
        
        case ASTNodeType::ASSIGN_STMT:
        {
            std::wcout << indentStr << L"Assignment" << std::endl;
            auto assign = std::static_pointer_cast<AssignStmtNode>(node);
            PrintAST(assign->target, indent + 1);
            PrintAST(assign->value, indent + 1);
            break;
        }
        
        case ASTNodeType::IF_STMT:
        {
            std::wcout << indentStr << L"If" << std::endl;
            auto ifStmt = std::static_pointer_cast<IfStmtNode>(node);
            std::wcout << indentStr << L"  Condition:" << std::endl;
            PrintAST(ifStmt->condition, indent + 2);
            std::wcout << indentStr << L"  Then:" << std::endl;
            for (const auto& stmt : ifStmt->thenBlock) {
                PrintAST(stmt, indent + 2);
            }
            if (!ifStmt->elseBlock.empty()) {
                std::wcout << indentStr << L"  Else:" << std::endl;
                for (const auto& stmt : ifStmt->elseBlock) {
                    PrintAST(stmt, indent + 2);
                }
            }
            break;
        }
        
        case ASTNodeType::FOR_STMT:
        {
            std::wcout << indentStr << L"For" << std::endl;
            auto forStmt = std::static_pointer_cast<ForStmtNode>(node);
            std::wcout << indentStr << L"  Var: " << forStmt->loopVar << std::endl;
            for (const auto& stmt : forStmt->body) {
                PrintAST(stmt, indent + 1);
            }
            break;
        }
        
        case ASTNodeType::RETURN_STMT:
        {
            std::wcout << indentStr << L"Return" << std::endl;
            auto returnStmt = std::static_pointer_cast<ReturnStmtNode>(node);
            if (returnStmt->returnValue) {
                PrintAST(returnStmt->returnValue, indent + 1);
            }
            break;
        }
        
        case ASTNodeType::BINARY_EXPR:
        {
            auto binary = std::static_pointer_cast<BinaryExprNode>(node);
            std::wcout << indentStr << L"BinaryOp" << std::endl;
            PrintAST(binary->left, indent + 1);
            PrintAST(binary->right, indent + 1);
            break;
        }
        
        case ASTNodeType::IDENTIFIER_EXPR:
        {
            auto id = std::static_pointer_cast<IdentifierExprNode>(node);
            std::wcout << indentStr << L"Identifier: " << id->name << std::endl;
            break;
        }
        
        case ASTNodeType::LITERAL_EXPR:
        {
            auto literal = std::static_pointer_cast<LiteralExprNode>(node);
            std::wcout << indentStr << L"Literal: " << literal->value << std::endl;
            break;
        }
        
        case ASTNodeType::CALL_EXPR:
        {
            auto call = std::static_pointer_cast<CallExprNode>(node);
            std::wcout << indentStr << L"Call: " << call->functionName << std::endl;
            for (const auto& arg : call->arguments) {
                PrintAST(arg, indent + 1);
            }
            break;
        }
        
        default:
            std::wcout << indentStr << L"Node (type=" << (int)node->type << L")" << std::endl;
            break;
    }
}

// 测试语法分析器
void TestParser() {
    std::wcout << L"\n========== 测试语法分析器 ==========\n" << std::endl;
    
    std::wstring testCode = LR"(
变量 x 整数型
常量 PI 小数型 = 3.14

子程序 测试函数 整数型
    变量 结果 整数型
    结果 = x + 10
    返回(结果)
)";
    
    std::wcout << L"源代码:\n" << testCode << std::endl;
    
    Parser parser;
    auto ast = parser.Parse(testCode);
    
    if (parser.HasErrors()) {
        std::wcout << L"\n语法错误:\n" << std::endl;
        for (const auto& error : parser.GetErrors()) {
            std::wcout << L"[" << error.line << L":" << error.column << L"] ";
            std::wcout << error.message;
            if (!error.context.empty()) {
                std::wcout << L" (在 '" << error.context << L"' 附近)";
            }
            std::wcout << std::endl;
        }
    } else {
        std::wcout << L"\n语法分析成功!" << std::endl;
    }
    
    std::wcout << L"\nAST树:\n" << std::endl;
    PrintAST(ast);
}

// 测试语法检查
void TestSyntaxCheck() {
    std::wcout << L"\n========== 测试语法检查 ==========\n" << std::endl;
    
    // 正确的代码
    std::wstring correctCode = L"变量 x 整数型 = 100";
    std::wcout << L"测试正确代码: " << correctCode << std::endl;
    
    Parser parser1;
    auto errors1 = parser1.CheckSyntax(correctCode);
    if (errors1.empty()) {
        std::wcout << L"✓ 无语法错误" << std::endl;
    } else {
        std::wcout << L"✗ 发现错误:" << std::endl;
        for (const auto& error : errors1) {
            std::wcout << L"  [" << error.line << L":" << error.column << L"] " << error.message << std::endl;
        }
    }
    
    // 错误的代码
    std::wstring errorCode = L"变量  整数型 = 100";  // 缺少变量名
    std::wcout << L"\n测试错误代码: " << errorCode << std::endl;
    
    Parser parser2;
    auto errors2 = parser2.CheckSyntax(errorCode);
    if (errors2.empty()) {
        std::wcout << L"✓ 无语法错误" << std::endl;
    } else {
        std::wcout << L"✗ 发现错误:" << std::endl;
        for (const auto& error : errors2) {
            std::wcout << L"  [" << error.line << L":" << error.column << L"] " << error.message << std::endl;
        }
    }
}

int main() {
    SetupConsole();
    
    std::wcout << L"易语言词法和语法分析器测试程序\n" << std::endl;
    std::wcout << L"====================================\n" << std::endl;
    
    try {
        TestLexer();
        TestParser();
        TestSyntaxCheck();
        
        std::wcout << L"\n====================================\n" << std::endl;
        std::wcout << L"测试完成!" << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
