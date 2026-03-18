bits 64
section .text
global _start

_start:
    jmp actual_code

; treat this as a poor man's .data segment lol
_data:
    db "Hello, whatever", 0xA

actual_code:
    mov rax, 1  ; write
    mov rdi, 1  ; STDOUT
    lea rsi, [rel _data] ; data ptr
    mov rdx, 16          ; msg_len
    syscall
    mov rax, 60 ; exit syscall
    mov rdi, 39 ; exit code 39
    syscall