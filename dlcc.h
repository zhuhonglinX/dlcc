//
// Created by zhu honglin on 2019-04-21.
//
#ifndef MYCOMPILER_DLCC_H
#define MYCOMPILER_DLCC_H

#include <string>
#include <map>
#include <vector>


using namespace std;

// 前置声明 AstNode
class AstNode;

class VarEnv;

enum class Token : int {
    IDENTIFIER,
    NUMBER,
    KW_IF,                          // if
    KW_ELSE,                        // else
    KW_WHILE,                       // while
    KW_RET,                         // return
    KW_INT,                         // int
    STRING,                         // string
    ADD, SUB, MUL, DIV, MOD,        // +, -, *, /, %
    AND, OR, XOR,                   // & | ^
    GT, GE, LT, LE, EQ, NEQ,        // >, >=, <, <=, ==, !=
    LAND, LOR,                      // &&, ||
    LPAREN, RPAREN,                 // ( )
    LBK, RBK,                       // [ ]
    LBRACE, RBRACE,                 // { }
    ASSIGN,                         // =
    COMMA, COLON, SEMICON,          // , : ;
    COMMENT,                        // "//"
    END_OF_FILE = -1,
    ERR = -2
};

// 语法数类型
enum class AstType : int {
    NUM,
    STR,
    VAR,
    VARDEC,
    BINEXP,
    IfExp,
    WhileExp,
    BLOCK,
    STATE,
    CALLFUNC,
    RET,
    FUNC,
};

// 虚拟指令
enum class Ins : int {
    PUSHL,  // int 立即数压栈
    PUSHS,  // str 常量压栈
    PUSHV,  // var 变量压栈(int)
    INT,
    MOV,    // 变量出栈，存储(int)
    LAB,    // label L0:
    FUNC,
    CALL,
    PRINT,  // printf 函数 %d
    RET,
    JE, JMP,
    ADD, SUB, MUL, DIV, MOD,
    GT, GE, LT, LE,
    AND, OR, XOR,               // & | ^
    LAND, LOR,                  // && ||
};

extern string content;                          // 源文件
extern vector<pair<Token, string>> tokens;      // tokens stream
extern vector<AstNode *> ast_list;              // 语法树
extern VarEnv *top_env;                         // 顶级作用域
extern vector<string> str_data;                 // 字符串常量
extern map<string, int> var_symbol;    // var 符号表，记录 int
extern vector<string> func_symbol;              // 函数符号表
//extern map<string, int> var_tbl;      // 记录 var 在符号表的位置
//extern map<string, int> func_tbl;     // 记录 func 在符号表的位置
extern string asm_code;                         // 汇编代码
extern vector<pair<Ins, string>> vm_code;                     // 虚拟指令
extern map<Token, int> op_precedence;           // 符号优先级判定
extern map<Token, string> kw_map;               // kw -> str
extern map<string, Ins> ins_map;                // str -> Ins

extern void print_ast_type(AstType type);       // 打印 AstType
extern void print_ins(Ins ins);                 // 打印 Ins
#endif //MYCOMPILER_DLCC_H
