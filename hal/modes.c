#include "hal.h"
#include <stdint.h>

uint8_t _interrupts(mode_t mode) {
   if (mode == on) {
       asm volatile ("sti");
   } else if (mode == off) {
       asm volatile ("cli");
   }
    return 0;
}

void _halt() {
    asm volatile ("hlt");
}

void _syscritical(mode_t mode) {
    if (mode == on) {
        _interrupts(off);
    } else if (mode == off) {
        _interrupts(on);
    }
}

void system_panic(string msg) {
    atomicalStart();
    uint8_t panicColor = getColor(vga_light_grey, vga_blue);
    setConsoleColor(panicColor);
    clear_console();
    kprint("KERNEL PANIC \n");
    kprint("There was an error and JLK had to halt. Restart your device. \n");
    kprint("Error message: ");
    kprint(msg);
    _halt();
}