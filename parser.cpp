//
// Created by zhu honglin on 2019-04-21.
//

#include "parser.h"

#define DEBUG

pair<Token, string> &Parser::get_next_token() {
    return cur_token_ = tokens[pos_++];
}

void Parser::parse_debug() {
    for (auto ast : ast_list) {
        ast->print();
    }
}

void Parser::preprocess() {
#ifdef DEBUG
    cout << "preprocess" << endl;
#endif
    vector<pair<Token, string>> rst_tokens;
    get_next_token();
    while (cur_token_.first != Token::END_OF_FILE) {
        if (cur_token_.first == Token::KW_INT &&
            tokens[pos_ + 1].first != Token::LPAREN) {
            // 变量声明
            // int a, b;
            // int a = 10, b = 10;
            // int a;
            pair<Token, string> type = cur_token_;
            bool end_stat = false;
            vector<pair<Token, string>> tmp_tokens;
            get_next_token(); // eat 'int'
            while (true) {
                rst_tokens.emplace_back(type);       // push type
                rst_tokens.emplace_back(cur_token_); // push id
                rst_tokens.emplace_back(make_pair(Token::SEMICON, ";"));

                if (tokens[pos_].first == Token::ASSIGN) {
                    tmp_tokens.emplace_back(cur_token_); // push id
                    get_next_token();                    // eat id
                    tmp_tokens.emplace_back(cur_token_); // push '='
                    get_next_token();                    // eat '='
                    while (true) {
                        if (cur_token_.first == Token::SEMICON) {
                            end_stat = true;
                            break;
                        }
                        if (cur_token_.first == Token::COMMA) {
                            break;
                        }
                        tmp_tokens.emplace_back(cur_token_);
                        get_next_token();
                    }
                    tmp_tokens.emplace_back(make_pair(Token::SEMICON, ";"));
                } else if (tokens[pos_].first == Token::SEMICON) {
                    get_next_token(); // eat id
                    end_stat = true;
                } else if (tokens[pos_].first == Token::COMMA) {
                    get_next_token(); // eat id
                }
                get_next_token(); // eat ";" or ","
                if (end_stat)
                    break;
            }
            // stat end
            rst_tokens.insert(rst_tokens.end(), tmp_tokens.begin(),
                              tmp_tokens.end());
        } else {
            rst_tokens.emplace_back(cur_token_);
            get_next_token();
        }
    }
    rst_tokens.emplace_back(make_pair(Token::END_OF_FILE, "\0"));
    tokens = rst_tokens;
#ifdef DEBUG
    cout << "========= preprocess ========" << endl;
    for (auto it = tokens.begin(); it != tokens.end(); it++) {
        cout << "type: " << (int)it->first << ";\tvalue: " << it->second
             << endl;
    }
    cout << "=============================" << endl;
#endif
    pos_ = 0;
    cur_token_ = tokens[pos_];
}

bool Parser::parse() {
#ifdef DEBUG
    cout << "parse: " << cur_token_.second << endl;
#endif
    get_next_token(); // start parsing
    AstNode *stat = nullptr;
    while (cur_token_.first != Token::END_OF_FILE) {
        stat = parse_statement();
        if (stat) {
            ast_list.emplace_back(stat);
        }
    }
    return true;
}

AstNode *Parser::parse_number() {
#ifdef DEBUG
    cout << "parse num: " << cur_token_.second << endl;
#endif
    // AstNode *result = new NumberAst(strtod(cur_token_.second.c_str(),
    // nullptr));
    AstNode *rst = new NumAst(cur_token_.second);
    get_next_token();
    return rst;
}

AstNode *Parser::parse_string() {
#ifdef DEBUG
    cout << "parse str: " << cur_token_.second << endl;
#endif
    AstNode *rst = new StrAst(cur_token_.second);
    get_next_token();
    return rst;
}

AstNode *Parser::parse_statement() {
#ifdef DEBUG
    cout << "parse statement: " << cur_token_.second << endl;
#endif
    switch (cur_token_.first) {
    case Token::KW_IF: {
        return parse_if_exp();
    }
    case Token::KW_WHILE: {
        return parse_while_exp();
    }
    case Token::KW_INT: {
        return parse_dec();
    }
    case Token::KW_RET: {
        return parse_func_ret();
    }
    default: { return parse_expression(); }
    }
}

AstNode *Parser::parse_dec() {
#ifdef DEBUG
    cout << "parseDec: " << cur_token_.second << endl;
#endif
    Token aheadToken = tokens[pos_ + 1].first;
    // function declaration
    if (aheadToken == Token::LPAREN) {
        return parse_func_dec();
    }
    // variable declaration
    else {
        return parse_var_dec();
    }
}

AstNode *Parser::parse_func_dec() {
#ifdef DEBUG
    cout << "parseFuncDec: " << cur_token_.second << endl;
#endif
    Token type = cur_token_.first;
    get_next_token(); // eat function return value type
    string name = cur_token_.second;
    get_next_token(); // eat function name
    vector<pair<Token, string>> args;

    if (tokens[pos_].first != Token::RPAREN) {
        // if function has argument
        do {
            get_next_token(); // eat '(' or ','
            Token varType = cur_token_.first;
            get_next_token(); // eat type name
            string varName = cur_token_.second;
            get_next_token(); // eat arg name
            args.emplace_back(make_pair(varType, varName));
        } while (cur_token_.first != Token::RPAREN);
    } else {
        // else function has no argument
        get_next_token(); // eat '('
    }
    get_next_token(); // eat ')'
    AstNode *block = parse_block();
    return new FuncAst(type, name, args, block);
}

