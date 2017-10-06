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
    "0",
    "Available",
    "Reserved",
    "ACPI Reclaim",
    "ACPI NVS Memory"
};


int initRegions(multiboot_info_t* bootinfo) {
    // Check GRUB passed the memmory map
    if (!(bootinfo->flags & MULTIBOOT_INFO_MEM_MAP)) {
        system_panic("Couldn't load memory map");
    }
    memory_map_t* mmap = bootinfo->mmap_addr;
    uint8_t regNum = 0;
    while (mmap < bootinfo->mmap_addr + bootinfo->mmap_length) {
        if (mmap->type > 4) {
          mmap->type = 2;
        }
        kprint("Found section ");kernelPrintDec(regNum);kprint(" of type ");kprint(memTypes[mmap->type]);kprint(". \n");
        kprint("Section base: ");kernelPrintHex(mmap->base_addr_low); kprint("\n");
        if (mmap->type == 1) {
          pmmInitRegion((physaddr)mmap->base_addr_low, (size_t)mmap->size);
          kprint("Initialized region in PMM \n");
        }
        mmap = (memory_map_t*)((unsigned int)mmap + mmap->size + sizeof(mmap->size));
        regNum++;
    }
}


int kernel_main(multiboot_info_t* bootinfo) {
    _interrupts(off);
    halInitialize();

    kprint("Welcome to the JSLK Kernel! \n");
    kprint("Copyright (c) 2017 Jacobo Soffer. All Rights Reserved \n");
    #ifndef KERNEL_VERSION_EXTRA
    kprintf("Kernel version: %i.%i.%i-%s \n", KERNEL_VERSION_MAJOR, KERNEL_VERSION_MINOR, KERNEL_VERSION_RELEASE, KERNEL_RELEASE_TYPE);
    #else
    kprintf("Kernel version: %i.%i.%i.%i-%s \n", KERNEL_VERSION_MAJOR, KERNEL_VERSION_MINOR, KERNEL_VERSION_RELEASE, KERNEL_VERSION_EXTRA, KERNEL_RELEASE_TYPE);
    #endif
    kprint("Kernel API version: "); kprint(JSLK_API_VERSION); kprint("\n");
    kprint("Build number: "); kernelPrintDec(KERNEL_BUILD_NUM); kprint("\n");
    if ((bootinfo->flags & MULTIBOOT_INFO_MEM_MAP)) {
        kprint("Bit is set \n");
    }
    kprint("Running test for styled strings: \n");
    uint8_t stringColor = getColor(vga_red, vga_green);
    writeStyledString("This is a string with style \n", stringColor);
    kprint("Test concluded, running test for interrupts \n");
    registerInterruptHandler(3, testHandler);
    genInterrupt(3);
    genInterrupt(4);
    kprint("Test concluded, running test for a 5 second timer \n");
    register_timer(5.0, timerHandler);
    kprint("Test concluded, running test for a 2 second delay \n");
    delay(2);
    kprint("Test concluded\n");
    size_t memsize = ((bootinfo->mem_lower + bootinfo->mem_upper)*1024);
    pmmInit(memsize, getPmmSize());
    kprint("Started System PMM with "); kernelPrintDec(memsize / 1024); kprint(" kb of physical memory \n");
    initRegions(bootinfo);
    // memregion_t* region = (memregion_t*)0x1000;
    /*for (int i = 0; i < 15; ++i) {

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
    }*/
    pmmDeinitRegion(0x100000, (getPmmSize()*512));
    _halt();
}

void testHandler() {
    kprint("Inside Handler \n");
}

void timerHandler() {
    kprint("Executing listener() for timer 0 \n");
    kernelPrintDec(readSystemTime()); kprint("\n");
}
