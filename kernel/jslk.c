#include <hal.h>
#include <types.h>
#include <version.h>
#include <timer.h>
#include "multiboot.h"

void testHandler();
void timerHandler();

extern unsigned int* end;
extern unsigned int* start;
size_t getSizeOfKernel() {
    return ((size_t)(&start) + 1) - ((size_t)(&end));
}

struct memregion
{
    uint32_t startLo;
    uint32_t startHi;
    uint32_t sizeLo;
    uint32_t sizeHi;
    uint32_t type;
    uint32_t acpi_3_0;
};

typedef struct memregion memregion_t;

string memTypes[] = {
    "Available",
    "Reserved",
    "ACPI Reclaim",
    "ACPI NVS Memory"
};

int kernel_main(multiboot_t* bootinfo) {
    _interrupts(off);   // We don't have and IDT yet
    halInitialize();

    kprint("Welcome to the JSLK Kernel! \n");
    kprint("Copyright (c) 2017 Jacobo Soffer. All Rights Reserved \n");
    kprintf("Kernel version: %i.%i.%i-%s \n", KERNEL_VERSION_MAJOR, KERNEL_VERSION_MINOR, KERNEL_VERSION_RELEASE, KERNEL_RELEASE_TYPE);
    kprint("Kernel API version: "); kprint(JSLK_API_VERSION); kprint("\n"); 
    kprint("Build number: "); kprint(KERNEL_BUILD); kprint("\n");
    kprint("Running test for styled strings: \n");
    uint8_t stringColor = getColor(vga_red, vga_green);
    kprintf("test, %i", 21);
    writeStyledString("This is a string with style \n", stringColor);
    kprint("Test concluded, running test for interrupts \n");
    registerInterruptHandler(3, testHandler);
    genInterrupt(3);
    genInterrupt(4);
    kprint("Test concluded, running test for a 5 second timer \n");
    //start_timer(15.0, timerHandler);
    kprint("Test concluded, running test for a 2 second delay \n");
    delay(2);
    kprint("Test concluded\n");
    size_t memsize = ((bootinfo->memoryLo + bootinfo->memoryHi)*1024);
    pmmInit(memsize, getPmmSize());
    kprint("Started System PMM with "); kernelPrintDec(memsize / 1024); kprint(" kb of physical memory \n");
    memregion_t* region = (memregion_t*)0x1000;
    for (int i = 0; i < 15; ++i) {

        //! sanity check; if type is > 4 mark it reserved
        if (region[i].type > 4)
            region[i].type = 1;

        //! if start address is 0, there is no more entries, break out
        if (i > 0 && region[i].startLo == 0)
            break;
        if (region[i].type == 1) {
            pmmInitRegion(region[i].startLo, region[i].sizeLo);
            kprint("Initialized region "); kernelPrintDec(i); kprint(" of type "); kprint(memTypes[region[i].type - 1]); kprint(" \n");
        }
    }
    pmmDeinitRegion(0x100000, (getPmmSize()*512));
    _halt();
}

void testHandler() {
    kprint("Inside Handler \n");
}

void timerHandler() {
    kernelPrintDec(readSystemTime()); kprint("\n");
}