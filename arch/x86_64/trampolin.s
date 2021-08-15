;
; Copyright 2009-2021 Srijan Kumar Sharma
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

[bits 16]

org 1000h
section .text
trampolin:
    cli
    mov sp, stack + 0x200
    lgdt [gdtr]
    mov eax, cr0 
    or al, 1        
    mov cr0, eax
    jmp 08h:PModeMain
[bits 32]
PModeMain:
    hlt
section .data
gdtr:
    .size dw (.gdtr_end - .gdt -1)
    .offset dd .gdt
.gdt:
    .gdt_zero dq 0x0
    .gdt_cs dq 0x00CF9A000000FFFF
    .gdt_ds dq 0x00CF92000000FFFF
.gdtr_end:
section .rdata
align 0x200
stack:
TIMES 0x200 db 0
