cmake_minimum_required(VERSION 3.14)
project(dlcc)

set(CMAKE_CXX_STANDARD 11)

add_executable(dlcc main.cpp dlcc.h lex.cpp lex.h ast.cpp ast.h parser.cpp parser.h semantic.cpp semantic.h env.cpp env.h codegen.cpp codegen.h vm.cpp vm.h)