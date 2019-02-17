[global _start]
section .text
_start:
    xor rax,rax
    lea rdi,[0xB8000]
    lea rsi,[hello]
    mov ah,0xF0
.print:
    lodsb
    cmp al,0
    je .done_loop
    stosw
    jmp .print
.done_loop:
    pause
    cli
    hlt
    jmp .done_loop

section .data
hello db "hello world!",0