;
; Copyright 2009-2017 Srijan Kumar Sharma
;
; This file is part of Momentum.
;
; Momentum is free software: you can rRDIstribute it AND/or modify
; it under the terms of the GNU General Public License as published by
; the Free Software Foundation, either version 3 of the License, or
; (at your option) any later version.
;
; Momentum is distributed in the hope that it will be useful,
; but WITHOUT ANY WARRANTY; without even the implied warranty of
; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
; GNU General Public License for more details.
;
; You should have received a copy of the GNU General Public License
; along with Momentum.  If not, see <http://www.gnu.org/licenses/>.
;

[BITS 32]
[global loader]
[global stack]
[global alined_alloc]
[global g_qKernelEnd]
[global PML4T]
[global PDPT]
[global PDT]
[extern kernel_start]
[extern kernel_end]
[extern stage2]
[extern readMultiBootInfo]

section .text0
; setting up the Multiboot header - see GRUB docs for details
MODULEALIGN equ  1<<0                   ; align loaded modules on page boundaries
MEMINFO     equ  1<<1                   ; provide memory map
FLAGS       equ  MODULEALIGN | MEMINFO  ; this is the Multiboot 'flag' field
MAGIC       equ    0x1BADB002           ; 'magic number' lets bootloader find the header
CHECKSUM    equ -(MAGIC + FLAGS)        ; checksum required

section .text0
align 8

MultiBootHeader:
   dd MAGIC
   dd FLAGS
   dd CHECKSUM
STACKSIZE equ 0x1000
align 8
[BITS 32]
loader:
    ;EBX has multiboot info structure.
	cli
	MOV ESP,stack+STACKSIZE
	MOV EAX,kernel_end-0xC0000000
    MOV DWORD [kend_lo-0xC0000000],EAX
;
;	KILL PIC, its dead.
;	
	mov al, 0xff
	out 0xa1, al
	out 0x21, al
;
;	End any pending interrupt
;
	mov al, 0x20
	out 0xa0, al
	out 0x20, al
.gdt:
    MOV eax,gdt_start
    lgdt [EAX]
    jmp 0x08:.reload_cs
.reload_cs:
    MOV   AX, 0x10 ; 0x10 points at the new data selector
    MOV   DS, AX
    MOV   ES, AX
    MOV   FS, AX
    MOV   GS, AX
    MOV   SS, AX
;
;	enable cr0.PM
;
	mov eax, 000000011h
	mov cr0, eax
;
;	load new page table. 1gb identity mapped
;
	MOV EAX,PML4T-0xC0000000
	MOV cr3,EAX
;
;	enable PAE
;
	mov eax, cr4                 ; Set the A-register to control register 4.
    or eax, 1 << 5               ; Set the PAE-bit, which is the 6th bit (bit 5).
    mov cr4, eax                 ; Set control register 4 to the A-register.
;
;	The Switch from Protected Mode to long mode
;
	mov ecx, 0xC0000080          ; Set the C-register to 0xC0000080, which is the EFER MSR.
    rdmsr                        ; Read from the model-specific register.
    or eax, 1 << 8               ; Set the LM-bit which is the 9th bit (bit 8).
    wrmsr                        ; Write to the model-specific register.
;
;	Enabling paging
;
	mov eax, cr0                 ; Set the A-register to control register 0.
    or eax, 1 << 31              ; Set the PG-bit, which is the 32nd bit (bit 31).
    mov cr0, eax                 ; Set control register 0 to the A-register.
.gdt64:
	lgdt [GDT64.Pointer]
	jmp GDT64.Code:Realm64
; Use 64-bit.
[BITS 64]
 
