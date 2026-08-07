BITS 64
org 0x400078
global _start
_start:
mov rax, 5
push rax
mov rax, 4
pop rcx
add rax, rcx
call itoa
mov rax,1
mov rdi,1
lea rsi,[itoa_buffer]
movzx rdx,[itoa_len]
syscall
mov rax, 60
mov rdi, 0
syscall
section .data
count: db 0
itoa_buffer: times 12 db 0
itoa_len: db 0

section .text
itoa:
    push rbp
    mov rbp, rsp

    mov [count], 0
    mov rcx, 10
.loop:
    xor rdx, rdx
    div rcx
    add dl, '0'
    push rdx
    inc [count]
    cmp rax, 0
    jne .loop

    mov al, [count]
    mov [itoa_len], al
    lea rdi, [itoa_buffer]
.pop:
    pop rax
    mov [rdi], al
    inc rdi
    dec [count]
    jnz .pop

    leave
    ret
mov rax, 60
mov rdi, 0
syscall
