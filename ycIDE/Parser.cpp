#include "Parser.h"

Parser::Parser() {
}

Parser::~Parser() {
}

std::shared_ptr<ProgramNode> Parser::Parse(const std::wstring& source) {
    errors_.clear();
    lexer_.SetSource(source);
    Advance(); // 获取第一个Token
    
    return ParseProgram();
}

std::vector<SyntaxError> Parser::CheckSyntax(const std::wstring& source) {
    Parse(source);
    return errors_;
}

void Parser::Advance() {
    previousToken_ = currentToken_;
    currentToken_ = lexer_.NextToken();
    
    // 跳过注释和空白
    while (currentToken_.type == EYTokenType::COMMENT || 
           currentToken_.type == EYTokenType::WHITESPACE) {
        currentToken_ = lexer_.NextToken();
    }
}

bool Parser::Check(EYTokenType type) const {
    return currentToken_.type == type;
}

bool Parser::Match(EYTokenType type) {
    if (Check(type)) {
        Advance();
        return true;
    }
    return false;
}

bool Parser::MatchMultiple(const std::vector<EYTokenType>& types) {
    for (EYTokenType type : types) {
        if (Match(type)) {
            return true;
        }
    }
    return false;
}

Token Parser::Expect(EYTokenType type, const std::wstring& errorMsg) {
    if (Check(type)) {
        Token token = currentToken_;
        Advance();
        return token;
    }
    
    AddError(errorMsg);
    return currentToken_;
}

void Parser::Synchronize() {
    Advance();
    
    while (!IsAtEnd()) {
        // 在语句边界处停止
        if (previousToken_.type == EYTokenType::NEWLINE) {
            return;
        }
        
        // 在关键字前停止
        switch (currentToken_.type) {
            case EYTokenType::KEYWORD_SUB:
            case EYTokenType::KEYWORD_IF:
            case EYTokenType::KEYWORD_WHILE:
            case EYTokenType::KEYWORD_FOR:
            case EYTokenType::KEYWORD_VAR:
            case EYTokenType::KEYWORD_CONST:
            case EYTokenType::KEYWORD_RETURN:
                return;
            default:
                break;
        }
        
        Advance();
    }
}

void Parser::AddError(const std::wstring& message) {
    AddError(message, currentToken_);
}

void Parser::AddError(const std::wstring& message, const Token& token) {
    errors_.push_back(SyntaxError(message, token.line, token.column, token.value));
}

void Parser::SkipNewlines() {
    while (Match(EYTokenType::NEWLINE)) {
        // 继续跳过
    }
}

bool Parser::IsDataType(const Token& token) const {
    return token.IsDataType();
}

bool Parser::IsAtEnd() const {
    return currentToken_.type == EYTokenType::EOF_TOKEN;
}

// ========== 解析方法 ==========

std::shared_ptr<ProgramNode> Parser::ParseProgram() {
    auto program = std::make_shared<ProgramNode>();
    
    SkipNewlines();
    
    while (!IsAtEnd()) {
        // 跳过空行
        if (Match(EYTokenType::NEWLINE)) {
            continue;
        }
        
        auto decl = ParseDeclaration();
        if (decl) {
            if (decl->type == ASTNodeType::SUBROUTINE) {
                program->subroutines.push_back(decl);
            } else {
                program->declarations.push_back(decl);
            }
        }
        
        SkipNewlines();
    }
    
    return program;
}

std::shared_ptr<ASTNode> Parser::ParseDeclaration() {
    // 变量声明
    if (Check(EYTokenType::KEYWORD_VAR)) {
        return ParseVarDecl();
    }
    
    // 常量声明
    if (Check(EYTokenType::KEYWORD_CONST)) {
        return ParseConstDecl();
    }
    
    // 子程序声明
    if (Check(EYTokenType::KEYWORD_SUB)) {
        return ParseSubroutine();
    }
    
    // 否则当作语句解析
    return ParseStatement();
}

