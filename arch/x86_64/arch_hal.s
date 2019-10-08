;
; Copyright 2009-2019 Srijan Kumar Sharma
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
[bits 64]
[EXTERN isr_handler]
[extern syscall]
[GLOBAL irq_common_stub]
[GLOBAL get_rflags]
[GLOBAL get_cpl]
[GLOBAL switch_context]
[GLOBAL get_spin_lock]
[GLOBAL get_async_spin_lock]
[GLOBAL release_spin_lock]
[GLOBAL get_cr2]
[GLOBAL get_cr3]
[GLOBAL outb]
[GLOBAL outl]
[GLOBAL outw]
[GLOBAL inb]
[GLOBAL inw]
[GLOBAL inl]
[GLOBAL atomic_increment]
[GLOBAL load_interrupt_descriptor_table]
[GLOBAL get_gdt]
[GLOBAL system_call]

%macro ISR_NOERRCODE 1  ; define a macro, taking one parameter
	[GLOBAL isr%1]        ; %1 accesses the first parameter.
isr%1:
    cli
	push QWORD 0x00
    push QWORD %1
	push QWORD 0x00; zero padding
    jmp isr_common_stub
%endmacro

%macro ISR_ERRCODE 1
	[GLOBAL isr%1]
isr%1:
    cli
	push QWORD %1
	push QWORD 0x00; zero padding
    jmp isr_common_stub
%endmacro 

%macro POP_ALL_REGISTERS 0
	FXRSTOR [RSP]
	ADD RSP, 512
    pop r15
	pop r14
	pop r13
	pop r12
	pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rsi
	pop rbp
	add rsp, 8
	pop rbx
	pop rdx
	pop rcx
	pop rax
%endmacro 
%macro PUSH_ALL_REGISTERS 0
    push rax
	push rcx
	push rdx
	push rbx
	push rsp
	push rbp
	push rsi
	push rdi
	push r8
	push r9
	push r10
	push r11
	push r12
	push r13
	push r14
	push r15
	SUB RSP, 512
	FXSAVE [RSP]
%endmacro 
	
ISR_NOERRCODE 0
ISR_NOERRCODE 1
ISR_NOERRCODE 2
ISR_NOERRCODE 3
ISR_NOERRCODE 4
ISR_NOERRCODE 5
ISR_NOERRCODE 6
ISR_NOERRCODE 7
ISR_ERRCODE 8
ISR_NOERRCODE 9
ISR_ERRCODE 10
ISR_ERRCODE 11
ISR_ERRCODE 12
ISR_ERRCODE 13
ISR_ERRCODE 14
ISR_NOERRCODE 15
ISR_NOERRCODE 16
ISR_NOERRCODE 17
ISR_NOERRCODE 18
ISR_NOERRCODE 19
ISR_NOERRCODE 20
ISR_NOERRCODE 21
ISR_NOERRCODE 22
ISR_NOERRCODE 23
ISR_NOERRCODE 24
ISR_NOERRCODE 25
ISR_NOERRCODE 26
ISR_NOERRCODE 27
ISR_NOERRCODE 28
ISR_NOERRCODE 29
ISR_NOERRCODE 30
ISR_NOERRCODE 31
; This is IRQ definations
ISR_NOERRCODE 32    ;LVT Timer
ISR_NOERRCODE 33    ;LVT Thermal Sensor
ISR_NOERRCODE 34    ;LVT Performance Monitor
ISR_NOERRCODE 35    ;LVT LINT0
ISR_NOERRCODE 36    ;LVT LINT1
ISR_NOERRCODE 37    ;LVT Error
ISR_NOERRCODE 38
ISR_NOERRCODE 39
ISR_NOERRCODE 40
ISR_NOERRCODE 41
ISR_NOERRCODE 42
ISR_NOERRCODE 43
ISR_NOERRCODE 44
ISR_NOERRCODE 45
ISR_NOERRCODE 46
ISR_NOERRCODE 47
ISR_NOERRCODE 48
ISR_NOERRCODE 49
ISR_NOERRCODE 50
ISR_NOERRCODE 51
ISR_NOERRCODE 52
ISR_NOERRCODE 53
ISR_NOERRCODE 54
ISR_NOERRCODE 55
ISR_NOERRCODE 56
ISR_NOERRCODE 57
ISR_NOERRCODE 58
ISR_NOERRCODE 59
ISR_NOERRCODE 60
ISR_NOERRCODE 61
ISR_NOERRCODE 62
ISR_NOERRCODE 63
ISR_NOERRCODE 64

