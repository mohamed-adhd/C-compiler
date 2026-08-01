#include "parse.h"
#include <fstream>
#include <c++/16.1.1/cstdio>

void codegen_program(astnode *program, FILE *out);
void codegen_function(astnode *function, FILE *out);
void codegen_statement(astnode *stmt, FILE *out);
void codegen_expression(astnode *expr, FILE *out);