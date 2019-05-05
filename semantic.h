//
// Created by zhu honglin on 2019-04-21.
//

#ifndef MYCOMPILER_SEMANTIC_H
#define MYCOMPILER_SEMANTIC_H

#include "ast.h"
#include "env.h"
#include <iostream>
#include <string>
#include <tuple>
#include <vector>

using namespace std;

class Semantic {
  public:
    // 生成符号表信息
    static void traverse_ast();
    static void gen_symbol(AstNode *node, VarEnv *cur_env);

    static void symbol_debug();
    static void print_symbol_info(VarEnv *e);
};

#endif // MYCOMPILER_SEMANTIC_H
