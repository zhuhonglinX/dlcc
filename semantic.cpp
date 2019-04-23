//
// Created by zhu honglin on 2019-04-21.
//

#include "semantic.h"

#define DEBUG

void Semantic::traverse_ast() {
#ifdef DEBUG
    cout << "traverse_ast" << endl;
#endif
    for (auto ast : ast_list) {
        gen_symbol(ast, top_env);
    }
}

void Semantic::gen_symbol(AstNode *node, VarEnv *cur_env) {
#ifdef DEBUG
    printf("gen_symbol: ");
    print_ast_type(node->get_ast_type());
#endif
    switch (node->get_ast_type()) {
        case AstType::FUNC : {
            // 将函数加入函数符号表
            auto *func = dynamic_cast<FuncAst *>(node);
//            func_tbl.insert(make_pair(func->name_, func_symbol.size()));
//            func_symbol.emplace_back(func->name_);

            auto *env = new VarEnv();
            cur_env->add_child_env(env);

            func->env_ = env;

            // args
            for (auto &arg : func->args_) {
                env->var_symbol_.insert(make_pair(arg.second, arg.first));
                arg.second += to_string(env->scope_id_);

//                var_tbl.insert(make_pair(arg.second, var_symbol.size()));
                var_symbol.insert(make_pair(arg.second, 0));
            }

            // block
            auto *block = dynamic_cast<BlockAst *>(func->block_ast_);
            for (auto stat : block->stats_) {
                gen_symbol(stat, env);
            }
            break;
        }
        case AstType::CALLFUNC : {
            auto *call_func = dynamic_cast<CallFuncAst *>(node);
            call_func->env_ = cur_env;
            for (auto arg : call_func->args_) {
                gen_symbol(arg, cur_env);
            }
            break;
        }
        case AstType::VARDEC : {
            auto *var_dec = dynamic_cast<VarDecAst *>(node);
            var_dec->env_ = cur_env;
            cur_env->var_symbol_.insert(make_pair(var_dec->name_, var_dec->type_));
            var_symbol.insert(make_pair(var_dec->name_ + to_string(cur_env->scope_id_), 0));
            break;
        }
        case AstType::BINEXP : {
            auto *bin_exp = dynamic_cast<BinExpAst *>(node);
            bin_exp->env_ = cur_env;
            gen_symbol(bin_exp->lt_, cur_env);
            gen_symbol(bin_exp->rt_, cur_env);
            break;
        }
        case AstType::IfExp : {
            auto *if_exp = dynamic_cast<IfExpAst *>(node);
            if_exp->env_ = cur_env;
            gen_symbol(if_exp->cond_, cur_env);
            gen_symbol(if_exp->if_block_, cur_env);
            if (if_exp->else_block_) {
                gen_symbol(if_exp->else_block_, cur_env);
            }
            break;
        }
        case AstType::BLOCK : {
            auto *env = new VarEnv();
            cur_env->add_child_env(env);
            auto *block = dynamic_cast<BlockAst *>(node);
            block->env_ = cur_env;
            for (auto stat : block->stats_) {
                gen_symbol(stat, env);
            }
            break;
        }
        case AstType::VAR : {
            auto *var = dynamic_cast<VarAst *>(node);
            VarEnv *target_env;
            bool ok;
            tie(target_env, ok) = VarEnv::get_var_env(var->name_, cur_env);
            // 没找到这个变量
            if (!ok) {
                printf("var '%s' not define\n", var->name_.c_str());
                exit(0);
            }
            var->env_ = target_env;
            var->name_ += to_string(target_env->scope_id_);

            break;
        }
        case AstType::STR : {
            auto *str_ast = dynamic_cast<StrAst *>(node);
            str_ast->env_ = top_env;  // 暂时没用
            str_ast->data_offset_ = (int) str_data.size();
            str_data.push_back(str_ast->value_);
            break;
        }
        case AstType::RET : {
            auto *ret = dynamic_cast<ReturnAst *>(node);
            ret->env_ = cur_env;
            gen_symbol(ret->ret_value_, cur_env);
            break;
        }
        default: {
            cout << "gen_symbol: default ";
            print_ast_type(node->get_ast_type());
            break;
        }
    }
}

void Semantic::symbol_debug() {
    printf("traverse env tree:\n");
    print_symbol_info(top_env);
}

void Semantic::print_symbol_info(VarEnv *e) {
    e->print_info();
    if (e->next_) {
        print_symbol_info(e->next_);
    }
    if (e->fch_) {
        print_symbol_info(e->fch_);
    }
}