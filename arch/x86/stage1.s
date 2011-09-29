[BITS 32]
[global start]
[global stack]
[extern stage1_5]

; setting up the Multiboot header - see GRUB docs for details
MODULEALIGN equ  1<<0                   ; align loaded modules on page boundaries
MEMINFO     equ  1<<1                   ; provide memory map
FLAGS       equ  MODULEALIGN | MEMINFO  ; this is the Multiboot 'flag' field
MAGIC       equ    0x1BADB002           ; 'magic number' lets bootloader find the header
CHECKSUM    equ -(MAGIC + FLAGS)        ; checksum required

section .stext
align 4
 

MultiBootHeader:
   dd MAGIC
   dd FLAGS
   dd CHECKSUM
STACKSIZE equ 0x1000                  ; that's 4k.
start:
    mov esp,stack +STACKSIZE- 0xc0000000
    pusha
    mov eax,gdt_ptr
    lgdt [eax]
    jmp 0x08:start.new_gdt
.new_gdt:
    mov ax,0x10
    mov ds,ax
    mov es,ax
    mov gs,ax
    mov fs,ax
    mov ss,ax
    popa
    mov esp,stack +STACKSIZE- 0xc0000000
    push eax                           ; pass Multiboot magic number
    push ebx                           ; pass Multiboot info structure
    cli
    call stage1_5
hang:
    jmp hang
t1:
    xchg bx,bx
    jmp hang
gdt_ptr:
    dw gdt_entry.end - gdt_entry-1
    dd gdt_entry
gdt_entry:
.gdt0:
    dd 0x0
    dd 0x0
.gdt1:
    dw 0xFFFF
    dw 0x0000
    db 0x00
    db 0x9A
    db 0xCF
    db 0x00
.gdt2:
    dw 0xFFFF
    dw 0x0000
    db 0x00
    db 0x92
    db 0xCF
    db 0x00
.end:
section .bss
align 4096
stack:
	resb STACKSIZE
