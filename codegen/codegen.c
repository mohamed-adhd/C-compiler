#include "codegen.h"
#include <fstream>

void codegen_program(astnode *program, FILE *out);
void codegen_function(astnode *function, FILE *out);
void codegen_statement(astnode *stmt, FILE *out) {
    codegen_expression(stmt->expr,out);
    fprintf(out, "ret\n");
}
char *codegen_expression(astnode *expr, FILE *out) {
    fprintf(out, "movl $%d, %%eax\n", expr->value);
}