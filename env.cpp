//
// Created by zhu honglin on 2019-04-21.
//

#include "env.h"

int VarEnv::env_cnt_ = 0;

// 返回该变量应当存在的 env(作用域)
tuple<VarEnv *, bool> VarEnv::get_var_env(string &name, VarEnv *cur_env) {
    if (cur_env->var_exist(name)) {
        // 在当前 env 找到变量
        return make_tuple(cur_env, true);
    }
    // 当前 env 未找到
    if (cur_env->fa_) {
        return get_var_env(name, cur_env->fa_);
    }

    return make_tuple(nullptr, false);
}

VarEnv::VarEnv() {
    prev_ = next_ = fa_ = fch_ = lch_ = nullptr;
    scope_id_ = env_cnt_++;
}

void VarEnv::add_child_env(VarEnv *child) {
    if (fch_) {
        child->fa_ = this;
        lch_->next_ = child;
        child->prev_ = lch_;
        lch_ = child;
    } else {  // no child
        fch_ = lch_ = child;
        child->fa_ = this;
    }
}

bool VarEnv::var_exist(string &name) {
    return var_symbol_.count(name) != 0;
}

void VarEnv::print_info() {
    printf("===========================\n");
    printf("env id: %d\n", scope_id_);
    if (fa_) {
        printf("father env id: %d, ", fa_->scope_id_);
    } else {
        printf("top env, ");
    }

    if(prev_) {
        printf("prev env id: %d, ", prev_->scope_id_);
    } else {
        printf("no prev env, ");
    }

    if (next_) {
        printf("next env id: %d, ", next_->scope_id_);
    } else {
        printf("no next env, ");
    }

    if (fch_) {
        printf("first child env id: %d\n", fch_->scope_id_);
    } else {
        printf("no child env\n");
    }

    if (var_symbol_.empty()) {
        printf("no var belongs this env\n");
    } else {
        printf("list variables belong to this env:\n");
        for (auto it : var_symbol_) {
            cout << it.first << endl;
        }
    }
    printf("===========================\n");
}