Realm64:
    mov ax, GDT64.Data            ; Set the A-register to the data descriptor.
    mov ds, ax                    ; Set the data segment to the A-register.
    mov es, ax                    ; Set the extra segment to the A-register.
    mov fs, ax                    ; Set the F-segment to the A-register.
    mov gs, ax                    ; Set the G-segment to the A-register.
    mov ss, ax                    ; Set the stack segment to the A-register.
    mov edi, 0xB8000              ; Set the destination index to 0xB8000.
    mov RAX, 0x1F201F201F201F20   ; Set the A-register to 0x1F201F201F201F20.
    mov ecx, 500                  ; Set the C-register to 500.
    rep stosq                     ; Clear the screen.

	MOV RAX,PML4T-0xC0000000
	MOV cr3,RAX

;	wait for debugger
;	MOV RAX,1
;debugger:
;	test RAX,1
;	pause
;	jnz debugger
	cli
;
;	push stack magic marker.
;
	MOV RBP,0xDEADBEEFDEADBEEF
	PUSH RBP
	PUSH RBP
	MOV RBP,RSP
	MOV rdi,rbx
	MOV RAX, stage2
	CALL RAX
hang:
    cli;
	hlt;
[BITS 32]
find_acpi:
	MOV EAX,0xE0000
.loop:
        cmp DWORD [EAX],'RSD '
        jne .continue
        cmp DWORD [EAX+4],'PTR '
        je .done
.continue:
        cmp EAX,0xFFFFF
        jge .failed
        add EAX,2
        jmp .loop
.failed:
        xor EAX,EAX
.done:
        ret


parse_acpi_tables:
        CALL find_acpi                  ;got rsdp.
        cmp EAX,0
        jz .failed
        MOV DWORD [EBX + rsdp - loader],EAX
        MOV EAX,[EAX+16]                ;got rsdt.
        cmp DWORD [EAX],'RSDT'
        jne .failed
        MOV ECX,[EAX+4]
        SUB ECX,36                      ;length of descriptor tables.
        shr ECX,2                       ;divide by 4.
        add EAX,36
.test_table:
        cmp ECX,0
        jz .failed
        MOV EDX,[EAX]
        cmp DWORD [EDX],'APIC'
        je .found_madt
        add EAX,4
        dec ECX
        jmp .test_table
.found_madt:
        MOV EAX,EDX
        MOV ECX,[EAX+4]
        SUB ECX,44                      ;length of entire apic structure.
        MOV EDX,[EAX+36]
        MOV DWORD [EBX + local_apic_address - loader],EDX   ;store local apic address.
        add EAX,44
.test_apic_entries:
        cmp ECX,0
        jz .done
.switch0:
        cmp byte [EAX],0
        jne .next_entry
        inc DWORD [EBX + no_of_cpus - loader]
.next_entry:
        xor EDX,EDX
        MOV dl,byte [EAX+1]
        cmp ECX,EDX
        jl .failed
        add EAX,EDX
        SUB ECX,EDX
        jmp .test_apic_entries
.failed:
        xor EAX,EAX
        ret
.done:
        MOV EAX,1
        ret


;
;   void* alined_alloc(size,alignment)
;
alined_alloc:
        PUSH EBP
        MOV EBP,ESP
        ;MOV EAX,DWORD [EBX + kend_lo - loader]
        add EAX,DWORD [ESP + (4*(2+1))]
        dec EAX
        xor EDX,EDX
        div DWORD [ESP + (4*(2+1))]
        xor EDX,EDX
        mul DWORD [ESP + (4*(2+1))]
        MOV EDX,EAX
        add EDX,DWORD [ESP + (4*(2+0))]
        ;MOV DWORD [EBX + kend_lo - loader],EDX
        xor EDX,EDX
.done:
        MOV ESP,EBP
        POP EBP
        ret 8


;
;   void map_4mb(void* physical_address,void* virtual_address);
;
map_4mb:
        PUSH EBP
        MOV EBP,ESP
        MOV EDI,DWORD [ESP + (4*(2+1))]
        shr EDI,22  ;page table index
        shl EDI,2  ;page table offset
        add EDI,DWORD [EBX + paging_structure - loader]
        or DWORD [EDI],3
        MOV EAX,[EDI]
        AND EAX,0xFFFFFFFFFFFFF000
        MOV EDX,DWORD [ESP + (4*(2+0))]
        shr EDX,22
        shl EDX,22
        or EDX,3
        MOV ECX,1024
