[bits 16]

org 8000h
trampolin:
    jmp start
data_struct:
    .mtx dw 0
    .proc_count dd 0 
    .page_table dq 0
    TIMES (128-($-trampolin)) db 0
start:
    cli
    ;   acquire trampolin mutex
.acquire_mtx:
    mov ax,1
    xchg ax,[data_struct.mtx]
    cmp ax,0
    jnz .acquire_mtx
    ;   mutex acquired, increase processor count
    inc DWORD [data_struct.proc_count]
    ;   release mutex
    mov WORD [data_struct.mtx], 0
.enable_long_mode:
    mov eax, cr4                 ; Set the A-register to control register 4.
    or eax, 1 << 5               ; Set the PAE-bit, which is the 6th bit (bit 5).
    mov cr4, eax                 ; Set control register 4 to the A-register.

    mov ecx, 0xC0000080
    rdmsr
    or eax, 1 << 8
    wrmsr

    mov eax, cr0
    or eax, 1 << 31 | 1 << 0
    mov cr0, eax

    mov eax, cr4
    or eax, (1<<12) ;CR4.LA57
    mov cr4, eax
    ;   far jump to long mode
    lgdt [gdt_long.Pointer]         ; Load the 64-bit global descriptor table.
    jmp gdt_long.Code:long_mode       ; Set the code segment and enter 64-bit long mode.

[bits 64]
long_mode:
    mov ax, gdt_long.Data            ; Set the A-register to the data descriptor.
    mov ds, ax                    ; Set the data segment to the A-register.
    mov es, ax                    ; Set the extra segment to the A-register.
    mov fs, ax                    ; Set the F-segment to the A-register.
    mov gs, ax                    ; Set the G-segment to the A-register.
    mov ss, ax                    ; Set the stack segment to the A-register.
    mov edi, 0xB8000              ; Set the destination index to 0xB8000.
    mov rax, 0x1F201F201F201F20   ; Set the A-register to 0x1F201F201F201F20.
    mov ecx, 500                  ; Set the C-register to 500.
    rep stosq                     ; Clear the screen.
    ;   hang indefinitely
    hlt
    jmp start
gdt_long:                           ; Global Descriptor Table (64-bit).
    .Null: equ $ - gdt_long         ; The null descriptor.
    dw 0xFFFF                    ; Limit (low).
    dw 0                         ; Base (low).
    db 0                         ; Base (middle)
    db 0                         ; Access.
    db 1                         ; Granularity.
    db 0                         ; Base (high).
    .Code: equ $ - gdt_long         ; The code descriptor.
    dw 0                         ; Limit (low).
    dw 0                         ; Base (low).
    db 0                         ; Base (middle)
    db 10011010b                 ; Access (exec/read).
    db 10101111b                 ; Granularity, 64 bits flag, limit19:16.
    db 0                         ; Base (high).
    .Data: equ $ - gdt_long         ; The data descriptor.
    dw 0                         ; Limit (low).
    dw 0                         ; Base (low).
    db 0                         ; Base (middle)
    db 10010010b                 ; Access (read/write).
    db 00000000b                 ; Granularity.
    db 0                         ; Base (high).
    .Pointer:                    ; The GDT-pointer.
    dw $ - gdt_long - 1             ; Limit.
    dq gdt_long                     ; Base.
trampolin_end:
    TIMES (512-($-trampolin)) db 0x90