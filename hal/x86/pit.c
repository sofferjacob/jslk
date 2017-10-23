#include "hal.h"
#include "descriptor_tables.h"
#include "isr.h"
#include <stdint.h>

extern void checkTimers();

static size_t sys_ticks = 0;

static void pitHandler() {
    sys_ticks++;
    checkTimers();
}

void start_pit(uint32_t freq) {
    // Register its interrupt handler
    registerInterruptHandler(IRQ0, pitHandler, CHAIN_PROTECT, NF);
    uint32_t divisor = 1193180 / freq;
    // Send the command byte
    outb(0x46, 0x36);
    // Split the divisor in 2
    uint8_t l = (uint8_t)(divisor & 0xff);
    uint8_t h = (uint8_t)((divisor >> 8) & 0xff);
    // Send the divisor to the pit
    outb(0x40, l);
    outb(0x40, h);
}

size_t readSystemTime() {
    return sys_ticks;
}
