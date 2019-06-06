//
// Created by zhu honglin on 2019-04-22.
//

#include "codegen.h"

#define DEBUG

int CodeGen::jmp_cnt = 0;

string CodeGen::get_jmp_label() { return "L" + to_string(jmp_cnt++); }

void CodeGen::gen_asm_code() {
#ifdef DEBUG
    printf("gen_asm_code\n");
#endif

    jmp_cnt = 0;
    string s_data = ".data\n";
    string s_bss = ".bss\n";
    string s_text = ".text\n.globl main\n";

    // 处理 data 段信息
    int cnt = 0;
    for (auto &str : str_data) {
        s_data += "    str_";
        s_data += to_string(cnt);
        s_data += ": .asciz \"";
        s_data += str;
        s_data += "\"\n";
        cnt++;
    }

    // 处理 bss 段信息
    for (auto &var : var_symbol) {
        s_bss += "    .lcomm ";
        s_bss += var.first;
        s_bss += ", 4\n";
    }

    // 处理 text 段信息
    for (auto ast : ast_list) {
        s_text += ast_to_asm(ast);
    }
    asm_code += s_data;
    asm_code += s_bss;
    asm_code += s_text;
}

string CodeGen::ast_to_asm(AstNode *node) {
#ifdef DEBUG
    printf("ast => asmcode: ");
    print_ast_type(node->get_ast_type());
#endif
    string rst;
    switch (node->get_ast_type()) {
    case AstType::FUNC: {
        auto *func = dynamic_cast<FuncAst *>(node);
        if (func->name_ == "main") {
            rst += "main:\nfinit\n";
        }
        rst += ast_to_asm(func->block_ast_);
        break;
    }
    case AstType::CALLFUNC: {
        auto *call = dynamic_cast<CallFuncAst *>(node);
        // 逆序
        for (int i = call->args_.size() - 1; i >= 0; i--) {
            rst += ast_to_asm(call->args_[i]);
        }
        rst += "call " + call->fname_ + "\n";
        // 退栈
        int pop_cnt = 4 * call->args_.size();
        rst += "addl $" + to_string(pop_cnt) + ", %esp\n";
        break;
    }
    case AstType::VARDEC: {
        break;
    }
    case AstType::BINEXP: {
        auto *bin_exp = dynamic_cast<BinExpAst *>(node);
        rst += ast_to_asm(bin_exp->lt_);
        rst += ast_to_asm(bin_exp->rt_);
        rst += "popl %ebx\npopl %eax\n";

        switch (bin_exp->op_) {
        case Token::ASSIGN: {
            auto *var = dynamic_cast<VarAst *>(bin_exp->lt_);
            rst += "movl %ebx, " + var->name_ + to_string(var->env_->scope_id_) + "\n";
            break;
        }
        case Token::ADD: {
            rst += "addl %ebx, %eax\npushl %eax\n";
            break;
        }
        case Token::SUB: {
            rst += "subl %ebx, %eax\npushl %eax\n";
            break;
        }
        case Token::MUL: {
            rst += "imull %ebx, %eax\npushl %eax\n";
            break;
        }
        case Token::GT: { // >
            rst += "cmp %ebx, %eax\n"
                   "setg %al\n"
                   "andl $0x000000ff, %eax\n"
                   "pushl %eax\n";
            break;
        }
        case Token::GE: { // >=
            rst += "cmp %ebx, %eax\n"
                   "setge %al\n"
                   "andl $0x000000ff, %eax\n"
                   "pushl %eax\n";
            break;
        }
        case Token::LT: { // <
            rst += "cmp %ebx, %eax\n"
                   "setl %al\n"
                   "andl $0x000000ff, %eax\n"
                   "pushl %eax\n";
            break;
        }
        case Token::LE: { // <=
            rst += "cmp %ebx, %eax\n"
                   "setle %al\n"
                   "andl $0x000000ff, %eax\n"
                   "pushl %eax\n";
            break;
        }
        case Token::LAND: { // &&
            rst += "cmp $0, %eax\n"
                   "setne %al\n"
                   "andl $0x000000ff, %ebx\n"
                   "cmp $0, %ebx\n"
                   "setne %bl\n"
                   "andl $0x000000ff, %ebx\n"
                   "andl %ebx, %eax\n"
                   "pushl %eax\n";
            break;
        }
        default: {
            cout << "op:" << kw_map[bin_exp->op_] << " not support yet" << endl;
            exit(0);
        }
        }
        break;
    }
    case AstType::IfExp: {
        auto *if_exp = dynamic_cast<IfExpAst *>(node);
        rst += ast_to_asm(if_exp->cond_);
        string label = get_jmp_label();
        rst += "popl %eax\n"
               "cmpl $0, %eax\n"
               "je " +
               label + "\n";
        rst += ast_to_asm(if_exp->if_block_);
        if (if_exp->else_block_) {
            string label_2 = get_jmp_label();
            // 如果执行 if block , 则跳到 else block 后面
            rst += "jmp " + label_2 + "\n";
            rst += label + ":\n";
            rst += ast_to_asm(if_exp->else_block_);
            rst += label_2 + ":\n";
        } else {
            rst += label + ":\n";
        }
        break;
    }
    case AstType::WhileExp: {
        auto *while_exp = dynamic_cast<WhileExpAst *>(node);
        string label = get_jmp_label();
        rst += label + ":\n";
        rst += ast_to_asm(while_exp->cond_);
        string label_2 = get_jmp_label();
        rst += "popl %eax\ncmpl $0, %eax\nje " + label_2 + "\n";
        rst += ast_to_asm(while_exp->block_);
        rst += "jmp " + label + "\n";
        rst += label_2 + ":\n";
    }
    case AstType::BLOCK: {
        auto *block = dynamic_cast<BlockAst *>(node);
        for (auto stat : block->stats_) {
            rst += ast_to_asm(stat);
        }
        break;
    }
    case AstType::NUM: {
        auto *num = dynamic_cast<NumAst *>(node);
        rst += "pushl $" + num->value_ + "\n";
        break;
    }
    case AstType::VAR: {
        auto *var = dynamic_cast<VarAst *>(node);
        string name = var->name_ + to_string(var->env_->scope_id_);
        rst += "pushl " + name + "\n";
        break;
    }
    case AstType::STR: {
        auto *ast = dynamic_cast<StrAst *>(node);
        string name = "str_" + to_string(ast->data_offset_);
        rst += "pushl $" + name + "\n";
        break;
    }
    case AstType::RET: {
        auto *ret = dynamic_cast<ReturnAst *>(node);
        rst += ast_to_asm(ret->ret_value_);
        rst += "call exit\n";
        break;
    }
    default: {
        printf("ast to asm code error: ");
        print_ast_type(node->get_ast_type());
        exit(0);
    }
    }
    return rst;
}

