//
// Created by zhu honglin on 2019-04-21.
//

#include "lex.h"

bool Lexer::lex() {
    Token result;
    do {
        result = lex_token();
    } while (result != Token::END_OF_FILE && result != Token::ERR);

    if (result == Token::ERR) {
        return false;
    }
    return true;
}

Token Lexer::lex_token() {
    char cur_char = content[pos_++];
    // shift space and '\n'
    while (cur_char == ' ' || cur_char == '\n' || cur_char == '\t') {
        cur_char = content[pos_++];
    }
    // identifier
    // [a-z | A-Z | _] {_ | a-z | A-Z | 0-9}
    if (isalpha(cur_char) || cur_char == '_') {
        string idname;
        idname += cur_char;
        cur_char = content[pos_++];
        while (isalnum(cur_char) || cur_char == '_') {
            idname += cur_char;
            cur_char = content[pos_++];
        }
        --pos_;
        if (idname == "if") {
            tokens.emplace_back(make_pair(Token::KW_IF, idname));
            return Token::KW_IF;
        } else if (idname == "else") {
            tokens.emplace_back(make_pair(Token::KW_ELSE, idname));
            return Token::KW_ELSE;
        } else if (idname == "while") {
            tokens.emplace_back(make_pair(Token::KW_WHILE, idname));
            return Token::KW_WHILE;
        } else if (idname == "int") {
            tokens.emplace_back(make_pair(Token::KW_INT, idname));
            return Token::KW_INT;
        } else if (idname == "return") {
            tokens.emplace_back(make_pair(Token::KW_RET, idname));
            return Token::KW_RET;
        }
        tokens.emplace_back(std::make_pair(Token::IDENTIFIER, idname));
        return Token::IDENTIFIER;
    }
    // 数字 only int
    // [0-9.] {0-9.}
    if (isdigit(cur_char)) {
        string num_str;
        num_str += cur_char;
        cur_char = content[pos_++];
        while (isdigit(cur_char)) {
            num_str += cur_char;
            cur_char = content[pos_++];
        }
        --pos_;
        tokens.emplace_back(make_pair(Token::NUMBER, num_str));
        return Token::NUMBER;
    }
    // other
    switch (cur_char) {
    case '\0': {
        tokens.emplace_back(make_pair(Token::END_OF_FILE, "\0"));
        return Token::END_OF_FILE;
    }
    case '=': {
        // "=="
        if (content[pos_] == '=') {
            pos_++;
            tokens.emplace_back(make_pair(Token::EQ, "=="));
            return Token::EQ;
        }
        // '='
        tokens.emplace_back(make_pair(Token::ASSIGN, "="));
        return Token::ASSIGN;
    }
    case '>': {
        // '>='
        if (content[pos_] == '=') {
            pos_++;
            tokens.emplace_back(make_pair(Token::GE, ">="));
            return Token::GE;
        }
        tokens.emplace_back(make_pair(Token::GT, ">"));
        return Token::GT;
    }
    case '<': {
        // "<="
        if (content[pos_] == '=') {
            pos_++;
            tokens.emplace_back(make_pair(Token::LE, "<="));
            return Token::LE;
        }
        tokens.emplace_back(make_pair(Token::LT, "<"));
        return Token::LT;
    }
    case '+': {
        tokens.emplace_back(make_pair(Token::ADD, "+"));
        return Token::ADD;
    }
    case '-': {
        tokens.emplace_back(make_pair(Token::SUB, "-"));
        return Token::SUB;
    }
    case '*': {
        tokens.emplace_back(make_pair(Token::MUL, "*"));
        return Token::MUL;
    }
    case '/': {
        // 注释 "//"
        if (content[pos_] == '/') {
            while (content[pos_] != '\n') {
                pos_++;
            }
            return Token::COMMENT;
        }
        tokens.emplace_back(make_pair(Token::DIV, "/"));
        return Token::DIV;
    }
    case '%': {
        tokens.emplace_back(make_pair(Token::MOD, "%%"));
        return Token::MOD;
    }
    case '&': {
        // "&&"
        if (content[pos_] == '&') {
            pos_++;
            tokens.emplace_back(make_pair(Token::LAND, "&&"));
            return Token::LAND;
        }
        // "&"
        tokens.emplace_back(make_pair(Token::AND, "&"));
        return Token::AND;
    }
    case '|': {
        // "||"
        if (content[pos_] == '|') {
            pos_++;
            tokens.emplace_back(make_pair(Token::LOR, "||"));
            return Token::LOR;
        }
        // "|"
        tokens.emplace_back(make_pair(Token::OR, "|"));
        return Token::OR;
    }
    case ';': {
        tokens.emplace_back(make_pair(Token::SEMICON, ";"));
        return Token::SEMICON;
    }
    case ',': {
        tokens.emplace_back(make_pair(Token::COMMA, ","));
        return Token::COMMA;
    }
    // string
    case '"': {
        string str;
        cur_char = content[pos_++];
        while (cur_char != '"' && pos_ < content.length()) {
            str += cur_char;
            cur_char = content[pos_++];
        }
        tokens.emplace_back(make_pair(Token::STRING, str));
        return Token::STRING;
    }
    case '(': {
        tokens.emplace_back(make_pair(Token::LPAREN, "("));
        return Token::LPAREN;
    }
    case ')': {
        tokens.emplace_back(make_pair(Token::RPAREN, ")"));
        return Token::RPAREN;
    }
    case '{': {
        tokens.emplace_back(make_pair(Token::LBRACE, "{"));
        return Token::LBRACE;
    }
    case '}': {
        tokens.emplace_back(make_pair(Token::RBRACE, "}"));
        return Token::RBRACE;
    }
    default: {
        // error
        printf("lex_debug_info: cur_char is %c\n", cur_char);
        return Token::ERR;
    }
    } // switch
}

void Lexer::lex_debug() {
    cout << "========= lex ========" << endl;
    for (auto it = tokens.begin(); it != tokens.end(); it++) {
        cout << "type: " << (int)it->first << ";\tvalue: " << it->second
             << endl;
    }
    cout << "========================" << endl;
}