#include <hal.h>
#include <stdint.h>
#include <version.h>
#include <timer.h>
#include <va_list.h>
#include <jslk.h>
#include <heap.h>
#include <vfs.h>
#include <initrd.h>
#include <assert.h>
#include <multiboot.h>

extern heap_t *kheap;
extern uint32_t placement_address;
extern int kernel_main();

int kinit(multiboot_info_t *bootinfo)
{
    _interrupts(off);
    uint8_t prekern = getColor(vga_blue, vga_black);
    cprintf("pre-kern", prekern);
    kprint("Staring HAL \n");
    halInitialize();
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
    fs_root = initialise_initrd(initrd_location);
    keyboard_install();
    kernel_main();
    /*
    Load the kernel binary from the initrd (or as a grub module?) as a kernel process here!
    */
}