std::shared_ptr<VarDeclNode> Parser::ParseVarDecl() {
    auto varDecl = std::make_shared<VarDeclNode>();
    varDecl->line = currentToken_.line;
    varDecl->column = currentToken_.column;
    
    // "变量" 或 "局部变量"
    Expect(EYTokenType::KEYWORD_VAR, L"期望'变量'关键字");
    
    // 变量名
    Token nameToken = Expect(EYTokenType::IDENTIFIER, L"期望变量名");
    varDecl->name = nameToken.value;
    
    // 类型（可选，易语言可以不指定类型）
    if (IsDataType(currentToken_)) {
        varDecl->dataType = currentToken_.value;
        Advance();
    }
    
    // 数组维度（可选）
    if (Match(EYTokenType::LBRACKET)) {
        varDecl->isArray = true;
        
        // 解析数组维度
        do {
            auto dimension = ParseExpression();
            if (dimension) {
                varDecl->arrayDimensions.push_back(dimension);
            }
        } while (Match(EYTokenType::COMMA));
        
        Expect(EYTokenType::RBRACKET, L"期望']'");
    }
    
    // 初始值（可选）
    if (Match(EYTokenType::OP_EQ)) {
        varDecl->initialValue = ParseExpression();
    }
    
    // 期望换行或文件结束
    if (!Check(EYTokenType::NEWLINE) && !IsAtEnd()) {
        AddError(L"变量声明后期望换行");
    }
    
    return varDecl;
}

std::shared_ptr<ConstDeclNode> Parser::ParseConstDecl() {
    auto constDecl = std::make_shared<ConstDeclNode>();
    constDecl->line = currentToken_.line;
    constDecl->column = currentToken_.column;
    
    // "常量"
    Expect(EYTokenType::KEYWORD_CONST, L"期望'常量'关键字");
    
    // 常量名
    Token nameToken = Expect(EYTokenType::IDENTIFIER, L"期望常量名");
    constDecl->name = nameToken.value;
    
    // 类型（可选）
    if (IsDataType(currentToken_)) {
        constDecl->dataType = currentToken_.value;
        Advance();
    }
    
    // 必须有初始值
    Expect(EYTokenType::OP_EQ, L"常量声明必须有初始值");
    constDecl->value = ParseExpression();
    
    if (!Check(EYTokenType::NEWLINE) && !IsAtEnd()) {
        AddError(L"常量声明后期望换行");
    }
    
    return constDecl;
}

std::shared_ptr<SubroutineNode> Parser::ParseSubroutine() {
    auto sub = std::make_shared<SubroutineNode>();
    sub->line = currentToken_.line;
    sub->column = currentToken_.column;
    
    // "子程序"
    Expect(EYTokenType::KEYWORD_SUB, L"期望'子程序'关键字");
    
    // 子程序名
    Token nameToken = Expect(EYTokenType::IDENTIFIER, L"期望子程序名");
    sub->name = nameToken.value;
    
    // 返回类型（可选）
    if (IsDataType(currentToken_)) {
        sub->returnType = currentToken_.value;
        Advance();
    }
    
    SkipNewlines();
    
    // 解析参数、局部变量和语句
    while (!IsAtEnd()) {
        if (Check(EYTokenType::KEYWORD_VAR)) {
            // 局部变量
            auto varDecl = ParseVarDecl();
            if (varDecl) {
                sub->localVars.push_back(varDecl);
            }
        } else if (Match(EYTokenType::NEWLINE)) {
            // 跳过空行
            continue;
        } else {
            // 普通语句
            auto stmt = ParseStatement();
            if (stmt) {
                sub->statements.push_back(stmt);
            }
        }
        
        // TODO: 检测子程序结束标记
        // 易语言没有明确的子程序结束标记，需要根据缩进或特定规则判断
    }
    
    return sub;
}