; This is our common ISR stub. It saves the processor state, sets
; up for kernel mode segments, calls the C-level fault handler,
; and finally restores the stack frame.
isr_common_stub:
	PUSH_ALL_REGISTERS
	mov rax,0x10
	mov ds,ax
	mov es,ax
	mov ss,ax
	mov fs,ax
	mov gs,ax
	mov rdi,rsp
	add rdi,0x280
	mov rsi,rsp
	call isr_handler
	POP_ALL_REGISTERS
	add rsp,0x18
	iretq
	
get_rflags:
	push rbp
	mov rbp,rsp
	pushfq
	pop rax
	leave
	ret
	
switch_context:
	mov rax,rsi
	mov ds,ax
	mov rsp,rdi
	POP_ALL_REGISTERS
	add rsp,0x18
	iretq

system_call:
	push rdx
	push rcx
	push r11
	call syscall
	pop r11
	pop rcx
	pop rdx
	o64 sysret

atomic_exchange:
	mov rbx,[rsp+4]
	mov rax,[rsp+8]
	mov [rbx],rax
	ret
	
get_spin_lock:
.get_Lock:
	mov rax, 1
	xchg rax, QWORD [rdi] ;TRY TO GET LOCK
	test rax, 1 ;TEST IF SUCCESSFUL
	jz .done
	jmp .get_Lock
.done:
	ret				;return

get_async_spin_lock:
	mov rax, 1
	xchg rax, QWORD [rdi] ;TRY TO GET LOCK
	ret				;return

	
release_spin_lock:
	mov rax,QWORD [rdi]
	AND rax,0xFFFFFFFFFFFFFFFE
	xchg rax, QWORD [rdi]
	ret
	
get_cr2:
        mov rax,CR2
        ret

get_cr3:
        mov rax,CR3
        ret

;extern void outb(unsigned short port, unsigned char val)
;rdi=port
;rsi=val
outb:
	push rdx
	push rax
	mov rdx,rdi
	mov rax,rsi
	out dx, al
	pop rax
	pop rdx
	ret

;extern void outw(unsigned short port, uint16_t val);
;rdi=port
;rsi=val
outw:
	push rdx
	push rax
	mov rdx,rdi
	mov rax,rsi
	out dx, ax
	pop rax
	pop rdx
	ret

;extern void outl(unsigned short port, uint32_t val);
;rdi=port
;rsi=val
outl:
	push rdx
	push rax
	mov rdx,rdi
	mov rax,rsi
	out dx, eax
	pop rax
	pop rdx
	ret
;extern uint8_t inb(uint16_t port);
;rdi=port
inb:
	push rdx
	mov rdx,rdi
	in al, dx
	pop rdx
	ret
;extern uint16_t inw(uint16_t port);
;rdi=port
inw:
	push rdx
	mov rdx,rdi
	XOR RAX, RAX
	in ax, dx
	pop rdx
	ret
;extern uint32_t inl(uint16_t port);
;rdi=port
inl:
	push rdx
	mov rdx,rdi
	in eax, dx
	pop rdx
	ret

;extern int64_t atomic_increment(int64_t *val);
atomic_increment:
	mov rax, 1
	lock xadd [rdi], rax
	ret

;rdi=pointer
load_interrupt_descriptor_table:
	PUSH RBX
	MOV RBX, RDI
	LIDT [RBX]
	POP RBX
	CLI
	RET

;extern void inl(uint64_t pGdt);
;rdi=gdt pointer
get_gdt:
	mov RAX,RDI
	sgdt [RAX]
	ret