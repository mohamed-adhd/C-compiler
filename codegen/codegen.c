#include "codegen.h"
#include <stdio.h>

#include "elfgen.h"
void codegen_program(astnode *program, FILE *out) {
    const char *ITOA_HELPER =
        "BITS 64\n"
  "org 0x400000\n"
  "section .data\n"
  "count: db 0\n"
  "itoa_buffer: times 12 db 0\n"
  "itoa_len: db 0\n"
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

}
void codegen_function(astnode *function, FILE *out) {
    (void)function;
    fprintf(out, "global _start\n_start:\n");
    codegen_statement(function->body, out);

}
void codegen_statement(astnode *stmt, FILE *out) {
    if (stmt->type == NODE_PRINT) {
        codegen_expression(stmt->expr, out);
        codegen_print(out,stmt->value);
        return;
    }
    codegen_expression(stmt->expr,out);
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
        if (expr->op == TOKEN_SUB) fprintf(out, "sub rcx, rax\nmov rax, rcx\n");
    }
}
void codegen_print(FILE *out,int val) {
    fprintf(out, "call itoa\n") ;
    const char *s="mov rax,1\n"
                "mov rdi,1\n"
                "lea rsi,[itoa_buffer]\n"
                "movzx rdx,byte [itoa_len]\n"
                "syscall\n";
    fprintf(out,"%s", s);




}
