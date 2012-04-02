;
; Copyright 2009-2012 Srijan Kumar Sharma
;
; This file is part of Momentum.
;
; Momentum is free software: you can redistribute it and/or modify
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
[extern kernel_start]
[extern kernel_end]
[extern sys_info]
[extern stage2]

; setting up the Multiboot header - see GRUB docs for details
MODULEALIGN equ  1<<0                   ; align loaded modules on page boundaries
MEMINFO     equ  1<<1                   ; provide memory map
FLAGS       equ  MODULEALIGN | MEMINFO  ; this is the Multiboot 'flag' field
MAGIC       equ    0x1BADB002           ; 'magic number' lets bootloader find the header
CHECKSUM    equ -(MAGIC + FLAGS)        ; checksum required

section .text
align 4


MultiBootHeader:
   dd MAGIC
   dd FLAGS
   dd CHECKSUM
STACKSIZE equ 0x1000                  ; that's 4k.

loader:
	call near .locator
.locator:
	pop ecx								;got the return address;
	sub ecx,5							;address of loader.
        xchg ebx,ecx    ;ebx has multiboot info structure.
	lea esp,[ebx + (stack +STACKSIZE )]
	sub esp,loader						;absolute location of stack.
	mov eax,kernel_end
        sub eax,loader
        add eax,ebx
	mov dword [ebx + kend - loader],eax
.copy_memory_map_structure_from_multiboot_loader:
        mov esi,[ecx+48]
        mov ecx,[ecx+44]
        push esi
        push ecx
        push dword 1
        push dword ecx
        call alined_alloc
        mov dword [ebx + memory_map - loader],eax        
        mov edi,eax
        pop ecx
        pop esi
        mov dword [ebx + memory_map_len - loader],ecx
        rep movsb
        
	call parse_acpi_tables
        mov eax,dword [ebx + no_of_cpus - loader]
        xor edx,edx
        mov ecx,gdt_start.gdt_end - gdt_start
        mul ecx
        push dword 4096
        push dword eax
        call alined_alloc
        mov dword [ebx + gdt_ptr - loader],eax
        mov edi,eax
        mov ecx,dword [ebx + no_of_cpus - loader]
.copy_gdt:
        push ecx
        lea esi,[ebx + gdt_start - loader]
        mov ecx,gdt_start.gdt_end - gdt_start
        rep movsb
        pop ecx
        dec ecx
        cmp ecx,0
        jnz .copy_gdt
        mov eax,[ebx + no_of_cpus - loader]
        xor edx,edx
        mov ecx,gdt_start.gdt_end - gdt_start
        mul ecx
        dec eax
        xchg eax,ecx
        mov eax,dword [ebx + gdt_ptr - loader]
        mov dword [eax],ecx
        lea ecx,[eax+6]
        mov dword [eax+2],ecx
        lgdt [eax]
        jmp 0x08:(.reload_cs - 0xC0000000)
.reload_cs:
        MOV   AX, 0x10 ; 0x10 points at the new data selector
        MOV   DS, AX
        MOV   ES, AX
        MOV   FS, AX
        MOV   GS, AX
        MOV   SS, AX
.allocate_idt:
        push dword 1
        push dword (6 + (256*8))
        call alined_alloc
        mov dword [ebx + idt_ptr- loader],eax
.start_paging:
        mov ecx,5*1024*1024     ;5MB for page table.
        mov eax,[ebx + no_of_cpus - loader]
        mul ecx                 ;page table for each cpu.
        push dword 4096
        push dword eax
        call alined_alloc
        mov dword [ebx + paging_structure - loader],eax
        mov ecx,1024
        mov edi,eax
        add eax,1024*1024
.fill_pagedirectory:
        stosd
        add eax,0x1000
        dec ecx
        cmp ecx,0
        jnz .fill_pagedirectory
        mov eax,dword [ebx + paging_structure - loader]
        mov ecx,1024*1024
        add eax,1024*1024
        mov edi,eax
        xor eax,eax
.fill_pagetable:
        stosd
        add eax,0x1000
        dec ecx
        cmp ecx,0
        jnz .fill_pagetable
        mov eax,[ebx + kend - loader]
        sub eax,kernel_start
		add eax,loader
        sub eax,ebx
        add eax,(0x400000 - 1)
        xor edx,edx
        mov ecx,0x400000
        div ecx
        xor ecx,ecx
        xchg eax,ecx
.allocate_pages:
        push ecx
        push eax
;
; create stack for both the processes.
;
        push eax
        push eax
        push eax
        push eax
        add dword [esp + 4],0xC0000000
        call map_4mb
        call map_4mb
        

        pop eax
        pop ecx
        add eax,0x400000
        dec ecx
        cmp ecx,0
        jnz .allocate_pages
        mov eax,dword [ebx + paging_structure - loader]
        mov cr3,eax
        mov eax,cr0
        or eax,0x80000000
        mov cr0,eax
        push 1
        push dword 0xC0000000
        call alined_alloc