.fill_page_table:
        MOV [EAX],EDX
        add EDX,0x1000
        add EAX,4
        dec ECX
        cmp ECX,0
        jnz .fill_page_table
.done:
        MOV ESP,EBP
        POP EBP
        ret 8
data:
        gdt_ptr dd 0
        paging_structure dd 0
        local_apic_address dd 0
        no_of_cpus dd 0
        memory_map dd 0
        memory_map_len dd 0
        idt_ptr dd 0
        rsdp dd 0
align 8
gdt_start:
.gdt_ptr_start:
        .limit dw gdt_start.gdt_end - gdt_start.gdt_entry_start-1
        .base dd gdt_start.gdt_entry_start
.gdt_ptr_end:
.gdt_entry_start:
        .gdt_null dd 0,0
        .gdt_cs dd 0x0000FFFF,0x00CF9A00
        .gdt_ds dd 0x0000FFFF,0x00CF9200
        .gdt_ucs dd 0x0000FFFF,0x00CFFA00
        .gdt_uds dd 0x0000FFFF,0x00CFF200
        .gdt_TIB dd 0x0000FFFF,0x00CFF200
.gdt_end:

;
; Establish a temporary 32-bit GDT and IDT.
;
GDT64:                           ; Global Descriptor Table (64-bit).
    .Null: equ $ - GDT64         ; The null descriptor.
    dw 0                         ; Limit (low).
    dw 0                         ; Base (low).
    db 0                         ; Base (middle)
    db 0                         ; Access.
    db 0                         ; Granularity.
    db 0                         ; Base (high).
    .Code: equ $ - GDT64         ; The code descriptor.
    dw 0                         ; Limit (low).
    dw 0                         ; Base (low).
    db 0                         ; Base (middle)
    db 10011010b                 ; Access (exec/read).
    db 00100000b                 ; Granularity.
    db 0                         ; Base (high).
    .Data: equ $ - GDT64         ; The data descriptor.
    dw 0                         ; Limit (low).
    dw 0                         ; Base (low).
    db 0                         ; Base (middle)
    db 10010010b                 ; Access (read/write).
    db 00000000b                 ; Granularity.
    db 0                         ; Base (high).
    .Pointer:                    ; The GDT-pointer.
    dw $ - GDT64 - 1             ; Limit.
    dq GDT64                     ; Base.

pIDE64: ; Used by LGDT.
	dw IDT64.end-IDT64 ; GDT limit ...
	dd IDT64 ; and 32-bit GDT base
IDT64:
	dd 0,0
	dd 0x0000FFFF,0x00CF9A00
	dd 0x0000FFFF,0x00CF9200
.end:

align 0x1000
stack:
	resb STACKSIZE
section .data
;
; Pointer to end of kernel.
;
g_qKernelEnd:
	kend_lo dd 0
	kend_hi dd 0

;
; long mode temporary page table.
; 1GB of identity mapped memory
;
align 0x1000
PML4T:
	dq PDPT + 0x03 - 0xC0000000
	dq 0
align 0x1000
PDPT:
	dq PDT + 0x03 - 0xC0000000
	dq 0x00000000
	dq 0x00000000
	dq PDT.PDT3GB + 0x03 - 0xC0000000
	resb 0x1000 - ($ - PDPT)
align 0x1000
PDT:
	dq 0x00000083
	dq 0x00200083
	dq 0x00400083
	dq 0x00600083
	resb 0x3000 - ($ - PDT)
.PDT3GB:
	dq 0x00000083
	dq 0x00200083
	dq 0x00400083
	dq 0x00600083
	resb 0x1FC000 - ($ - .PDT3GB)