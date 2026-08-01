#include <stdio.h>

#include "parse.h"

void codegen_program(astnode *program, FILE *out);
void codegen_function(astnode *function, FILE *out);
void codegen_statement(astnode *stmt, FILE *out);
void codegen_expression(astnode *expr, FILE *out);