#include <stdint.h>
#include "timer.h"
#include "global.h"
#include "multitask.h"

void apic_timer_callback(registers_t* reg)
{
    static uint32_t tick = 0;
    tick++;
    thread_info_t *thread = 0;
    /*
     * Save context.
     */
    __asm__ volatile("mov %%fs:(0x0),%0;" : "=r"(thread));
    thread->context = *reg;
    /*
     * Get next thread.
     */
    printf("\nNext thread (%x)", thread);
    if (thread->next_thread)
        thread = thread->next_thread;

    /*
     * Load context.
     */

    *reg = thread->context;
    __asm__ volatile("mov %0,%%fs:(0x0);" ::"r"(thread));
}

void init_timer(uint32_t frequency)
{
    // Firstly, register our timer callback.
    //register_interrupt_handler(IRQ(0), &timer_callback);
    /*
    // The value we send to the PIT is the value to divide it's input clock
    // (1193180 Hz) by, to get our required frequency. Important to note is
    // that the divisor must be small enough to fit into 16-bits.
    uint32_t divisor = 1193180 / frequency;

    // Send the command byte.
    outb(0x43, 0x36);

    // Divisor has to be sent byte-wise, so split here into upper/lower bytes.
    uint8_t l = (uint8_t) (divisor & 0xFF);
    uint8_t h = (uint8_t) ((divisor >> 8) & 0xFF);

    // Send the frequency divisor.
    outb(0x40, l);
    outb(0x40, h);
     */
}
