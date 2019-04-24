#include <iostream>
#include <fstream>
#include "dlcc.h"
#include "lex.h"
#include "parser.h"
#include "semantic.h"
#include "ast.h"
#include "env.h"
#include "codegen.h"
#include "vm.h"

using namespace std;

string content;
vector<pair<Token, string>> tokens;
vector<AstNode *> ast_list;
VarEnv *top_env = new VarEnv();
vector<string> str_data;
map<string, int> var_symbol;
vector<string> func_symbol;
string asm_code;
vector<pair<Ins, string>> vm_code;

map<Token, int> op_precedence = {
        {Token::ASSIGN, 0},
        {Token::LOR,    5},
        {Token::LAND,   6},
        {Token::EQ,     10},
        {Token::NEQ,    10},
        {Token::LT,     15},
        {Token::LE,     15},
        {Token::GE,     15},
        {Token::GT,     15},
        {Token::ADD,    20},
        {Token::SUB,    20},
        {Token::MUL,    21},
        {Token::DIV,    21},
        {Token::MOD,    21},
};

map<Token, string> kw_map = {
        {Token::EQ,     "=="},
        {Token::NEQ,    "!="},
        {Token::LOR,    "||"},
        {Token::LAND,   "&&"},
        {Token::LE,     "<="},
        {Token::LT,     "<"},
        {Token::GE,     ">="},
        {Token::GT,     ">"},
        {Token::ADD,    "+"},
        {Token::SUB,    "-"},
        {Token::MUL,    "*"},
        {Token::DIV,    "/"},
        {Token::MOD,    "%%"},
        {Token::ASSIGN, "="},
        {Token::KW_INT, "int"},
        {Token::RPAREN, ")"},
        {Token::RBRACE, "}"},
};

void print_ast_type(AstType type) {
    switch (type) {
        case AstType::FUNC: printf("Func Ast\n"); break;
        case AstType::NUM: printf("Num Ast\n"); break;
        case AstType::STR: printf("Str Ast\n"); break;
        case AstType::VAR: printf("Var Ast\n"); break;
        case AstType::VARDEC: printf("VarDec Ast\n"); break;
        case AstType::BINEXP: printf("BinExp Ast\n"); break;
        case AstType::IfExp: printf("IfExp Ast\n"); break;
        case AstType::WhileExp: printf("WhileExp Ast\n"); break;
        case AstType::BLOCK: printf("Block Ast\n"); break;
        case AstType::CALLFUNC: printf("CallFunc Ast\n"); break;
        case AstType::STATE: printf("Statement Ast\n"); break;
        case AstType::RET: printf("Ret Ast\n"); break;
    }
}

void print_ins(Ins ins) {
    switch (ins) {
        case Ins::PUSHL : printf("PUSHL "); break;
        case Ins::PUSHS : printf("PUSHS "); break;
        case Ins::PUSHV : printf("PUSHV "); break;
        case Ins::INT : printf("INT "); break;
        case Ins::MOV : printf("MOV "); break;
        case Ins::LAB : printf("LAB "); break;
        case Ins::FUNC : printf("FUNC "); break;
        case Ins::CALL : printf("CALL "); break;
        case Ins::PRINT : printf("PRINT "); break;
        case Ins::RET : printf("RET "); break;
        case Ins::JE : printf("JE "); break;
        case Ins::JMP : printf("JMP "); break;
        case Ins::ADD : printf("ADD "); break;
        case Ins::SUB : printf("SUB "); break;
        case Ins::MUL : printf("MUL "); break;
        case Ins::DIV : printf("DIV "); break;
        case Ins::MOD : printf("MOD "); break;
        case Ins::GT : printf("GT "); break;
        case Ins::GE : printf("GE "); break;
        case Ins::LT : printf("LT "); break;
        case Ins::LE : printf("LE "); break;
        case Ins::LAND : printf("LAND "); break;
        case Ins::LOR : printf("LOR "); break;
        case Ins::OR : printf("OR "); break;
        case Ins::AND : printf("AND "); break;
        case Ins::XOR : printf("XOR "); break;

    }
}

bool assembly_mode = false;
bool interpret_mode = false;


int main(int argc, char *argv[]) {
    argc--;
    argv++;

    string in_file_name, out_file_name;

    // 命令行参数处理
    while (argc > 0) {
        if (argc > 0 && **argv == '-') {
            switch ((*argv)[1]) {
                case 's': {
                    assembly_mode = true;
                    break;
                }
                case 'i': {
                    interpret_mode = true;
                    break;
                }
                case 'o': {
                    argc--;
                    argv++;
                    out_file_name = argv[0];
                    break;
                }
                default: {
                    printf("命令行参数错误\n");
                    exit(0);
                }
            }
        } else {
            in_file_name = argv[0];
        }
        argv++;
        argc--;
    }

    ifstream in(in_file_name);
    istreambuf_iterator<char> begin(in);
    istreambuf_iterator<char> end;
    content = string(begin, end);

//    cout << content;
    Lexer lexer;
    if (!lexer.lex()) {
        printf("lex error\n");
        return 0;
    }
    lexer.lex_debug();

    Parser parser;
//    parser.preprocess();
    parser.parse();
    parser.parse_debug();

    Semantic::traverse_ast();
    Semantic::symbol_debug();

    CodeGen::gen_vm_code();
    for (auto &item : vm_code) {
        print_ins(item.first);
        cout << item.second << endl;
    }

//    CodeGen::gen_asm_code();
//    cout << asm_code;

    VM vm;
    vm.run();



    return 0;
}
