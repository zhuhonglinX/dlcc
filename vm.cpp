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
                pc = line; // 起始指令
                // 初始化函数栈和函数栈指针
                map<string, int> symbol = {{it.second, 0}};
                func_st_.emplace_back(symbol);
                cur_fptr = func_st_.size() - 1;
            }
        }
        line++;
    }
    // 初始化 pc 栈
    pc_st_.push(vm_code.size());

#ifdef DEBUG
    cout << "============ mark info =============" << endl;
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
        case Ins::PUSHL: {
            int x = stoi(it.second);
            data_st_.push(x);
            break;
        }
        case Ins::PUSHV: {
            int x = func_st_[cur_fptr][it.second];
            data_st_.push(x);
            break;
        }
        case Ins::PUSHS: {
            int x = stoi(it.second);
            data_st_.push(x);
            break;
        }
        case Ins::INT: {
            func_st_[cur_fptr].insert(make_pair(it.second, 0));
            break;
        }
        case Ins::MOV: {
            func_st_[cur_fptr][it.second] = data_st_.top();
            data_st_.pop();
            break;
        }
        case Ins::CALL: {
            pc_st_.push(pc);
            pc = mark_[it.second]; // FUNC <...>
            map<string, int> symbol = {{it.second, 0}};
            func_st_.emplace_back(symbol);
            cur_fptr++;
            break;
        }
        case Ins::PRINT: {
            int argc = stoi(it.second) - 1;
            string fstr = str_data[data_st_.top()];
            data_st_.pop();
            int len = fstr.size();
            int i = 0;
            while (i < len) {
                if (fstr[i] == '%') {
                    int x = data_st_.top();
                    data_st_.pop();
                    i++;
                    printf("%d", x);
                } else if (fstr[i] == '\\') {
                    i++;
                    if (fstr[i] == 'n')
                        printf("\n");
                    else if (fstr[i] == 't')
                        printf("\t");
                    else if (fstr[i] == '\\')
                        printf("\\");
                } else {
                    printf("%c", fstr[i]);
                }
                i++;
            }
            // fflush(stdout);
            break;
        }
        case Ins::RET: {
            pc = pc_st_.top();
            pc_st_.pop();
            func_st_.pop_back();
            cur_fptr--;
            break;
        }
        case Ins::JE: {
            int a = data_st_.top();
            data_st_.pop();
            if (a == 0) {
                pc = mark_[it.second];
            }
            break;
        }
        case Ins::JMP: {
            pc = mark_[it.second];
            break;
        }
        case Ins::ADD: {
            int b = data_st_.top();
            data_st_.pop();
            int a = data_st_.top();
            data_st_.pop();
            data_st_.push(a + b);
            break;
        }
        case Ins::SUB: {
            int b = data_st_.top();
            data_st_.pop();
            int a = data_st_.top();
            data_st_.pop();
            data_st_.push(a - b);
            break;
        }
        case Ins::MUL: {
            int b = data_st_.top();
            data_st_.pop();
            int a = data_st_.top();
            data_st_.pop();
            data_st_.push(a * b);
            break;
        }
        case Ins::DIV: {
            int b = data_st_.top();
            data_st_.pop();
            int a = data_st_.top();
            data_st_.pop();
            data_st_.push(a / b);
            break;
        }
        case Ins::MOD: {
            int b = data_st_.top();
            data_st_.pop();
            int a = data_st_.top();
            data_st_.pop();
            data_st_.push(a % b);
            break;
        }
        case Ins::AND: {
            int b = data_st_.top();
            data_st_.pop();
            int a = data_st_.top();
            data_st_.pop();
            data_st_.push(a & b);
            break;
        }
        case Ins::OR: {
            int b = data_st_.top();
            data_st_.pop();
            int a = data_st_.top();
            data_st_.pop();
            data_st_.push(a | b);
            break;
        }
        case Ins::XOR: {
            int b = data_st_.top();
            data_st_.pop();
            int a = data_st_.top();
            data_st_.pop();
            data_st_.push(a ^ b);
            break;
        }
        case Ins::GT: {
            int b = data_st_.top();
            data_st_.pop();
            int a = data_st_.top();
            data_st_.pop();
            data_st_.push(a > b);
            break;
        }
        case Ins::GE: {
            int b = data_st_.top();
            data_st_.pop();
            int a = data_st_.top();
            data_st_.pop();
            data_st_.push(a >= b);
            break;
        }
        case Ins::LT: {
            int b = data_st_.top();
            data_st_.pop();
            int a = data_st_.top();
            data_st_.pop();
            data_st_.push(a < b);
            break;
        }
        case Ins::LE: {
            int b = data_st_.top();
            data_st_.pop();
            int a = data_st_.top();
            data_st_.pop();
            data_st_.push(a <= b);
            break;
        }
        case Ins::LAND: {
            int b = data_st_.top();
            data_st_.pop();
            int a = data_st_.top();
            data_st_.pop();
            data_st_.push(a && b);
            break;
        }
        case Ins::LOR: {
            int b = data_st_.top();
            data_st_.pop();
            int a = data_st_.top();
            data_st_.pop();
            data_st_.push(a || b);
            break;
        }
        default: {}
        } // switch
    }
}