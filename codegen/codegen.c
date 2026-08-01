#include "codegen.h"
#include <stdio.h>


void codegen_program(astnode *program, FILE *out) {
    codegen_function(program->function, out);
    //it will staty like this just for a while
}
void codegen_function(astnode *function, FILE *out) {
    fprintf(out, ".globl %s\n%s:\n", function->name, function->name);
    codegen_statement(function->body, out);
}
void codegen_statement(astnode *stmt, FILE *out) {
    codegen_expression(stmt->expr,out);
    fprintf(out, "ret\n");
}
//fuck that bitch , fuck this compiler, fuck c , fuck assembly , and fuck this god damn heat
void codegen_expression(astnode *expr, FILE *out) {
    if (expr->type == NODE_CONSTANT) {
        fprintf(out, "movl $%d, %%eax\n", expr->value);
    } else if (expr->type == NODE_BINARY_OP) {
        codegen_expression(expr->left, out);
        fprintf(out, "push %%eax\n");
        codegen_expression(expr->right, out);
        fprintf(out, "pop %%ecx\n");
        if (expr->op == TOKEN_ADD) fprintf(out, "addl %%ecx, %%eax\n");
        if (expr->op == TOKEN_SUB) fprintf(out, "subl %%ecx, %%eax\n");
    }
}
