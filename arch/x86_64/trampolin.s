[bits 16]

org 8000h
trampolin:
    jmp start
data_struct:
    TIMES (128-($-trampolin)) db 0
start:
    xor ax,ax
    mov di,0xb800
    mov si, [hello_world]
    mov cx,12
print_loop:
    lodsb
    stosw
    dec cx
    cmp cx,0
    jnz print_loop
    cli
    hlt
    jmp start
hello_world db 'hello world!'
trampolin_end:
    TIMES (512-($-trampolin)) db 0x90