#include <hal.h>
#include <types.h>
#include <version.h>
#include <timer.h>
#include <va_list.h>
#include <jslk.h>
#include <heap.h>
#include "multiboot.h"

extern heap_t *kheap;
void testHandler();
void timerHandler();
void c1();
void c2();
void c3();
void doubleFault();
void outOfBounds();

    /*
extern unsigned int* end;
extern unsigned int* _start;
size_t getSizeOfKernel() {
    return ((size_t)(&_start) + 1) - ((size_t)(&end));
} */

    string memTypes[] = {
        "0",
        "Available",
        "Reserved",
        "ACPI Reclaim",
        "ACPI NVS Memory"};

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
          //init_region((uint32_t)mmap->base_addr_low, (uint32_t)mmap->size);
          kprintf("Initialized region %i\n", regNum);
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
    kprintf("Kernel API version: %s \n", JSLK_API_VERSION);
    kprint("Build number: "); kernelPrintDec(KERNEL_BUILD_NUM); kprint("\n");
    if ((bootinfo->flags & MULTIBOOT_INFO_MEM_MAP)) {
        kprint("Bit is set \n");
    }
    kprint("Running test for styled strings: \n");
    uint8_t stringColor = getColor(vga_red, vga_green);
    writeStyledString("This is a string with style \n", stringColor);
    kprint("Test concluded, running test for interrupts \n");
    registerInterruptHandler(5, outOfBounds, NF);
    registerInterruptHandler(3, testHandler, CHAIN_PROTECT, NF);
    genInterrupt(3);
    genInterrupt(4);
    kprint("Running test for chained interrupts: \n");
    registerInterruptHandler(4, c1, CHAINED_INTERRUPT, NF);
    registerInterruptHandler(4, c2, CHAINED_INTERRUPT, NF);
    registerInterruptHandler(4, c3, CHAINED_INTERRUPT, NF);
    genInterrupt(4);
    delay(5.0);
    clearInterrupt(3);
    clearInterrupt(4);
    kprint("Starting a 5 second timer \n");
    register_timer(5.0, timerHandler);
    kprint("Test concluded, running test for a 2 second delay \n");
    delay(2);
    kprint("Test concluded\n");
    uint32_t memsize = ((bootinfo->mem_lower + bootinfo->mem_upper)*1024);
    kprint("Detected "); kernelPrintDec(memsize / 1024); kprint(" kb of physical memory \n");
    start_pmm(memsize);
    kprint("Initialized PMM");
    initRegions(bootinfo);
    cprintf("Cprintf test %i.%i.%i \n", stringColor, KERNEL_VERSION_MAJOR, KERNEL_VERSION_MINOR, KERNEL_VERSION_RELEASE);
    kprint("test concluded \n");
    keyboard_install();
    kprintf("Keyboard enabled at %i ticks \n", readSystemTime());
    registerInterruptHandler(8, doubleFault, NF);
    kprint("Beggining memory test \n");
    kprint("Enabling paging \n");
    delay(2);
    uint32_t bp = kmalloc(sizeof(int));
    uint32_t bp2 = kmalloc(sizeof(int));
    kprintf("BP: %h \n, BP2: %h \n", bp, bp2);
    initialise_paging();
    kprint("Paging enabled \n");
    uint32_t ap = kmalloc(8);
    uint32_t aph = kmalloc(8);
    kprintf("AP: %h \n", ap);
    kprintf("APH: %h \n", aph);
    kfree(ap);
    kfree(aph);
    uint32_t aphd = kmalloc(8);
    kprintf("APHD: %h \n", aphd);
    kprint("Finished testing memory \n");
    /*
    kprint("Welcome to jsh!\n");
    kprint("Enter a command \n");
    kprint("> ");*/
    // initPaging();
    // uint32_t *ptr = (uint32_t *)0xDEADBEEF;
    // *ptr = 21;
    //kprintf("%i \n", *ptr);
    //genInterrupt(14);
}

void c1() {
    uint16_t a = 21;
    kprintf("Inside first handler %i \n", 21);
}

void c2() {
    kprint("Inside second handler \n");
}
void c3()
{
    kprint("Inside third handler \n");
}

void doubleFault() {
    atomicalStart();
    setMenubarText("ERROR");
    uint8_t faultColor = getColor(vga_white, vga_red);
    writeStyledString("Could not enable paging!! \n", faultColor);
    atomicalRelease();
    delay(15.0);
    system_panic("Non recoverable kernel exception (PGN_NE) \n");
}

void outOfBounds() {
    system_panic("Trying to execute code outside of RAM or ROM \n");
}

void testHandler() {
    kprint("Inside Handler \n");
}

void timerHandler() {
    kprintf("Executing listener() for timer 0. System time: %i \n", readSystemTime());
}

void jsh(char* userinput) {
    if (strcmp(userinput, "help") == 0) {
        kprint("AAA \n");
    }
    else {
        kprint("Command not found \n");
    }
    kprint("> ");
}