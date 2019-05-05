//
// Created by zhu honglin on 2019-04-21.
//

#ifndef MYCOMPILER_ENV_H
#define MYCOMPILER_ENV_H

#include "dlcc.h"
#include <iostream>
#include <map>
#include <string>
#include <tuple>

using namespace std;

class VarEnv {
  public:
    static int env_cnt_; // 统计创建的环境(作用域)个数
    static tuple<VarEnv *, bool> get_var_env(string &name, VarEnv *cur_env);

    VarEnv *prev_, *next_, *fa_, *fch_, *lch_;
    int scope_id_;
    map<string, Token> var_symbol_;

    VarEnv();

    void add_child_env(VarEnv *child);
    bool var_exist(string &name);
    void print_info();
};

#endif // MYCOMPILER_ENV_H