std::shared_ptr<ASTNode> Parser::ParseStatement() {
    // 如果语句
    if (Check(EYTokenType::KEYWORD_IF)) {
        return ParseIfStatement();
    }
    
    // 循环语句
    if (Check(EYTokenType::KEYWORD_WHILE)) {
        return ParseWhileStatement();
    }
    
    // 计次循环
    if (Check(EYTokenType::KEYWORD_FOR)) {
        return ParseForStatement();
    }
    
    // 返回语句
    if (Check(EYTokenType::KEYWORD_RETURN)) {
        return ParseReturnStatement();
    }
    
    // 跳出循环
    if (Match(EYTokenType::KEYWORD_BREAK)) {
        auto stmt = std::make_shared<ASTNode>(ASTNodeType::BREAK_STMT);
        return stmt;
    }
    
    // 继续循环
    if (Match(EYTokenType::KEYWORD_CONTINUE)) {
        auto stmt = std::make_shared<ASTNode>(ASTNodeType::CONTINUE_STMT);
        return stmt;
    }
    
    // 否则当作表达式语句或赋值语句
    auto expr = ParseExpression();
    if (!expr) {
        Synchronize();
        return nullptr;
    }
    
    // 检查是否是赋值
    if (Match(EYTokenType::OP_EQ)) {
        auto assign = std::make_shared<AssignStmtNode>();
        assign->target = expr;
        assign->value = ParseExpression();
        return assign;
    }
    
    // 普通表达式语句
    auto exprStmt = std::make_shared<ASTNode>(ASTNodeType::EXPR_STMT);
    return exprStmt;
}

std::shared_ptr<IfStmtNode> Parser::ParseIfStatement() {
    auto ifStmt = std::make_shared<IfStmtNode>();
    ifStmt->line = currentToken_.line;
    ifStmt->column = currentToken_.column;
    
    // "如果" 或 "如果真"
    Expect(EYTokenType::KEYWORD_IF, L"期望'如果'关键字");
    
    // 条件（如果有括号）
    bool hasParens = Match(EYTokenType::LPAREN);
    ifStmt->condition = ParseExpression();
    if (hasParens) {
        Expect(EYTokenType::RPAREN, L"期望')'");
    }
    
    SkipNewlines();
    
    // then块
    while (!IsAtEnd() && !Check(EYTokenType::KEYWORD_ELSE)) {
        if (Match(EYTokenType::NEWLINE)) {
            continue;
        }
        
        // TODO: 需要判断何时结束then块
        auto stmt = ParseStatement();
        if (stmt) {
            ifStmt->thenBlock.push_back(stmt);
        }
        
        // 简化处理：解析一个语句后就退出
        break;
    }
    
    // else块（可选）
    if (Match(EYTokenType::KEYWORD_ELSE)) {
        SkipNewlines();
        while (!IsAtEnd()) {
            if (Match(EYTokenType::NEWLINE)) {
                continue;
            }
            
            auto stmt = ParseStatement();
            if (stmt) {
                ifStmt->elseBlock.push_back(stmt);
            }
            
            // 简化处理
            break;
        }
    }
    
    return ifStmt;
}

std::shared_ptr<WhileStmtNode> Parser::ParseWhileStatement() {
    auto whileStmt = std::make_shared<WhileStmtNode>();
    whileStmt->line = currentToken_.line;
    whileStmt->column = currentToken_.column;
    
    // "判断循环首"
    Expect(EYTokenType::KEYWORD_WHILE, L"期望'判断循环首'关键字");
    
    // 条件
    bool hasParens = Match(EYTokenType::LPAREN);
    whileStmt->condition = ParseExpression();
    if (hasParens) {
        Expect(EYTokenType::RPAREN, L"期望')'");
    }
    
    SkipNewlines();
    
    // 循环体
    // TODO: 解析到"到循环尾"
    while (!IsAtEnd()) {
        if (Match(EYTokenType::NEWLINE)) {
            continue;
        }
        
        auto stmt = ParseStatement();
        if (stmt) {
            whileStmt->body.push_back(stmt);
        }
        
        // 简化处理
        break;
    }
    
    return whileStmt;
}

