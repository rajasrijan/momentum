/*
 * Copyright 2009-2020 Srijan Kumar Sharma
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

#ifndef APIC_H
#define APIC_H
#include <stdint.h>

#define APICBASE_RESERVED 0x000006ff
#define APICBASE_BSP 0x00000100
#define APICBASE_ENABLED 0x00000800
#define APICBASE_ADDRESS 0xfffff000

//  APIC interrupt command register defines
#define APIC_ICR_DELIVERY_FIXED 0x000
#define APIC_ICR_DELIVERY_LOWPRIORITY 0x100
#define APIC_ICR_DELIVERY_SMI 0x200
#define APIC_ICR_DELIVERY_RESV0 0x300
#define APIC_ICR_DELIVERY_NMI 0x400
#define APIC_ICR_DELIVERY_INIT 0x500
#define APIC_ICR_DELIVERY_STARTUP 0x600
#define APIC_ICR_DELIVERY_RESV1 0x700

#define APIC_ICR_DESTMODE_PHYSICAL 0x000
#define APIC_ICR_DESTMODE_LOGICAL 0x800

#define APIC_ICR_DELIVERY_STATUS 0x1000

#define APIC_ICR_LEVEL_DEASSERT 0x0000
#define APIC_ICR_LEVEL_ASSERT 0x4000

#define APIC_ICR_TRIGGER_EDGE 0x0000
#define APIC_ICR_TRIGGER_LEVEL 0x8000

#define APIC_ICR_DEST_NONE 0x00000
#define APIC_ICR_DEST_SELF 0x40000
#define APIC_ICR_DEST_ALL_SELF 0x80000
#define APIC_ICR_DEST_ALL_NOSELF 0xC0000

#ifdef __cplusplus
extern "C"
{
#endif
#define LVT_MASKED (1 << 16)

    struct local_apic_structure
    {
        uint8_t resv0[0x20];
        uint32_t apic_id, resv1[0x3], apic_ver, resv2[0x3];
        uint8_t resv47[0x40];
        uint32_t tpr, resv3[0x3], apr, resv4[0x3], ppr, resv5[0x3], eoi, resv6[0x3], rrd, resv7[0x3], logical_dest, resv8[0x3], dest_format, resv9[0x3], svr, resv09[0x3];
        uint32_t isr0, resv10[0x3], isr1, resv11[0x3], isr2, resv12[0x3], isr3, resv13[0x3], isr4, resv14[0x3], isr5, resv15[0x3], isr6, resv16[0x3], isr7, resv17[0x3];
        uint32_t tmp0, resv18[0x3], tmp1, resv19[0x3], tmp2, resv20[0x3], tmp3, resv21[0x3], tmp4, resv22[0x3], tmp5, resv23[0x3], tmp6, resv24[0x3], tmp7, resv25[0x3];
        uint32_t irr0, resv26[0x3], irr1, resv27[0x3], irr2, resv28[0x3], irr3, resv29[0x3], irr4, resv30[0x3], irr5, resv31[0x3], irr6, resv32[0x3], irr7, resv33[0x3];
        uint32_t err_stat, resv34[0x3];
        uint8_t resv02[0x60];
        uint32_t lvt_cmcl, resv35[0x3], icr_lo, resv36[0x3], icr_hi, resv37[0x3], lvt_timer, resv38[0x3], lvt_thermal, resv39[0x3], lvt_perf, resv40[0x3], lvt_lint0, resv41[0x3], lvt_lint1, resv42[0x3], lvt_err, resv43[0x3], init_count,
            resv44[0x3], curr_count, resv45[0x3];
        uint8_t resv03[0x40];
        uint32_t div_conf, resv46[0x3];
        uint8_t resv04[0x10];
    } __attribute__((packed));

    struct io_apic_structure
    {
        uint32_t ioregsel, resv0[3];
        uint32_t iowin, resv1[3];
        uint32_t version;
    } __attribute__((packed));
    typedef volatile struct io_apic_structure ioapic_t;
    extern ioapic_t *ioapic;
    void init_apic_timer(uint32_t frequency);
    void init_ioapic(void);
    void write_ioapic(uint32_t offset, uint32_t value);
    uint32_t read_ioapic(uint32_t offset);
    void apic_pin_enable(uint32_t pin);
    void send_eoi(void);
#ifdef __cplusplus
}
#endif

/**
 * @brief Send INIT interrut to destination cpu. This function is only used for AP startup.
 * 
 * @param isShorthand Set to true to indicate 'dest' is shorthand address, false to indicate full address.
 * @param dest Destination address
 */
void apic_interrupt_command_init(bool isShorthand, uint64_t dest);

/**
 * @brief Send STARTUP interrut to destination cpu. This function is only used for AP startup.
 * 
 * @param isShorthand Set to true to indicate 'dest' is shorthand address, false to indicate full address.
 * @param dest Destination address
 * @param vector 
 */
void apic_interrupt_command_sipi(bool isShorthand, uint64_t dest, uint8_t vector);

/**
 * @brief Initialize symmetric muliprocessor
 * 
 */
int init_symmetric_multi_processor();

#endif /* APIC_H */
