#include <hal.h>
#include <types.h>
#include <version.h>

int kernel_main() {
    _interrupts(off);   // We don't have and IDT yet
    halInitialize();

    kprint("Welcome to the JLK Kernel! \n");
    kprint("Copyright (c) 2017 Jacobo Soffer. All Rights Reserved \n");
    kprint("Kernel version: "); kprint(KERNEL_VERSION_STRING); kprint("\n");
    kprint("Kernel API version: "); kprint(JLK_API_VERSION); kprint("\n"); 
    kprint("Build number: "); kprint(KERNEL_BUILD); kprint("\n");
    kprint("Running test for styled strings: \n");
    uint8_t stringColor = getColor(vga_red, vga_green);
    writeStyledString("This is a string with style \n", stringColor);
    kprint("Test concluded, halting the kernel. \n");
    _halt();
}