std::shared_ptr<ForStmtNode> Parser::ParseForStatement() {
    auto forStmt = std::make_shared<ForStmtNode>();
    forStmt->line = currentToken_.line;
    forStmt->column = currentToken_.column;
    
    // "计次循环首"
    Expect(EYTokenType::KEYWORD_FOR, L"期望'计次循环首'关键字");
    
    // 循环变量
    bool hasParens = Match(EYTokenType::LPAREN);
    Token loopVarToken = Expect(EYTokenType::IDENTIFIER, L"期望循环变量名");
    forStmt->loopVar = loopVarToken.value;
    
    // 起始值
    Expect(EYTokenType::COMMA, L"期望','");
    forStmt->startValue = ParseExpression();
    
    // 结束值
    Expect(EYTokenType::COMMA, L"期望','");
    forStmt->endValue = ParseExpression();
    
    // 步长（可选）
    if (Match(EYTokenType::COMMA)) {
        forStmt->stepValue = ParseExpression();
    }
    
    if (hasParens) {
        Expect(EYTokenType::RPAREN, L"期望')'");
    }
    
    SkipNewlines();
    
    // 循环体
    while (!IsAtEnd()) {
        if (Match(EYTokenType::NEWLINE)) {
            continue;
        }
        
        auto stmt = ParseStatement();
        if (stmt) {
            forStmt->body.push_back(stmt);
        }
        
        // 简化处理
        break;
    }
    
    return forStmt;
}

std::shared_ptr<ReturnStmtNode> Parser::ParseReturnStatement() {
    auto returnStmt = std::make_shared<ReturnStmtNode>();
    returnStmt->line = currentToken_.line;
    returnStmt->column = currentToken_.column;
    
    // "返回"
    Expect(EYTokenType::KEYWORD_RETURN, L"期望'返回'关键字");
    
    // 返回值（可选）
    if (!Check(EYTokenType::NEWLINE) && !IsAtEnd()) {
        bool hasParens = Match(EYTokenType::LPAREN);
        returnStmt->returnValue = ParseExpression();
        if (hasParens) {
            Expect(EYTokenType::RPAREN, L"期望')'");
        }
    }
    
    return returnStmt;
}

// ========== 表达式解析（运算符优先级） ==========

std::shared_ptr<ASTNode> Parser::ParseExpression() {
    return ParseLogicalOr();
}

std::shared_ptr<ASTNode> Parser::ParseLogicalOr() {
    auto left = ParseLogicalAnd();
    
    while (Match(EYTokenType::OP_OR)) {
        auto binary = std::make_shared<BinaryExprNode>();
        binary->op = EYTokenType::OP_OR;
        binary->left = left;
        binary->right = ParseLogicalAnd();
        left = binary;
    }
    
    return left;
}

std::shared_ptr<ASTNode> Parser::ParseLogicalAnd() {
    auto left = ParseEquality();
    
    while (Match(EYTokenType::OP_AND)) {
        auto binary = std::make_shared<BinaryExprNode>();
        binary->op = EYTokenType::OP_AND;
        binary->left = left;
        binary->right = ParseEquality();
        left = binary;
    }
    
    return left;
}

std::shared_ptr<ASTNode> Parser::ParseEquality() {
    auto left = ParseComparison();
    
    while (MatchMultiple({EYTokenType::OP_EQ, EYTokenType::OP_NEQ})) {
        EYTokenType op = previousToken_.type;
        auto binary = std::make_shared<BinaryExprNode>();
        binary->op = op;
        binary->left = left;
        binary->right = ParseComparison();
        left = binary;
    }
    
    return left;
}

std::shared_ptr<ASTNode> Parser::ParseComparison() {
    auto left = ParseAddition();
    
    while (MatchMultiple({EYTokenType::OP_GT, EYTokenType::OP_GTE, 
                          EYTokenType::OP_LT, EYTokenType::OP_LTE})) {
        EYTokenType op = previousToken_.type;
        auto binary = std::make_shared<BinaryExprNode>();
        binary->op = op;
        binary->left = left;
        binary->right = ParseAddition();
        left = binary;
    }
    
    return left;
}

std::shared_ptr<ASTNode> Parser::ParseAddition() {
    auto left = ParseMultiplication();
    
    while (MatchMultiple({EYTokenType::OP_PLUS, EYTokenType::OP_MINUS})) {
        EYTokenType op = previousToken_.type;
        auto binary = std::make_shared<BinaryExprNode>();
        binary->op = op;
        binary->left = left;
        binary->right = ParseMultiplication();
        left = binary;
    }
    
    return left;
}

std::shared_ptr<ASTNode> Parser::ParseMultiplication() {
    auto left = ParseUnary();
    
    while (MatchMultiple({EYTokenType::OP_MULTIPLY, EYTokenType::OP_DIVIDE, 
                          EYTokenType::OP_MOD, EYTokenType::OP_POWER})) {
        EYTokenType op = previousToken_.type;
        auto binary = std::make_shared<BinaryExprNode>();
        binary->op = op;
        binary->left = left;
        binary->right = ParseUnary();
        left = binary;
    }
    
    return left;
}

