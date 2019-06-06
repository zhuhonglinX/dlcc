//
// Created by zhu honglin on 2019-04-23.
//

#ifndef MYCOMPILER_VM_H
#define MYCOMPILER_VM_H

#include "dlcc.h"
#include <iostream>
#include <map>
#include <sstream>
#include <stack>
#include <string>
using namespace std;

class VM {
  public:
    int pc = 0;
    int cur_fptr = 0;

    vector<map<string, int>> func_st_;
    stack<int> data_st_;
    stack<int> pc_st_;
    map<string, int> mark_;

    void run();
    void scan();
    void eval();
};

#endif // MYCOMPILER_VM_H
