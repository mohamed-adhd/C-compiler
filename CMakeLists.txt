cmake_minimum_required(VERSION 4.3)
project(C_compiler C)

set(CMAKE_C_STANDARD 11)

include_directories(.)
include_directories(codegen)
include_directories(elfgen)
include_directories(lexer)
include_directories(parse)
include_directories(semantic)
include_directories(type)
include_directories(util)

add_executable(C_compiler
        codegen/codegen.c
        codegen/codegen.h
        lexer/lexer.c
        elfgen/elfgen.c
        elfgen/elfgen.h
        lexer/lexer.h
        lexer/token.c
        lexer/token.h
        parse/parse.c
        parse/parse.h
        semantic/symb.c
        semantic/symb.h
        type/type.c
        type/type.h
        util/errors.h
        util/util.c
        util/util.h
        main.c
        elfgen/elfgen.h)
