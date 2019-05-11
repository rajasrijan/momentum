/*
 * Copyright 2009-2019 Srijan Kumar Sharma
 * 
 * This file is part of Momentum.
 * 
 * Momentum is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * Momentum is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with Momentum.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _DESCRIPTOR_TABLE_H_
#define _DESCRIPTOR_TABLE_H_
#include <stdint.h>
 // This structure contains the value of one GDT entry.
 // We use the attribute 'packed' to tell GCC not to change
 // any of the alignment in the structure.
#define GDT_COUNT (8)

struct gdt_entry_struct
{
	uint16_t limit_low; // The lower 16 bits of the limit.
	uint16_t base_low; // The lower 16 bits of the base.
	uint8_t base_middle; // The next 8 bits of the base.
	uint8_t access; // Access flags, determine what ring this segment can be used in.
	uint8_t granularity;
	uint8_t base_high; // The last 8 bits of the base.
} __attribute__((packed));

struct gdt_ptr_struct
{
	uint16_t limit; // The upper 16 bits of all selector limits.
	uint64_t base; // The address of the first gdt_entry_t struct.
} __attribute__((packed));

struct gdt_struct
{
	struct gdt_ptr_struct gp; // The upper 16 bits of all selector limits.
	struct gdt_entry_struct ge[GDT_COUNT]; // The address of the first gdt_entry_t struct.
} __attribute__((packed));

typedef volatile struct gdt_ptr_struct gdt_ptr_t;
typedef volatile struct gdt_entry_struct gdt_entry_t;
typedef struct gdt_struct gdt_t;

// A struct describing an interrupt gate.

struct idt_entry_struct
{
	uint16_t base_lo_lo; // The lower 16 bits of the address to jump to when this interrupt fires.
	uint16_t sel; // Kernel segment selector.
	uint8_t ist; // bits 0..2 holds Interrupt Stack Table offset, rest of bits zero.
	uint8_t flags; // More flags. See documentation.
	uint16_t base_lo_hi; // The upper 16 bits of the address to jump to.
	uint32_t base_hi; // offset bits 32..63
	uint32_t zero;     // reserved
} __attribute__((packed));
typedef volatile struct idt_entry_struct idt_entry_t;

// A struct describing a pointer to an array of interrupt handlers.
// This is in a format suitable for giving to 'lidt'.

struct idt_ptr_struct
{
	uint16_t limit;
	uint64_t base; // The address of the first element in our idt_entry_t array.
} __attribute__((packed));
typedef volatile struct idt_ptr_struct idt_ptr_t;

struct idt_struct
{
	struct idt_ptr_struct ip;
	struct idt_entry_struct ie[256]; // The address of the first gdt_entry_t struct.
} __attribute__((packed));
typedef struct idt_struct idt_t;

#define IRQ(x) (38+((uint32_t)(x)))


void init_idt(void);
void idt_set_gate(idt_entry_t *idt_entries, uint8_t num, uint64_t base, uint16_t sel, uint8_t flags);

// These extern directives let us access the addresses of our ASM ISR handlers.
/*
 * Intel reserved interrupts.
 * start
 */
extern "C"
{
	extern void isr0(void);
	extern void isr1(void);
	extern void isr2(void);
	extern void isr3(void);
	extern void isr4(void);
	extern void isr5(void);
	extern void isr6(void);
	extern void isr7(void);
	extern void isr8(void);
	extern void isr9(void);
	extern void isr10(void);
	extern void isr11(void);
	extern void isr12(void);
	extern void isr13(void);
	extern void isr14(void);
	extern void isr15(void);
	extern void isr16(void);
	extern void isr17(void);
	extern void isr18(void);
	extern void isr19(void);
	extern void isr20(void);
	extern void isr21(void);
	extern void isr22(void);
	extern void isr23(void);
	extern void isr24(void);
	extern void isr25(void);
	extern void isr26(void);
	extern void isr27(void);
	extern void isr28(void);
	extern void isr29(void);
	extern void isr30(void);
	extern void isr31(void);
	/*
	 * Intel reserved interrupts.
	 * End
	 */
	extern void isr32(void);
	extern void isr33(void);
	extern void isr34(void);
	extern void isr35(void);
	extern void isr36(void);
	extern void isr37(void);
	extern void isr38(void);
	extern void isr39(void);
	extern void isr40(void);
	extern void isr41(void);
	extern void isr42(void);
	extern void isr43(void);
	extern void isr44(void);
	extern void isr45(void);
	extern void isr46(void);
	extern void isr47(void);
	extern void isr48(void);
	extern void isr49(void);
	extern void isr50(void);
	extern void isr51(void);
	extern void isr52(void);
	extern void isr53(void);
	extern void isr54(void);
	extern void isr55(void);
	extern void isr56(void);
	extern void isr57(void);
	extern void isr58(void);
	extern void isr59(void);
	extern void isr60(void);
	extern void isr61(void);
	extern void isr62(void);
	extern void isr63(void);
	extern void isr64(void);
	extern void isr65(void);
}
void set_gdt_gate(gdt_entry_t *gdt_entries, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran);
#endif
