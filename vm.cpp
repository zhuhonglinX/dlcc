//
// Created by zhu honglin on 2019-04-23.
//

#include "vm.h"

#define DEBUG

void VM::run() {
    scan();
    eval();
}

void VM::scan() {
    int line = 0;
    for (auto &it : vm_code) {
        if (it.first == Ins::FUNC || it.first == Ins::LAB) {
            mark_.insert(make_pair(it.second, line));
            if (it.second == "main") {
                pc = line;
            }
        }
        line++;
    }

#ifdef DEBUG
    cout << "mark info: " << endl;
    for (auto &it : mark_) {
        cout << it.first << ": " << it.second << endl;
    }
#endif

}

void VM::eval() {
#ifdef DEBUG
    printf("\n=============eval=============\n");
#endif

    while (pc < vm_code.size()) {
        auto &it = vm_code[pc++];
        switch (it.first) {
            case Ins::PUSHL : {
                int x = stoi(it.second);
                st_.push(x);
                break;
            }
            case Ins::PUSHV : {
                int x = var_symbol[it.second];
                st_.push(x);
                break;
            }
            case Ins::PUSHS : {
                int x = stoi(it.second);
                st_.push(x);
                break;
            }
            case Ins::MOV : {
                var_symbol[it.second] = st_.top();
                st_.pop();
                break;
            }
            case Ins::CALL : {
                func_st_.push(pc);
                pc = mark_[it.second];  // FUNC <...>
                break;
            }
            case Ins::PRINT : {
                int argc = stoi(it.second) - 1;

                string fstr = str_data[st_.top()];
                st_.pop();
                int len = fstr.size();

                int i = 0;
                while (i < len) {
                    if (fstr[i] == '%') {
                        int x = st_.top();
                        st_.pop();
                        i++;
                        printf("%d", x);
                    } else if (fstr[i] == '\\') {
                        i++;
                        if (fstr[i] == 'n') printf("\n");
                        if (fstr[i] == 't') printf("\t");
                        if (fstr[i] == '\\') printf("\\");
                    } else {
                        printf("%c", fstr[i]);
                    }
                    i++;
                }
//                fflush(stdout);
                break;
            }
            case Ins::RET : {
                pc = func_st_.top();
                func_st_.pop();
                break;
            }
            case Ins::JE : {
                int a = st_.top();
                st_.pop();
                if (a == 0) {
                    pc = mark_[it.second];
                }
                break;
            }
            case Ins::JMP : {
                pc = mark_[it.second];
                break;
            }
            case Ins::ADD : {
                int b = st_.top();
                st_.pop();
                int a = st_.top();
                st_.pop();
                st_.push(a + b);
                break;
            }
            case Ins::SUB : {
                int b = st_.top();
                st_.pop();
                int a = st_.top();
                st_.pop();
                st_.push(a - b);
                break;
            }
            case Ins::MUL : {
                int b = st_.top();
                st_.pop();
                int a = st_.top();
                st_.pop();
                st_.push(a * b);
                break;
            }
            case Ins::DIV : {
                int b = st_.top();
                st_.pop();
                int a = st_.top();
                st_.pop();
                st_.push(a / b);
                break;
            }
            case Ins::MOD : {
                int b = st_.top();
                st_.pop();
                int a = st_.top();
                st_.pop();
                st_.push(a % b);
                break;
            }
            case Ins::AND : {
                int b = st_.top();
                st_.pop();
                int a = st_.top();
                st_.pop();
                st_.push(a & b);
                break;
            }
            case Ins::OR : {
                int b = st_.top();
                st_.pop();
                int a = st_.top();
                st_.pop();
                st_.push(a | b);
                break;
            }
            case Ins::XOR : {
                int b = st_.top();
                st_.pop();
                int a = st_.top();
                st_.pop();
                st_.push(a ^ b);
                break;
            }
            case Ins::GT : {
                int b = st_.top();
                st_.pop();
                int a = st_.top();
                st_.pop();
                st_.push(a > b);
                break;
            }
            case Ins::GE : {
                int b = st_.top();
                st_.pop();
                int a = st_.top();
                st_.pop();
                st_.push(a >= b);
                break;
            }
            case Ins::LT : {
                int b = st_.top();
                st_.pop();
                int a = st_.top();
                st_.pop();
                st_.push(a < b);
                break;
            }
            case Ins::LE : {
                int b = st_.top();
                st_.pop();
                int a = st_.top();
                st_.pop();
                st_.push(a <= b);
                break;
            }
            case Ins::LAND : {
                int b = st_.top();
                st_.pop();
                int a = st_.top();
                st_.pop();
                st_.push(a && b);
                break;
            }
            case Ins::LOR : {
                int b = st_.top();
                st_.pop();
                int a = st_.top();
                st_.pop();
                st_.push(a || b);
                break;
            }
            default: {}
        }
    }
}