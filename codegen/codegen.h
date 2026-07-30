#include "parse.h"
#include <fstream>

void codegen_program(astnode *program, FILE *out);
void codegen_function(astnode *function, FILE *out);
void codegen_statement(astnode *stmt, FILE *out);
char *codegen_expression(astnode *expr, FILE *out); 