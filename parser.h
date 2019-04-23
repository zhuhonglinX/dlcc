//
// Created by zhu honglin on 2019-04-21.
//

#ifndef MYCOMPILER_PARSER_H
#define MYCOMPILER_PARSER_H

#include <iostream>
#include <vector>
#include <stack>
#include "dlcc.h"
#include "ast.h"
using namespace std;

class Parser {
    int pos_ = 0;
    pair<Token, string> cur_token_;         // 当前扫描到的 token
public:
    pair<Token, string> &get_next_token();
    void preprocess();  // 对tokens进行预处理
    bool parse();
    AstNode *parse_statement();
    AstNode *parse_number();
    AstNode *parse_string();
    AstNode *parse_identifier();
    AstNode *parse_paren_exp();
    AstNode *parse_expression();
    AstNode *parse_block();
    AstNode *parse_if_exp();
    AstNode *parse_while_exp();
    AstNode *parse_dec();
    AstNode *parse_func_dec();
    AstNode *parse_var_dec();
    AstNode *parse_func_ret();
    // parse expression, deal stack
    AstNode *deal_exp_stack(stack<AstNode *> &ast_stack, stack<Token> &op_stack);

    void parse_debug();
};


#endif //MYCOMPILER_PARSER_H
