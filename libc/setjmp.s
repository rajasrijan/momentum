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

[global _setjmp]
[global _longjmp]

_setjmp:
    MOV [RDI], RBX
    MOV [RDI+8], RSP
    MOV [RDI+16], RBP
    MOV [RDI+24], R12
    MOV [RDI+32], R13
    MOV [RDI+40], R14
    MOV [RDI+48], R15
    MOV RAX,[RSP]
    MOV [RDI+56], RAX
    XOR RAX, RAX
    RET

_longjmp:
    MOV RBX,[RDI]
    MOV RSP,[RDI+8]
    MOV RBP,[RDI+16]
    MOV R12,[RDI+24]
    MOV R13,[RDI+32]
    MOV R14,[RDI+40]
    MOV R15,[RDI+48]
    MOV RAX,[RDI+56]
    MOV [RSP],RAX
    MOV RAX, RSI
    RET