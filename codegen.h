//
// Created by zhu honglin on 2019-04-22.
//

#ifndef MYCOMPILER_CODEGEN_H
#define MYCOMPILER_CODEGEN_H

#include "ast.h"
#include "dlcc.h"
#include <iostream>
#include <string>
#include <vector>
using namespace std;

class CodeGen {
  public:
    static int jmp_cnt;
    static string get_jmp_label();

    // 生成汇编代码
    static void gen_asm_code();
    static string ast_to_asm(AstNode *node);

    // 生成虚拟指令
    static void gen_vm_code();
    static void ast_to_vm(AstNode *node);
};

#endif // MYCOMPILER_CODEGEN_H
