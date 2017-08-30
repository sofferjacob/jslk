#include "hal.h"
#include <stdint.h>

static mode_t interrupt_status;

uint8_t _interrupts(mode_t mode) {
    if (mode == on) {
        if (interrupt_status != on) {
            asm volatile ("sti");
            interrupt_status = on;
        }
    } else if (mode == off) {
        if (interrupt_status != off) {
            asm volatile ("cli");
            interrupt_status = off;
        } else {
            kprint("ERROR: Cannot disable interrupts (they are already disabled) \n");
            return 1;
        }
    }
    return 0;
}

void _halt() {
    asm volatile ("hlt");
}

void _syscritical(mode_t mode) {
    uint8_t previous_interrupt_status = interrupt_status;
    if (mode == on) {
        if (interrupt_status != off) {
            _interrupts(off);
        }
    } else if (mode == off) {
        if (previous_interrupt_status != off) {
            _interrupts(on);
        }
    }
}

mode_t readInterruptStatus() {
    return interrupt_status;
}

void system_panic(string msg) {
    uint8_t panicColor = getColor(vga_light_grey, vga_blue);
    setConsoleColor(panicColor);
    clear_screen();
    kprint("KERNEL PANIC \n");
    kprint("There was an error and JLK had to halt. Restart your device. \n");
    kprint("Error message: ");
    kprint(msg);
    _halt();
}