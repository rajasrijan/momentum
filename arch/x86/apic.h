/* 
 * File:   apic.h
 * Author: Srijan
 *
 * Created on June 27, 2011, 8:19 PM
 */

#ifndef APIC_H
#define	APIC_H
#include "global.h"
#ifdef	__cplusplus
extern "C"
{
#endif
#define LVT_MASKED (1<<16)

struct local_apic_structure
{
    uint8_t resv0[0x20];
    volatile uint32_t apic_id, resv1[0x3], apic_ver, resv2[0x3];
    uint8_t resv47[0x40];
    volatile uint32_t tpr, resv3[0x3], apr, resv4[0x3], ppr, resv5[0x3], eoi, resv6[0x3], rrd, resv7[0x3], logical_dest, resv8[0x3], dest_format, resv9[0x3], svr, resv09[0x3];
    volatile uint32_t isr0, resv10[0x3], isr1, resv11[0x3], isr2, resv12[0x3], isr3, resv13[0x3], isr4, resv14[0x3], isr5, resv15[0x3], isr6, resv16[0x3], isr7, resv17[0x3];
    volatile uint32_t tmp0, resv18[0x3], tmp1, resv19[0x3], tmp2, resv20[0x3], tmp3, resv21[0x3], tmp4, resv22[0x3], tmp5, resv23[0x3], tmp6, resv24[0x3], tmp7, resv25[0x3];
    volatile uint32_t irr0, resv26[0x3], irr1, resv27[0x3], irr2, resv28[0x3], irr3, resv29[0x3], irr4, resv30[0x3], irr5, resv31[0x3], irr6, resv32[0x3], irr7, resv33[0x3];
    volatile uint32_t err_stat, resv34[0x3];
    uint8_t resv02[0x60];
    volatile uint32_t lvt_cmcl, resv35[0x3], icr_lo, resv36[0x3], icr_hi, resv37[0x3], lvt_timer, resv38[0x3], lvt_thermal, resv39[0x3], lvt_perf, resv40[0x3], lvt_lint0, resv41[0x3], lvt_lint1, resv42[0x3], lvt_err, resv43[0x3], init_count, resv44[0x3], curr_count, resv45[0x3];
    uint8_t resv03[0x40];
    volatile uint32_t div_conf, resv46[0x3];
    uint8_t resv04[0x10];
} __attribute__((packed));
typedef struct local_apic_structure lapic_t;

struct io_apic_structure
{
    volatile uint32_t ioregsel, resv0[3];
    volatile uint32_t iowin, resv1[3];
    volatile uint32_t version;
} __attribute__((packed));
typedef struct io_apic_structure ioapic_t;
extern lapic_t* lapic;
extern ioapic_t* ioapic;
void init_apic_timer(uint32_t frequency);
void init_ioapic(void);
void write_ioapic(uint32_t offset, uint32_t value);
uint32_t read_ioapic(uint32_t offset);
void apic_pin_enable(uint32_t pin);
#ifdef	__cplusplus
}
#endif

#endif	/* APIC_H */