.fill_system_info_table:
        mov eax,dword [ebx + no_of_cpus - loader]
        mov dword [sys_info+0],eax
        mov eax,dword [ebx + gdt_ptr - loader]
        mov dword [sys_info+4],eax
        mov eax,dword [ebx + paging_structure - loader]
        mov dword [sys_info+8],eax
        mov eax,dword [ebx + memory_map - loader]
        mov dword [sys_info+12],eax
        mov eax,dword [ebx + memory_map_len - loader]
        mov dword [sys_info+16],eax
        mov eax,dword [ebx + kend - loader]
        mov dword [sys_info+20],eax
        mov eax,dword [ebx + local_apic_address - loader]
        mov dword [sys_info+24],eax
        mov eax,dword [ebx + idt_ptr - loader]
        mov dword [sys_info+28],eax
        mov eax,dword [ebx + rsdp - loader]
        mov dword [sys_info+32],eax
.fix_referances:
        jmp 0x08:.jump_to_main_kernel
.jump_to_main_kernel:
        mov esp,STACKSIZE+stack
        call stage2
hang:
        cli;
	hlt;
find_acpi:
	mov eax,0xE0000
.loop:
        cmp dword [eax],'RSD '
        jne .continue
        cmp dword [eax+4],'PTR '
        je .done
.continue:
        cmp eax,0xFFFFF
        jge .failed
        add eax,2
        jmp .loop
.failed:
        xor eax,eax
.done:
        ret


parse_acpi_tables:
        call find_acpi                  ;got rsdp.
        cmp eax,0
        jz .failed
        mov dword [ebx + rsdp - loader],eax
        mov eax,[eax+16]                ;got rsdt.
        cmp dword [eax],'RSDT'
        jne .failed
        mov ecx,[eax+4]
        sub ecx,36                      ;length of descriptor tables.
        shr ecx,2                       ;divide by 4.
        add eax,36
.test_table:
        cmp ecx,0
        jz .failed
        mov edx,[eax]
        cmp dword [edx],'APIC'
        je .found_madt
        add eax,4
        dec ecx
        jmp .test_table
.found_madt:
        mov eax,edx
        mov ecx,[eax+4]
        sub ecx,44                      ;length of entire apic structure.
        mov edx,[eax+36]
        mov dword [ebx + local_apic_address - loader],edx   ;store local apic address.
        add eax,44
.test_apic_entries:
        cmp ecx,0
        jz .done
.switch0:
        cmp byte [eax],0
        jne .next_entry
        inc dword [ebx + no_of_cpus - loader]
.next_entry:
        xor edx,edx
        mov dl,byte [eax+1]
        cmp ecx,edx
        jl .failed
        add eax,edx
        sub ecx,edx
        jmp .test_apic_entries
.failed:
        xor eax,eax
        ret
.done:
        mov eax,1
        ret


;
;   void* alined_alloc(size,alignment)
;
alined_alloc:
        push ebp
        mov ebp,esp
        mov eax,dword [ebx + kend - loader]
        add eax,dword [esp + (4*(2+1))]
        dec eax
        xor edx,edx
        div dword [esp + (4*(2+1))]
        xor edx,edx
        mul dword [esp + (4*(2+1))]
        mov edx,eax
        add edx,dword [esp + (4*(2+0))]
        mov dword [ebx + kend - loader],edx
        xor edx,edx
.done:
        mov esp,ebp
        pop ebp
        ret 8


;
;   void map_4mb(void* physical_address,void* virtual_address);
;
map_4mb:
        push ebp
        mov ebp,esp
        mov edi,dword [esp + (4*(2+1))]
        shr edi,22  ;page table index
        shl edi,2  ;page table offset
        add edi,dword [ebx + paging_structure - loader]
        or dword [edi],3
        mov eax,[edi]
        and eax,0xFFFFF000
        mov edx,dword [esp + (4*(2+0))]
        shr edx,22
        shl edx,22
        or edx,3
        mov ecx,1024
.fill_page_table:
        mov [eax],edx
        add edx,0x1000
        add eax,4
        dec ecx
        cmp ecx,0
        jnz .fill_page_table
.done:
        mov esp,ebp
        pop ebp
        ret 8

data:
        gdt_ptr dd 0
        paging_structure dd 0
        local_apic_address dd 0
        no_of_cpus dd 0
        memory_map dd 0
        memory_map_len dd 0
	kend dd 0
        idt_ptr dd 0
        rsdp dd 0
gdt_start:
.gdt_ptr_start:
        .limit dw 0
        .base dd 0
.gdt_ptr_end:
        .gdt_null dd 0,0
        .gdt_cs dd 0x0000FFFF,0x00CF9A00
        .gdt_ds dd 0x0000FFFF,0x00CF9200
        .gdt_ucs dd 0x0000FFFF,0x00CFFA00
        .gdt_uds dd 0x0000FFFF,0x00CFF200
        .gdt_TIB dd 0x0000FFFF,0x00CFF200
.gdt_end:

section .bss
stack:
	resb STACKSIZE
