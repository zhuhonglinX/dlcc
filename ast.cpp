//
// Created by zhu honglin on 2019-04-21.
//

#include "ast.h"

BinExpAst::~BinExpAst() {
    delete lt_;
    delete rt_;
}

IfExpAst::~IfExpAst() {
    delete cond_;
    delete if_block_;
    delete else_block_;
}

BlockAst::~BlockAst() {
    for (auto stat : stats_) {
        delete stat;
    }
}

CallFuncAst::~CallFuncAst() {
    for (auto arg : args_) {
        delete arg;
    }
}

void NumAst::print() {
    cout << value_ << "(Num) ";
}

void StrAst::print() {
    cout << value_ << "(Str) ";
}

void VarAst::print() {
    cout << name_ << "(Var) ";
}

void VarDecAst::print() {
    cout << "(def " << kw_map[type_] << " " << name_ << ") ";
}

void BinExpAst::print() {
    cout << "(" << kw_map[op_] << " ";
    lt_->print();
    rt_->print();
    cout << ") ";
}

void IfExpAst::print() {
    cout << "(if ";
    cond_->print();
    if_block_->print();
    if (else_block_) else_block_->print();
}
void BlockAst::print() {
    cout << "block :" << endl;
    for (auto stat : stats_) {
        stat->print();
    }
}
void CallFuncAst::print() {
    cout << "(call func " << fname_ << " ";
    for (auto arg : args_) {
        arg->print();
    }
    cout << ")";
}

void ReturnAst::print() {
    cout << "(ret ";
    ret_value_->print();
    cout << ")" << endl;
}

void FuncAst::print() {
    cout << "(def func: " << kw_map[type_] << " " << name_ << endl;
    cout << "args: ";
    for (auto &arg : args_) {
        cout << kw_map[arg.first] << " " << arg.second << ", ";
    }
    block_ast_->print();
    cout << ")"<< endl;
}