AstNode *Parser::parse_var_dec() {
#ifdef DEBUG
    cout << "parseVarDec: " << cur_token_.second << endl;
#endif
    Token type = cur_token_.first;
    get_next_token(); // eat type name
    string name = cur_token_.second;
    get_next_token();
    return new VarDecAst(type, name);
}

AstNode *Parser::parse_block() {
#ifdef DEBUG
    cout << "parse_block: " << cur_token_.second << endl;
#endif
    get_next_token(); // eat '{'
    vector<AstNode *> stats;
    while (cur_token_.first != Token::RBRACE) {
        AstNode *newStat = parse_statement();
        if (newStat) {
            stats.emplace_back(newStat);
        }
    }
    AstNode *block = new BlockAst(stats);
    get_next_token(); // eat '}'
    return block;
}

AstNode *Parser::parse_if_exp() {
#ifdef DEBUG
    cout << "parse if exp: " << cur_token_.second << endl;
#endif
    get_next_token(); // eat "if"
    AstNode *cond = parse_paren_exp();
    AstNode *if_block = parse_block();
    AstNode *else_block = nullptr;
    if (cur_token_.first == Token::KW_ELSE) {
        // not support "else if" yet
        get_next_token(); // eat "else"
        else_block = parse_block();
    }
    return new IfExpAst(cond, if_block, else_block);
}

AstNode *Parser::parse_while_exp() {
#ifdef DEBUG
    cout << "parse if exp: " << cur_token_.second << endl;
#endif
    get_next_token(); // eat "while"
    AstNode *cond = parse_paren_exp();
    AstNode *block = parse_block();
    return new WhileExpAst(cond, block);
    ;
}

AstNode *Parser::parse_paren_exp() {
#ifdef DEBUG
    cout << "parse_paren_exp: " << cur_token_.second << endl;
#endif
    get_next_token(); // eat '('
    AstNode *exp = parse_expression();
    if (cur_token_.first != Token::RPAREN) {
        cout << "expected ')'" << endl;
    }
    get_next_token(); // eat ')'
    return exp;
}

AstNode *Parser::parse_expression() {
#ifdef DEBUG
    cout << "parse_expressionn: " << cur_token_.second << endl;
#endif
    stack<AstNode *> ast_stack;
    stack<Token> op_stack;
    while (true) {
        Token tmp = cur_token_.first;
        switch (tmp) {
        case Token::NUMBER: {
            ast_stack.push(parse_number());
            break;
        }
            // func call or var
        case Token::IDENTIFIER: {
            ast_stack.push(parse_identifier());
            break;
        }
        case Token::STRING: {
            ast_stack.push(parse_string());
            break;
        }
            // only func call -> f(a, b, c)
        case Token::COMMA: {
            get_next_token(); // eat ,
            return deal_exp_stack(ast_stack, op_stack);
        }
        case Token::SEMICON: {
            get_next_token(); // eat ';'
            return deal_exp_stack(ast_stack, op_stack);
        }
            // '('
        case Token::LPAREN: {
            ast_stack.push(parse_paren_exp());
            break;
        }
            // ')'
        case Token::RPAREN: {
            return deal_exp_stack(ast_stack, op_stack);
        }
            // op
        default: {
            cout << "op: " << kw_map[tmp] << endl;
            if (!op_stack.empty() &&
                (op_precedence[op_stack.top()] >= op_precedence[tmp])) {
                Token op = op_stack.top();
                op_stack.pop();
                AstNode *left, *right;
                right = ast_stack.top();
                ast_stack.pop();
                left = ast_stack.top();
                ast_stack.pop();
                BinExpAst *ast = new BinExpAst(op, left, right);
                ast_stack.push(ast);
            }
            op_stack.push(tmp);
            get_next_token();
            break;
        }
        } // switch
    }     // while
}

AstNode *Parser::parse_identifier() {
#ifdef DEBUG
    cout << "parse_id: " << cur_token_.second << endl;
#endif
    string identifier = cur_token_.second;
    get_next_token();
    // variable name
    if (cur_token_.first != Token::LPAREN) {
        return new VarAst(identifier);
    }
    // function call
    get_next_token(); // eat '('
    vector<AstNode *> args;
    while (cur_token_.first != Token::RPAREN) { // match token ")"
        AstNode *arg = parse_expression();
        if (arg) {
            args.emplace_back(arg);
        }
    }
    get_next_token();
    return new CallFuncAst(identifier, args);
}

AstNode *Parser::parse_func_ret() {
#ifdef DEBUG
    cout << "parseRet: " << cur_token_.second << endl;
#endif
    get_next_token(); // eat "return"
    AstNode *retExp = parse_expression();
    if (!retExp) {
        retExp = new NumAst("0");
    }
    return new ReturnAst(retExp);
}

AstNode *Parser::deal_exp_stack(stack<AstNode *> &ast_stack,
                                stack<Token> &op_stack) {
    while (!op_stack.empty()) {
        Token op = op_stack.top();
        op_stack.pop();
        AstNode *left, *right;
        right = ast_stack.top();
        ast_stack.pop();
        left = ast_stack.top();
        ast_stack.pop();
        BinExpAst *Ast = new BinExpAst(op, left, right);
        ast_stack.push(Ast);
    }
    if (!ast_stack.empty())
        return ast_stack.top();
    else
        return nullptr;
}
