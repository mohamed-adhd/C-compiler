#include "codegen.h"
#include <stdio.h>


void codegen_program(astnode *program, FILE *out) {
    codegen_function(program->function, out);
    //it will staty like this just for a while
}
void codegen_function(astnode *function, FILE *out) {
    fprintf(out, "globl %s :\n%s", function->name, function->name);
    codegen_statement(function->body, out);
}
void codegen_statement(astnode *stmt, FILE *out) {
    codegen_expression(stmt->expr,out);
    fprintf(out, "ret\n");
}
char *codegen_expression(astnode *expr, FILE *out) {
    fprintf(out, "movl $%d, %%eax\n", expr->value);
}