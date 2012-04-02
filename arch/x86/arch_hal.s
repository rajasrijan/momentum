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

[EXTERN isr_handler]
[EXTERN irq_handler]
[GLOBAL irq_common_stub]
[GLOBAL get_eflags]
[GLOBAL get_cpl]
[GLOBAL switch_context]
[GLOBAL get_spin_lock]
[GLOBAL release_spin_lock]
[GLOBAL get_cr3]
%macro ISR_NOERRCODE 1  ; define a macro, taking one parameter
	[GLOBAL isr%1]        ; %1 accesses the first parameter.
isr%1:
    CLI
    PUSH ESP
    PUSH BYTE %1
    JMP isr_common_stub
%endmacro

%macro ISR_ERRCODE 1
	[GLOBAL isr%1]
isr%1:
    CLI
    PUSH BYTE %1
    JMP isr_common_stub
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
	PUSHA                    ; PUSHES EDI,ESI,EBP,ESP,EBX,EDX,ECX,EAX
	
	MOV AX, DS               ; LOWER 16-BITS OF EAX = DS.
	PUSH EAX                 ; SAVE THE DATA SEGMENT DESCRIPTOR
	
	MOV AX, 0X10  ; LOAD THE KERNEL DATA SEGMENT DESCRIPTOR
	MOV DS, AX
	MOV ES, AX
	;MOV FS, AX FS MUST REMAIN UNCHANGED.
	MOV GS, AX
	
	CALL isr_handler
	
	POP EAX        ; RELOAD THE ORIGINAL DATA SEGMENT DESCRIPTOR
	MOV DS, AX
	MOV ES, AX
	;MOV FS, AX FS MUST REMAIN UNCHANGED.
	MOV GS, AX
	
	POPA                     ; POPS EDI,ESI,EBP...
	ADD ESP, 8     ; CLEANS UP THE PUSHED ERROR CODE AND PUSHED ISR NUMBER
	STI
	IRETD           ; POPS 5 THINGS AT ONCE: CS, EIP, EFLAGS, SS, AND ESP
	
get_eflags:
	pushfd
	POP EAX
	RET
	
switch_context:
	XCHG BX,BX
	MOV ESP,[ESP+4]
	POP DS
	POPA
	IRETD
	
get_cpl:
	XOR EAX,EAX
	PUSH CS
	POP AX
	AND EAX,0X3
	RET
	
atomic_exchange:
	MOV EBX,[ESP+4]
	MOV EAX,[ESP+8]
	MOV [EBX],EAX
	RET
	
get_spin_lock:
	MOV EBX, [ESP+4]
	CMP DWORD [EBX], 0 ;CHECK IF LOCK IS FREE
	JE Get_Lock
	PAUSE ;Short delay
	JMP get_spin_lock
Get_Lock:
	MOV EAX, 1
	XCHG EAX, DWORD [EBX] ;TRY TO GET LOCK
	CMP EAX, 0 ;TEST IF SUCCESSFUL
	JNE get_spin_lock
	RET
	
release_spin_lock:
	MOV EBX,DWORD [ESP+4]
	MOV DWORD [EBX],0
	RET
	
get_cr3:
        MOV EAX,CR3
        RET