void CodeGen::gen_vm_code() {
#ifdef DEBUG
    printf("gen_vm_code\n");
#endif
    jmp_cnt = 0;
    for (auto ast : ast_list) {
        ast_to_vm(ast);
    }
}

void CodeGen::ast_to_vm(AstNode *node) {
#ifdef DEBUG
    printf("ast => vmcode: ");
    print_ast_type(node->get_ast_type());
#endif
    switch (node->get_ast_type()) {
    case AstType::FUNC: {
        auto *func = dynamic_cast<FuncAst *>(node);
        // Ins: FUNC <func name>:
        vm_code.emplace_back(make_pair(Ins::FUNC, func->name_));

        // 载入参数 顺序
        for (auto &arg : func->args_) {
            // Ins: INT <var>
            // Ins: LOAD <var>
            vm_code.emplace_back(make_pair(Ins::INT, arg.second));
            vm_code.emplace_back(make_pair(Ins::MOV, arg.second));
        }
        ast_to_vm(func->block_ast_);
        break;
    }
    case AstType::CALLFUNC: {
        auto call = dynamic_cast<CallFuncAst *>(node);
        // 参数逆序入栈
        for (int i = call->args_.size() - 1; i >= 0; i--) {
            ast_to_vm(call->args_[i]);
        }
        // Ins: PRINT <args num>
        if (call->fname_ == "printf") {
            vm_code.emplace_back(
                make_pair(Ins::PRINT, to_string(call->args_.size())));
            break;
        }
        // Ins: CALL <func id>
        vm_code.emplace_back(make_pair(Ins::CALL, call->fname_));
        break;
    }
    case AstType::BLOCK: {
        auto *block = dynamic_cast<BlockAst *>(node);
        for (auto stat : block->stats_) {
            ast_to_vm(stat);
        }
        break;
    }
    case AstType::NUM: {
        auto *num = dynamic_cast<NumAst *>(node);
        // Ins: PUSHL <num>
        vm_code.emplace_back(make_pair(Ins::PUSHL, num->value_));
        break;
    }
    case AstType::VARDEC: {
        break;
    }
    case AstType::VAR: {
        auto *var = dynamic_cast<VarAst *>(node);
        // Ins: STORE <var>
        vm_code.emplace_back(make_pair(Ins::PUSHV, var->name_));
        break;
    }
    case AstType::STR: {
        auto *ast = dynamic_cast<StrAst *>(node);
        // Ins: PUSHS <str>
        vm_code.emplace_back(
            make_pair(Ins::PUSHS, to_string(ast->data_offset_)));
        break;
    }
    case AstType::RET: {
        auto *ret = dynamic_cast<ReturnAst *>(node);
        ast_to_vm(ret->ret_value_);
        // Ins: RET
        vm_code.emplace_back(make_pair(Ins::RET, ""));
        break;
    }
    case AstType::BINEXP: {
        auto *bin = dynamic_cast<BinExpAst *>(node);
        if (bin->op_ != Token::ASSIGN) {
            ast_to_vm(bin->lt_);
        }
        ast_to_vm(bin->rt_);
        switch (bin->op_) {
        case Token::ASSIGN: {
            auto *var = dynamic_cast<VarAst *>(bin->lt_);
            vm_code.emplace_back(make_pair(Ins::MOV, var->name_));
            break;
        }
        case Token::ADD: {
            vm_code.emplace_back(make_pair(Ins::ADD, ""));
            break;
        }
        case Token::SUB: {
            vm_code.emplace_back(make_pair(Ins::SUB, ""));
            break;
        }
        case Token::MUL: {
            vm_code.emplace_back(make_pair(Ins::MUL, ""));
            break;
        }
        case Token::DIV: {
            vm_code.emplace_back(make_pair(Ins::DIV, ""));
            break;
        }
        case Token::MOD: {
            vm_code.emplace_back(make_pair(Ins::MOD, ""));
            break;
        }
        case Token::GT: { // >
            vm_code.emplace_back(make_pair(Ins::GT, ""));
            break;
        }
        case Token::GE: { // >=
            vm_code.emplace_back(make_pair(Ins::GE, ""));
            break;
        }
        case Token::LT: { // <
            vm_code.emplace_back(make_pair(Ins::LT, ""));
            break;
        }
        case Token::LE: { // <=
            vm_code.emplace_back(make_pair(Ins::LE, ""));
            break;
        }
        case Token::EQ: { // <=
            vm_code.emplace_back(make_pair(Ins::EQ, ""));
            break;
        }
        case Token::LAND: {
            vm_code.emplace_back(make_pair(Ins::LAND, ""));
            break;
        }
        case Token::LOR: {
            vm_code.emplace_back(make_pair(Ins::LOR, ""));
            break;
        }
        default: {
            cout << "op:" << kw_map[bin->op_] << " not support yet" << endl;
            exit(0);
        }
        }
        break;
    }
    case AstType::IfExp: {
        auto *if_exp = dynamic_cast<IfExpAst *>(node);
        ast_to_vm(if_exp->cond_);
        string label = get_jmp_label();
        // Ins: JE L0
        vm_code.emplace_back(make_pair(Ins::JE, label));

        ast_to_vm(if_exp->if_block_);
        if (if_exp->else_block_) {
            string label_2 = get_jmp_label();
            // 如果执行 if block , 则跳到 else block 后面
            // Ins: JMP L1
            vm_code.emplace_back(make_pair(Ins::JMP, label_2));
            // 否则执行 else block
            vm_code.emplace_back(make_pair(Ins::LAB, label));
            ast_to_vm(if_exp->else_block_);
            vm_code.emplace_back(make_pair(Ins::LAB, label_2));
        } else {
            vm_code.emplace_back(make_pair(Ins::LAB, label));
        }
        break;
    }
    case AstType::WhileExp: {
        auto *while_exp = dynamic_cast<WhileExpAst *>(node);
        string label = get_jmp_label(); // 重回 while 的标签
        // Ins: LAB L1
        vm_code.emplace_back(make_pair(Ins::LAB, label));
        ast_to_vm(while_exp->cond_);
        // Ins: jE L2
        string label_2 = get_jmp_label(); // 跳出 while 的标签
        vm_code.emplace_back(make_pair(Ins::JE, label_2));

        ast_to_vm(while_exp->block_);
        // Ins: JMP L1
        // Ins: LAB L2
        vm_code.emplace_back(make_pair(Ins::JMP, label));
        vm_code.emplace_back(make_pair(Ins::LAB, label_2));
        break;
    }
    default: {
        printf("ast to vm code error: ");
        print_ast_type(node->get_ast_type());
        exit(0);
    }
    }
}

