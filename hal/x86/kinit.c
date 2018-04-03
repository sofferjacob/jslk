#include <hal.h>
#include <stdint.h>
#include <version.h>
#include <timer.h>
#include <va_list.h>
#include <jslk.h>
#include <heap.h>
// #include <vfs.h>
#include <vfs.h>
#include <initrd.h>
#include <assert.h>
#include <multiboot.h>

extern heap_t *kheap;
extern uint32_t placement_address;
extern int kernel_main();
multiboot_info_t* meminfo;
uint32_t initial_esp;

void doubleFaultHandler(regs_t regs) {
    PANIC("Double fault");
    _halt();
}

void stackFaultHandler(regs_t regs) {
    PANIC("Stack fault");
    _halt();
}

void badTssFaultHandler(regs_t regs)
{
    PANIC("Bad tss");
    _halt();
}

void segNPFaultHandler(regs_t regs)
{
    PANIC("Segment not present exception");
    _halt();
}

int kinit(multiboot_info_t *bootinfo, uint32_t stack)
{
    _interrupts(off);
    initial_esp = stack;
    uint8_t prekern = getColor(vga_blue, vga_black);
    cprintf("pre-kern", prekern);
    kprint("Staring HAL \n");
    halInitialize();
    registerInterruptHandler(8, doubleFaultHandler, CHAIN_PROTECT, NF);
    registerInterruptHandler(10, badTssFaultHandler, CHAIN_PROTECT, NF);
    registerInterruptHandler(11, segNPFaultHandler, CHAIN_PROTECT, NF);
    registerInterruptHandler(12, stackFaultHandler, CHAIN_PROTECT, NF);
    kprint("[");
    cprintf("pre-kern", prekern);
    kprint("]");
    kprint(" Loading initrd... \n");
    assert(bootinfo->mods_count > 0);
    uint32_t initrd_location = *((uint32_t *)bootinfo->mods_addr);
    uint32_t initrd_end = *(uint32_t *)(bootinfo->mods_addr + 4);
    // Don't trample our module with placement accesses, please!
    placement_address = initrd_end;

    kprint("[");
    cprintf("pre-kern", prekern);
    kprint("]");
    kprint(" Starting VMM... \n");
    // Start paging.
    initialise_paging();

    // Initialise the initial ramdisk, and set it as the filesystem root.
    root_fs = initialise_initrd(initrd_location);
    keyboard_install();
    kernel_main();
    /*
    Load the kernel binary from the initrd (or as a grub module?) as a kernel process here!
    */
}