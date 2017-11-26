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
    _interrupts(off);
    asm volatile ("hlt");
    _halt(); // In case an interrupt occurs.
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
    setMenubarText("Warning!");
    #ifndef KERNEL_DEBUG
    clear_console();
    #endif
    kprint("KERNEL PANIC \n");
    kprint("An error ocurred and JSLK had to halt. Restart your device. \n");
    kprint("You may submit a bug report at: https://github.com/sofferjacob/jslk/issues \n");
    kprint("Error message: ");
    kprint(msg);
    _halt();
}

void full_system_panic(string msg, string file, uint32_t line) {
    atomicalStart();
    uint8_t panicColor = getColor(vga_light_grey, vga_blue);
    setConsoleColor(panicColor);
    #ifndef KERNEL_DEBUG
    clear_console();
    #endif
    kprint("KERNEL PANIC \n");
    kprint("An error ocurred and JSLK had to halt. Restart your device. \n");
    kprint("You may submit a bug report at: https://github.com/sofferjacob/jslk/issues \n");
    kprint("Error message: ");
    kprintf("%s \n", msg);
    #ifdef KERNEL_DEBUG
    kprintf("File %s, line %i", file, line);
    #endif
    _halt();
}