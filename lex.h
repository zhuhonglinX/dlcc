//
// Created by zhu honglin on 2019-04-21.
//

#ifndef MYCOMPILER_LEX_H
#define MYCOMPILER_LEX_H

#include <iostream>
#include <string>
#include <vector>
#include "dlcc.h"

using namespace std;

class Lexer {
    int pos_ = 0;
public:
    bool lex();
    Token lex_token();
    void lex_debug();
};


#endif //MYCOMPILER_LEX_H
