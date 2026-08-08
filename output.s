BITS 64
org 0x400078
section .data
count: db 0
itoa_buffer: times 12 db 0
itoa_len: db 0
str0 db "nigga i m alive"

section .text
global _start
_start:
mov rax, 1
mov rdi, 1
lea rsi, [str0]
mov rdx, 15
syscall
mov rax, 60
mov rdi, 0
syscall
mov rax, 60
mov rdi, 0
syscall
