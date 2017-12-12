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
#include "multiboot.h"

static void PRINT_PREETY_TEXT(char* text) {
    uint8_t kern = getColor(vga_green, vga_black);
    kprint("[");
    cprintf("kernel", kern);
    kprint("] ");
    kprintf("%s \n", text);
}

int kernel_main() {
    clear_console();
    kprint("Welcome to the JSLK Kernel! \n");
    kprint("Copyright (c) 2017 Jacobo Soffer. All Rights Reserved \n");
    #ifndef KERNEL_VERSION_EXTRA
    kprintf("Kernel version: %i.%i.%i-%s \n", KERNEL_VERSION_MAJOR, KERNEL_VERSION_MINOR, KERNEL_VERSION_RELEASE, KERNEL_RELEASE_TYPE);
    #else
    kprintf("Kernel version: %i.%i.%i.%i-%s \n", KERNEL_VERSION_MAJOR, KERNEL_VERSION_MINOR, KERNEL_VERSION_RELEASE, KERNEL_VERSION_EXTRA, KERNEL_RELEASE_TYPE);
    #endif
    kprintf("Kernel API version: %s \n", JSLK_API_VERSION);
    kprint("Build number: "); kernelPrintDec(KERNEL_BUILD_NUM); kprint("\n");
    delay(2);
    uint8_t kern = getColor(vga_green, vga_black);
    PRINT_PREETY_TEXT("Testing VFS and initrd...");
    int i = 0;
    struct dirent *node = 0;
    while ((node = readdir_fs(fs_root, i)) != 0)
    {
        kprint("Found file ");
        kprint(node->name);
        fs_node_t *fsnode = finddir_fs(fs_root, node->name);

        if ((fsnode->flags & 0x7) == FS_DIRECTORY)
        {
            kprint("\n\t(directory)\n");
        }
        else
        {
            kprint("\n\t contents: \"");
            char buf[256];
            uint32_t sz = read_fs(fsnode, 0, 256, (uint8_t*)buf);
            int j;
            for (j = 0; j < sz; j++)
                kputc(buf[j]);

            kprint("\"\n");
        }
        i++;
    }
    PRINT_PREETY_TEXT("Testing VMM...");
    uint32_t a = kmalloc(8);
    uint32_t b = kmalloc(8);
    kprintf("A: %h, B: %h ", a, b);
    kfree((void*)a);
    uint32_t c = kmalloc(8);
    kprintf("C: %h \n", c);
}