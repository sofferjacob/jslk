#include <hal.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <version.h>
#include <timer.h>
#include <va_list.h>
#include <jslk.h>
#include <heap.h>
#include <vfs.h>
#include <initrd.h>
#include <assert.h>
#include "multiboot.h"

static uint32_t logOffset = 0;

static void PRINT_PRETTY_TEXT(char* text) {
    uint8_t kern = getColor(vga_green, vga_black);
    kprint("[");
    cprintf("kernel", kern);
    kprint("] ");
    kprintf("%s \n", text);
}

void open(char* file_name) {
    char buf[256];
    FILE* fsnode = finddir_fs(root_fs, file_name);
    if (fsnode == 0) {
        kprintf("ERROR: File %s not found on disk. \n", file_name);
        return;
    }
    uint32_t length = fsnode->length;
    uint32_t bread = 0;
    while (bread < length) {
        uint32_t sz = read_fs(fsnode, bread, 256, (uint8_t *)buf);
        int j;
        for (j = 0; j < sz; j++)
            kputc(buf[j]);
        bread += sz;
        kprint("\n press enter to continue \n");
        while(read_key() != 0);
    }
    kprint("\n");
}

void vfsLog(uint8_t* buf) {
    FILE* fs_node = finddir_fs(root_fs, "initrd/log.txt");
    if (fs_node == 0) {
        kprintf("ERROR: log.txt not found! \n");
        return;
    }
    size_t bufLen = strlen(buf);
    size_t written = 0;
    logOffset = write_fs(fs_node, logOffset, bufLen, buf);
}

int kernel_main() {
    clear_console();
    kprint("Welcome to the JSLK Kernel! \n");
    kprintf("%s\n", LICENSE);
    #ifndef KERNEL_VERSION_EXTRA
    kprintf("Kernel version: %i.%i.%i-%s \n", KERNEL_VERSION_MAJOR, KERNEL_VERSION_MINOR, KERNEL_VERSION_RELEASE, KERNEL_RELEASE_TYPE);
    #else
    kprintf("Kernel version: %i.%i.%i.%i-%s \n", KERNEL_VERSION_MAJOR, KERNEL_VERSION_MINOR, KERNEL_VERSION_RELEASE, KERNEL_VERSION_EXTRA, KERNEL_RELEASE_TYPE);
    #endif
    kprintf("Kernel API version: %s \n", JSLK_API_VERSION);
    kprint("Build number: "); kernelPrintDec(KERNEL_BUILD_NUM); kprint("\n");
    delay(2);
    uint8_t kern = getColor(vga_green, vga_black);
    rtcTime_t time =  getRtcTime();
    kprintf("Hours: %i, minutes: %i, seconds %i, day: %i, month: %i, year: %i \n", time.hours, time.minutes, time.seconds, time.week_day, time.month_day, (time.year + 2000));
    delay(3);
    PRINT_PRETTY_TEXT("Testing VFS and initrd...");
    int i = 0;
    struct dirent *node = 0;
    while ((node = readdir_fs(root_fs, i)) != 0)
    {
        kprint("Found file ");
        kprint(node->name);
        FILE* fsnode = finddir_fs(root_fs, node->name);

        if (fsnode->type == FS_DIRECTORY)
        {
            kprint("\n\t(directory)\n");
        }
        else
        {
            kprintf("\n\t length: %i \n", fsnode->length);
            kprint("\t contents: \"");
            char buf[256];
            uint32_t sz = read_fs(fsnode, 0, 256, (uint8_t*)buf);
            int j;
            for (j = 0; j < sz; j++)
                kputc(buf[j]);

            kprint("\"\n");
        }
        i++; 
    }
    FILE* hpot = finddir_fs(root_fs, "initrd/hpotter.txt");
    kprintf("Initial file status: %i", hpot->eof);
    open("initrd/hpotter.txt");
    kprintf("File status: %i", hpot->eof);
    PRINT_PRETTY_TEXT("Testing VMM...");
    uint32_t a = kmalloc(8);
    uint32_t b = kmalloc(8);
    kprintf("A: %h, B: %h ", a, b);
    kfree((void*)a);
    uint32_t c = kmalloc(8);
    kprintf("C: %h \n", c);
    rtcTime_t time2 = getRtcTime();
    kprintf("Hours: %i, minutes: %i, seconds %i, day: %i, month: %i, year: %i \n", time2.hours, time2.minutes, time2.seconds, time2.week_day, time2.month_day, time2.year);
    uint8_t* strToLog = "hello world";
    vfsLog(strToLog);
    kprintf("done testing log. string logged: %s . Bytes written: %i \n", strToLog, logOffset);
    open("initrd/log.txt");
}