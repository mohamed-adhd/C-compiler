#include "codegen.h"
#include <stdio.h>


void codegen_program(astnode *program, FILE *out) {
    const char *ITOA_HELPER =
    "section .bss\n"
    "    count: resb 1\n"
    "    itoa_buffer: resb 12\n"
    "    itoa_len: resb 1\n"
    "\n"
    "section .text\n"
    "itoa:\n"
    "    push rbp\n"
    "    mov rbp, rsp\n"
    "\n"
    "    mov byte [count], 0\n"
    "    mov rcx, 10\n"
    ".loop:\n"
    "    xor rdx, rdx\n"
    "    div rcx\n"
    "    add dl, '0'\n"
    "    push rdx\n"
    "    inc byte [count]\n"
    "    cmp rax, 0\n"
    "    jne .loop\n"
    "\n"
    "    mov al, [count]\n"
    "    mov [itoa_len], al\n"
    "    lea rdi, [itoa_buffer]\n"
    ".pop:\n"
    "    pop rax\n"
    "    mov [rdi], al\n"
    "    inc rdi\n"
    "    dec byte [count]\n"
    "    jnz .pop\n"
    "\n"
    "    leave\n"
    "    ret\n";
    fprintf(out,"%s", ITOA_HELPER);
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
    }else if (expr->type == NODE_PRINT) {
        codegen_expression(expr->expr, out);
        codegen_print(out,expr->value);

    }

}
void codegen_print(FILE *out,int val) {

    fprintf(out, "mov rax, %d\n",val) ;
    fprintf(out, "call itoa\n") ;



}
