#include <hal.h>
#include <types.h>
#include <version.h>
#include <timer.h>

void testHandler();
void timerHandler();

int kernel_main() {
    _interrupts(off);   // We don't have and IDT yet
    halInitialize();

    kprint("Welcome to the JSLK Kernel! \n");
    kprint("Copyright (c) 2017 Jacobo Soffer. All Rights Reserved \n");
    kprint("Kernel version: "); kprint(KERNEL_VERSION_STRING); kprint("\n");
    kprint("Kernel API version: "); kprint(JSLK_API_VERSION); kprint("\n"); 
    kprint("Build number: "); kprint(KERNEL_BUILD); kprint("\n");
    kprint("Running test for styled strings: \n");
    uint8_t stringColor = getColor(vga_red, vga_green);
    writeStyledString("This is a string with style \n", stringColor);
    kprint("Test concluded, running test for interrupts \n");
    registerInterruptHandler(3, testHandler);
    genInterrupt(3);
    genInterrupt(4);
    kprint("Test concluded, running test for a 15 second timer \n");
    start_timer(15.0, timerHandler);
    kprint("Test concluded, running test for a 2 second delay \n");
    delay(2);
    kprint("Test concluded, halting the kernel \n");
    _halt();
}

void testHandler() {
    kprint("Inside Handler \n");
}

void timerHandler() {
    kernelPrintDec(readSystemTime()); kprint("\n");
}