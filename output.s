section .bss
    count: resb 1
    itoa_buffer: resb 12
    itoa_len: resb 1

section .text
itoa:
    push rbp
    mov rbp, rsp

    mov byte [count], 0
    mov rcx, 10
.loop:
    xor rdx, rdx
    div rcx
    add dl, '0'
    push rdx
    inc byte [count]
    cmp rax, 0
    jne .loop

    mov al, [count]
    mov [itoa_len], al
    lea rdi, [itoa_buffer]
.pop:
    pop rax
    mov [rdi], al
    inc rdi
    dec byte [count]
    jnz .pop

    leave
    ret
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
movzx rdx,byte [itoa_len]
syscall
mov rax, 60
mov rdi, 0
syscall