std::shared_ptr<ASTNode> Parser::ParseUnary() {
    if (MatchMultiple({EYTokenType::OP_NOT, EYTokenType::OP_MINUS})) {
        EYTokenType op = previousToken_.type;
        auto unary = std::make_shared<UnaryExprNode>();
        unary->op = op;
        unary->operand = ParseUnary();
        return unary;
    }
    
    return ParseCallOrAccess();
}

std::shared_ptr<ASTNode> Parser::ParsePrimary() {
    // 数字字面量
    if (Match(EYTokenType::NUMBER)) {
        auto literal = std::make_shared<LiteralExprNode>();
        literal->literalType = EYTokenType::NUMBER;
        literal->value = previousToken_.value;
        return literal;
    }
    
    // 字符串字面量
    if (Match(EYTokenType::STRING)) {
        auto literal = std::make_shared<LiteralExprNode>();
        literal->literalType = EYTokenType::STRING;
        literal->value = previousToken_.value;
        return literal;
    }
    
    // 逻辑值
    if (Match(EYTokenType::KEYWORD_TRUE)) {
        auto literal = std::make_shared<LiteralExprNode>();
        literal->literalType = EYTokenType::KEYWORD_TRUE;
        literal->value = L"真";
        return literal;
    }
    
    if (Match(EYTokenType::KEYWORD_FALSE)) {
        auto literal = std::make_shared<LiteralExprNode>();
        literal->literalType = EYTokenType::KEYWORD_FALSE;
        literal->value = L"假";
        return literal;
    }
    
    // 标识符
    if (Match(EYTokenType::IDENTIFIER)) {
        auto identifier = std::make_shared<IdentifierExprNode>();
        identifier->name = previousToken_.value;
        return identifier;
    }
    
    // 括号表达式
    if (Match(EYTokenType::LPAREN)) {
        auto expr = ParseExpression();
        Expect(EYTokenType::RPAREN, L"期望')'");
        return expr;
    }
    
    AddError(L"期望表达式");
    return nullptr;
}

std::shared_ptr<ASTNode> Parser::ParseCallOrAccess() {
    auto expr = ParsePrimary();
    if (!expr) return nullptr;
    
    while (true) {
        // 函数调用
        if (Match(EYTokenType::LPAREN)) {
            auto call = std::make_shared<CallExprNode>();
            
            // 从标识符获取函数名
            if (expr->type == ASTNodeType::IDENTIFIER_EXPR) {
                auto idExpr = std::static_pointer_cast<IdentifierExprNode>(expr);
                call->functionName = idExpr->name;
            }
            
            // 解析参数列表
            if (!Check(EYTokenType::RPAREN)) {
                do {
                    auto arg = ParseExpression();
                    if (arg) {
                        call->arguments.push_back(arg);
                    }
                } while (Match(EYTokenType::COMMA));
            }
            
            Expect(EYTokenType::RPAREN, L"期望')'");
            expr = call;
        }
        // 数组访问
        else if (Match(EYTokenType::LBRACKET)) {
            auto arrayAccess = std::make_shared<ArrayAccessNode>();
            
            // 从标识符获取数组名
            if (expr->type == ASTNodeType::IDENTIFIER_EXPR) {
                auto idExpr = std::static_pointer_cast<IdentifierExprNode>(expr);
                arrayAccess->arrayName = idExpr->name;
            }
            
            // 解析索引
            do {
                auto index = ParseExpression();
                if (index) {
                    arrayAccess->indices.push_back(index);
                }
            } while (Match(EYTokenType::COMMA));
            
            Expect(EYTokenType::RBRACKET, L"期望']'");
            expr = arrayAccess;
        }
        // 成员访问
        else if (Match(EYTokenType::DOT)) {
            // TODO: 实现成员访问
            Token member = Expect(EYTokenType::IDENTIFIER, L"期望成员名");
            // 暂时简化处理
        }
        else {
            break;
        }
    }
    
    return expr;
}
