#include "paging.h"
#include "../hal.h"
#include <stdint.h>
#include <stdbool.h>
#include <timer.h>

extern uint32_t placement_address;

void pageFaultHandler(regs_t regs);

static pageDirectory_t* kernel_directory = 0;
static pageDirectory_t* current_directory = 0;

// Kernel First Stage Memory Manager
// Physical Memory
static uint32_t* frames;
static uint32_t nframes;

// Note to self: You are using this macros to make code clearer (instead of using one unexplained math operation)
#define INDEX_FROM_BIT(a) (a/(8*4))
#define OFFSET_FROM_BIT(a) (a%(8*4))

static void setFrame(uint32_t bit) {
    uint32_t frame = bit / 0x1000;
    uint32_t idx = INDEX_FROM_BIT(frame);
    uint32_t offset = OFFSET_FROM_BIT(frame);
    frames[idx] |= (0x1 << off);
}

static void clearFrame(uint32_t bit) {
    uint32_t frame = bit / 0x1000;
    uint32_t idx = INDEX_FROM_BIT(frame);
    uint32_t offset = OFFSET_FROM_BIT(frame);
    frames[idx] &= ~(0x1 << off);
}

static uint32_t testFrame(uint32_t bit) {
    uint32_t frame = bit / 0x1000;
    uint32_t idx = INDEX_FROM_BIT(frame);
    uint32_t offset = OFFSET_FROM_BIT(frame);
    return frames[idx] & (0x1 << off);
}

static uint32_t mapFirstFree() {
    int bit_to_map;
    for (size_t i = 0; i < (nframes / 32); i++)
    {
        if (frames[i] != 0xffffffff)
        {
            for (int j = 0; j < 32; j++)
            {
                int bitToMap = 0x1 << j;
                if (!(frames[i] & bitToMap))
                {
                    return i * 4 * 8 + j;
                }
            }
        }
    }
    return -1;
}

static uint32_t mapFirstFrees(size_t size) {
    if (size == 0)
        return -1;

    if (size == 1)
        return mapFirstFree();

    for (uint32_t i = 0; i < nframes / 32; i++)
        if (frames[i] != 0xffffffff)
            for (int j = 0; j < 32; j++)
            { //! test each bit in the dword

                int bit = 1 << j;
                if (!(frames[i] & bit))
                {

                    int startingBit = i * 32;
                    startingBit += bit; //get the free bit in the dword at index i

                    uint32_t free = 0; //loop through each bit to see if its enough space
                    for (uint32_t count = 0; count <= size; count++)
                    {

                        if (!testFrame(startingBit + count))
                            free++; // this bit is clear (free frame)

                        if (free == size)
                            return i * 4 * 8 + j; //free count==size needed; return index
                    }
                }
            }

    return -1;
}

// Virtual Memory Related functions
void allocFrame(page_t* page, int isKernel, int isWriteable) {
    if (page->frame != 0) {
        // This page already has a frame
        return;
    } else {
        atomicalStart();
        uint32_t idx = mapFirstFree();
        if (idx == (uint32_t)-1) {
            PANIC("Out of memory!");
        }   
        setFrame(idx*0x1000);
        page->present = 1;
        page->rw = (isWriteable)?1:0;
        page->user = (isKernel)?0:1;
        page->frame = idx;
        atomicalRelease();
    }
}

void freeFrame(page_t* page) {
    uint32_t frame = page->frame;
    if (!(frame)) {
        return;  // The page didn't have a frame
    } else {
        clearFrame(frame);
        page->frame = 0x0;
    }
}

void initPaging() {
    uint32_t memEndPage = 0x1000000;  // We will work with 16 mb of RAM for now...
    nframes = memEndPage / 0x1000;
    frames = (uint32_t*)kmalloc(INDEX_FROM_BIT(nframes));
    memset(frames, 0, INDEX_FROM_BIT(nframes));
    kernel_directory = (pageDirectory_t*)kmalloc_a(sizeof(pageDirectory_t));
    memset(kernel_directory, 0, sizeof(pageDirectory_t));
    current_directory = kernel_directory;
    // Identity map
    int i = 0;
    while (i < placement_address) {
        allocFrame(getPage(i, true, kernel_directory), 0, 0);
        i += 0x1000;
    }
    registerInterruptHandler(14, pageFaultHandler, NF);
    switchPageDirectory(kernel_directory);
}

void switchPageDirectory(pageDirectory_t* dir) {
    current_directory = dir;
    asm volatile("mov %0, %%cr3" ::"r"(&dir->tablesPhysical));
    uint32_t cr0;
    asm volatile("mov %%cr0, %0": "=r"(cr0));
    cr0 |= 0x80000000; // Enable paging!
    asm volatile("mov %0, %%cr0" ::"r"(cr0));
} 

page_t* getPage(uint32_t addr, bool make, pageDirectory_t* dir) {
    addr /= 0x1000;
    uint32_t tableIdx = addr / 1024;
    if (dir->tables[tableIdx]) {
        return &dir->tables[tableIdx]->pages[addr%1024];
    } else if (make) {
        uint32_t tmp;
        dir->tables[tableIdx] = (pageTable_t*)kmalloc_ap(sizeof(pageTable_t), &tmp);
        memset(dir->tables[tableIdx], 0, 0x1000);
        dir->tablesPhysical[tableIdx] = tmp | 0x7;
        return &dir->tables[tableIdx]->pages[addr % 1024];
    } else {
        return 0;
    }
}

void pageFaultHandler(regs_t regs) {
    // A page fault has occurred.
    // The faulting address is stored in the CR2 register.
    uint32_t faulting_address;
    asm volatile("mov %%cr2, %0"
                 : "=r"(faulting_address));

    // The error code gives us details of what happened.
    int present = !(regs.err_code & 0x1); // Page not present
    int rw = regs.err_code & 0x2;         // Write operation?
    int us = regs.err_code & 0x4;         // Processor was in user-mode?
    int reserved = regs.err_code & 0x8;   // Overwritten CPU-reserved bits of page entry?
    int id = regs.err_code & 0x10;        // Caused by an instruction fetch?

    // Output an error message.
    kprint("Page fault! ( ");
    if (present)
    {
        kprint("present ");
    }
    if (rw)
    {
        kprint("read-only ");
    }
    if (us)
    {
        kprint("user-mode ");
    }
    if (reserved)
    {
        kprint("reserved ");
    }
    kprint(") at 0x");
    kernelPrintHex(faulting_address);
    kprint("\n");
    delay(2.0);
    PANIC("Page fault");
}