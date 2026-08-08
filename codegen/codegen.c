#include "codegen.h"
#include <stdio.h>
#include <string.h>
#include "elfgen.h"
void codegen_program(astnode *program, FILE *out) {
    const char *ITOA_HELPER =
    "itoa:\n"
    "    push rbp\n"
    "    mov rbp, rsp\n"
    "\n"
    "    mov [count], 0\n"
    "    mov rcx, 10\n"
    ".loop:\n"
    "    xor rdx, rdx\n"
    "    div rcx\n"
    "    add dl, '0'\n"
    "    push rdx\n"
    "    inc [count]\n"
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
    "    dec [count]\n"
    "    jnz .pop\n"
    "\n"
    "    leave\n"
    "    ret\n";
    fprintf(out, "BITS 64\n");
    fprintf(out, "org 0x400078\n");
    fprintf(out, "section .data\n");
    fprintf(out, "count: db 0\n");
    fprintf(out, "itoa_buffer: times 12 db 0\n");
    fprintf(out, "itoa_len: db 0\n");
    if (program->function->body->type == NODE_PRINT &&program->function->body->expr->type == NODE_STRING) {fprintf(out, "str0 db \"%s\"\n",program->function->body->expr->string);}
    fprintf(out, "\nsection .text\n");
    codegen_function(program->function, out);
    fprintf(out, "%s", ITOA_HELPER);
}
void codegen_function(astnode *function, FILE *out) {
    (void)function;
    fprintf(out, "global _start\n_start:\n");
    codegen_statement(function->body, out);
    fprintf(out, "mov rax, 60\n");
    fprintf(out, "mov rdi, 0\n");
    fprintf(out, "syscall\n");
}
void codegen_statement(astnode *stmt, FILE *out) {
    if (stmt->type == NODE_PRINT) {
        codegen_print(out, stmt->expr);
        return;}
    if (stmt->type == NODE_RETURN) {
        codegen_expression(stmt->expr, out);
        return;
    }
}
//fuck that bitch , fuck this compiler, fuck c , fuck assembly , and fuck this god damn heat
void codegen_expression(astnode *expr, FILE *out) {
    if (expr->type == NODE_CONSTANT) {
        fprintf(out, "mov rax, %d\n", expr->value);
    } else if (expr->type == NODE_BINARY_OP) {
        codegen_expression(expr->left, out);
        fprintf(out, "push rax\n");
        codegen_expression(expr->right, out);
        fprintf(out, "pop rcx\n");
        if (expr->op == TOKEN_ADD) fprintf(out, "add rax, rcx\n");
        if (expr->op == TOKEN_SUB) fprintf(out, "sub rcx, rax\nmov rax, rcx\n");}}
void codegen_print(FILE *out, astnode *expr) {

    if (expr->type == NODE_CONSTANT ||
        expr->type == NODE_BINARY_OP) {
        codegen_expression(expr, out);
        fprintf(out, "call itoa\n");
        fprintf(out,
            "mov rax, 1\n"
            "mov rdi, 1\n"
            "lea rsi, [itoa_buffer]\n"
            "movzx rdx, byte [itoa_len]\n"
            "syscall\n"
        );return;}
    if (expr->type == NODE_STRING) {
        fprintf(out,
            "mov rax, 1\n"
            "mov rdi, 1\n"
            "lea rsi, [str0]\n"
            "mov rdx, %zu\n"
            "syscall\n",
            strlen(expr->string)
        );
    }
}
