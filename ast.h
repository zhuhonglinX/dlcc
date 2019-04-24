//
// Created by zhu honglin on 2019-04-21.
//

#ifndef MYCOMPILER_AST_H
#define MYCOMPILER_AST_H

#include <vector>
#include "dlcc.h"
#include "env.h"

using namespace std;

class AstNode {
public:
    virtual ~AstNode() {};
    virtual void print() {}
    virtual AstType get_ast_type() = 0;
};

class NumAst : public AstNode {
public:
    string value_;
    VarEnv *env_ = nullptr;

    explicit NumAst(const string &value) : value_(value) {}

    AstType get_ast_type() override { return AstType::NUM; }
    void print() override;

};

class StrAst : public AstNode {
public:
    string value_;
    VarEnv *env_ = nullptr;
    int data_offset_;  // 表示 str 存储的地址(str_data的偏移)

    explicit StrAst(const string &value) : value_(value) {}

    AstType get_ast_type() override { return AstType::STR; }

    void print() override;

};

class VarAst : public AstNode {
public:
    string name_;
    VarEnv *env_ = nullptr;

    explicit VarAst(const string &name) : name_(name) {}

    AstType get_ast_type() override { return AstType::VAR; }

    void print() override;
};

// "int a"
class VarDecAst : public AstNode {
public:
    string name_;
    Token type_;
    VarEnv *env_ = nullptr;

    explicit VarDecAst(Token type, string &name) : type_(type), name_(name) {}

    AstType get_ast_type() override { return AstType::VARDEC; }
    void print() override;
};

class BinExpAst : public AstNode {
public:
    Token op_;
    AstNode *lt_, *rt_;
    VarEnv *env_ = nullptr;


    explicit BinExpAst(Token op, AstNode *lt, AstNode *rt) : op_(op), lt_(lt), rt_(rt) {}

    ~BinExpAst() override;

    AstType get_ast_type() override { return AstType::BINEXP; }
    void print() override;
};

class IfExpAst : public AstNode {
public:
    AstNode *cond_, *if_block_, *else_block_;
    VarEnv *env_ = nullptr;


    explicit IfExpAst(AstNode *cond, AstNode *if_block, AstNode *else_block)
            : cond_(cond), if_block_(if_block), else_block_(else_block) {}

    ~IfExpAst() override ;

    AstType get_ast_type() override { return AstType::IfExp; }
    void print() override;
};

class WhileExpAst : public AstNode {
public:
    AstNode *cond_, *block_;
    VarEnv *env_ = nullptr;

    explicit WhileExpAst(AstNode *cond, AstNode *block) : cond_(cond), block_(block) {}
    ~WhileExpAst() override  {delete cond_; delete block_;}

    AstType get_ast_type() override { return AstType::WhileExp; }

};

class BlockAst : public AstNode {
public:
    vector<AstNode *> stats_;
    VarEnv *env_ = nullptr;

    explicit BlockAst(vector<AstNode *> &stat) : stats_(stat) {}

    ~BlockAst() override;

    AstType get_ast_type() override { return AstType::BLOCK; }
    void print() override;
};

class CallFuncAst : public AstNode {
public:
    string fname_;
    vector<AstNode *> args_;
    VarEnv *env_ = nullptr;


    explicit CallFuncAst(string &fname, vector<AstNode *> &args) : fname_(fname), args_(args) {}

    ~CallFuncAst() override;

    AstType get_ast_type() override { return AstType::CALLFUNC; }
    void print() override;

};

class ReturnAst : public AstNode {
public:
    AstNode *ret_value_;
    VarEnv *env_ = nullptr;

    explicit ReturnAst(AstNode *value) : ret_value_(value) {}

    ~ReturnAst() override { delete ret_value_; }

    AstType get_ast_type() override { return AstType::RET; }
    void print() override;
};

class FuncAst : public AstNode {
public:
    Token type_;
    string name_;
    vector<pair<Token, string>> args_;
    AstNode *block_ast_;
    VarEnv *env_ = nullptr;


    explicit FuncAst(Token type, string &name, vector<pair<Token, string>> &args, AstNode *block)
            : type_(type), name_(name), args_(args), block_ast_(block) {}

    ~FuncAst() override { delete block_ast_; }

    AstType get_ast_type() override { return AstType::FUNC; }
    void print() override;

};

#endif //MYCOMPILER_AST_H
