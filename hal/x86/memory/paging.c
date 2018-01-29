// paging.c -- Defines the interface for and structures relating to paging.
//             Written for JamesM's kernel development tutorials.

#include "paging.h"
#include "kheap.h"
#include <stdint.h>
#include <timer.h>
#include <heap.h>
#include <string.h>

extern heap_t* kheap;
extern uint32_t end;

/**
   Handler for page faults.
**/
void page_fault(regs_t regs);

// The kernel's page directory
page_directory_t *kernel_directory=0;

// The current page directory;
page_directory_t *current_directory=0;

// A bitset of frames - used or free.
/*static*/ uint32_t *frames;
/*static*/ uint32_t nframes;
/*static*/ uint32_t mem_end_page = 0;
/*static*/ uint32_t used_frames;

// Defined in kheap.c
extern uint32_t placement_address;

uint32_t getTotalFrames() {
    return nframes;
}

// Macros used in the bitset algorithms.
#define INDEX_FROM_BIT(a) (a/(8*4))
#define OFFSET_FROM_BIT(a) (a%(8*4))
/*
// Static function to set a bit in the frames bitset
static void set_frame(uint32_t frame_addr)
{
    uint32_t frame = frame_addr/0x1000;
    uint32_t idx = INDEX_FROM_BIT(frame);
    uint32_t off = OFFSET_FROM_BIT(frame);
    frames[idx] |= (0x1 << off);
} */

void set_frame(uint32_t frame_addr) {
		uint32_t frame  = frame_addr / 0x1000;
		uint32_t index  = INDEX_FROM_BIT(frame);
		uint32_t offset = OFFSET_FROM_BIT(frame);
		frames[index] |= ((uint32_t)0x1 << offset);
}

// Static function to clear a bit in the frames bitset
static void clear_frame(uint32_t frame_addr)
{
    uint32_t frame = frame_addr/0x1000;
    uint32_t idx = INDEX_FROM_BIT(frame);
    uint32_t off = OFFSET_FROM_BIT(frame);
    frames[idx] &= ~(0x1 << off);
}

// Static function to test if a bit is set.
static uint32_t test_frame(uint32_t frame_addr)
{
    uint32_t frame = frame_addr/0x1000;
    uint32_t idx = INDEX_FROM_BIT(frame);
    uint32_t off = OFFSET_FROM_BIT(frame);
    return (frames[idx] & (0x1 << off));
}

// Static function to find the first free frame.
static uint32_t first_frame()
{
    uint32_t i, j;
    for (i = 0; i < INDEX_FROM_BIT(nframes); i++)
    {
        if (frames[i] != 0xFFFFFFFF) // nothing free, exit early.
        {
            // at least one bit is free here.
            for (j = 0; j < 32; j++)
            {
                uint32_t toTest = 0x1 << j;
                if ( !(frames[i]&toTest) )
                {
                    return i*4*8+j;
                }
            }
        }
    }
}

// Function to allocate a frame.
int alloc_frame(page_t *page, int is_kernel, int is_writeable)
{
    if (used_frames == nframes) {
        return 1;  // We don't have enough memory, return.
    }
    if (page->frame != 0)
    {
        return 0;
    }
    else
    {
        uint32_t idx = first_frame();
        if (idx == (uint32_t)-1)
        {
            PANIC("No free frames");
        }
        set_frame(idx*0x1000);
        page->present = 1;
        page->rw = (is_writeable)?1:0;
        page->user = (is_kernel)?0:1;
        page->frame = idx;
    }
}

// Function to deallocate a frame.
void free_frame(page_t *page)
{
    uint32_t frame;
    if (!(frame=page->frame))
    {
        return;
    }
    else
    {
        clear_frame(frame);
        page->frame = 0x0;
    }
}

/*
// Initialize a region in the PMM
void init_region(uint32_t base, size_t size) {
    uint32_t align = base / 0x1000;
    uint32_t blocks = size / 0x1000;
    for (; blocks > 0; blocks--) {
        clear_frame(INDEX_FROM_BIT(align++));
        used_frames--;
    }
    set_frame(0);
} 

// Deinitialize a region in the PMM
void deinit_region(uint32_t base, size_t size) {
    uint32_t align = base / 0x1000;
    uint32_t blocks = size / 0x1000;
    for (; blocks > 0; blocks--) {
        set_frame(OFFSET_FROM_BIT(align++));
        used_frames++;
        nframes--;
    }
}*/

void deinit_region(uint32_t addr, uint32_t size) {
    uint32_t address = addr/0x1000;
    uint32_t finalsize = addr + size;
    while (address < size) {
        set_frame(address);
        nframes--;
        address += 0x1000;
    }
}

string memTypes[] = {
    "0",
    "Available",
    "Reserved",
    "ACPI Reclaim",
    "ACPI NVS Memory"};

int initRegions(multiboot_info_t* bootinfo) {
    // Check GRUB passed the memmory map
    if (!(bootinfo->flags & MULTIBOOT_INFO_MEM_MAP)) {
        PANIC("Couldn't load memory map");
    }
    memory_map_t* mmap = (memory_map_t*)bootinfo->mmap_addr;
    uint8_t regNum = 0;
    while (mmap < bootinfo->mmap_addr + bootinfo->mmap_length) {
        if (mmap->type > 4) {
          mmap->type = 2;
        }
        kprint("Found section ");kernelPrintDec(regNum);kprint(" of type ");kprint(memTypes[mmap->type]);kprint(". \n");
        kprint("Section base: ");kernelPrintHex(mmap->base_addr_low); kprint("\n");
        if (mmap->type == 2) {
          uint64_t aligned_start = (mmap->base_addr_low + 0x0000000000000FFF) & 0xFFFFFFFFFFFFF000; 
          if (aligned_start < 0x00000000FFFFFFFF) {
              uint64_t aligned_end = (mmap->base_addr_low + mmap->length_low) & 0xFFFFFFFFFFFFF000;
              if (aligned_end > 0x0000000100000000) {
                  aligned_end = 0x0000000100000000;
              }
              for (uint64_t i = aligned_start; i < aligned_end; i += 0x1000) {
                  set_frame(i);
              }
          } 
          kprintf("Deinitialized region %i\n", regNum);
        }
        mmap = (memory_map_t*)((unsigned int)mmap + mmap->size + sizeof(mmap->size));
        regNum++;
    }
}

void start_pmm(multiboot_info_t* bootinfo) {
    uint32_t mem_size = ((bootinfo->mem_lower + bootinfo->mem_upper)/**/);
    kprintf("Started PMM with %i kb of physical memory \n", mem_size/1024);
    mem_end_page = mem_size;
    nframes = mem_size/0x1000;  // 4096 (the size of a frame)
    used_frames = 1;            // We don't know which parts of memory are usable.
    frames = (uint32_t*)kmalloc(INDEX_FROM_BIT(nframes));
    memset(frames, 0, INDEX_FROM_BIT(nframes));
    initRegions(bootinfo);
    kprintf("Available frames: %i \n", nframes);
}

void initialise_paging()
{
    kprintf("**TEST: Random Used frame status: %i **\n", test_frame(0x7fe0000 + 0x1000));
    delay(3.0);
    if (mem_end_page <= 0)
    {
        /*
        The PMM didn't start, assume we have
        16 MB of RAM
        */
        uint32_t mem_end_page = 0x1000000;
        nframes = mem_end_page / 0x1000;
        frames = (uint32_t *)kmalloc(INDEX_FROM_BIT(nframes));
        memset(frames, 0, INDEX_FROM_BIT(nframes));
    }
    else if (used_frames == nframes)
    {
        PANIC("Couldn't enable paging: not enough memory");
        return; // Not enough memory
    }
    // Let's make a page directory.
    uint32_t phys;
    kernel_directory = (page_directory_t *)kmalloc_a(sizeof(page_directory_t));
    memset(kernel_directory, 0, sizeof(page_directory_t));
    kernel_directory->physicalAddr = (uint32_t)kernel_directory->tablesPhysical;
    // Map some pages in the kernel heap area.
    // Here we call get_page but not alloc_frame. This causes page_table_t's
    // to be created where necessary. We can't allocate frames yet because they
    // they need to be identity mapped first below, and yet we can't increase
    // placement_address between identity mapping and enabling the heap!
    int i = 0;
    for (i = KHEAP_START; i < KHEAP_START + KHEAP_INITIAL_SIZE; i += 0x1000)
        get_page(i, 1, kernel_directory);
    // We need to identity map (phys addr = virt addr) from
    // 0x0 to the end of used memory, so we can access this
    // transparently, as if paging wasn't enabled.
    // NOTE that we use a while loop here deliberately.
    // inside the loop body we actually change placement_address
    // by calling kmalloc(). A while loop causes this to be
    // computed on-the-fly rather than once at the start.
    // Allocate a lil' bit extra so the kernel heap can be
    // initialised properly.
    //i = 0;
    i = 0;
    while (i < placement_address + 0x1000)
    {
        // Kernel code is readable but not writeable from userspace.
        alloc_frame(get_page(i, 1, kernel_directory), 0, 0);
        i += 0x1000;
    }

    // Now allocate those pages we mapped earlier.
    for (i = KHEAP_START; i < KHEAP_START + KHEAP_INITIAL_SIZE; i += 0x1000)
        alloc_frame(get_page(i, 1, kernel_directory), 0, 0);
    // Before we enable paging, we must register our page fault handler.
    registerInterruptHandler(14, page_fault, NF);

    // Now, enable paging!
    switch_page_directory(kernel_directory);
    kprint("Paging enabled \n");
    delay(2.0);

    // Initialise the kernel heap.
    kheap = createHeap(KHEAP_START, KHEAP_START + KHEAP_INITIAL_SIZE, 0xCFFFF000, 0, 0);
    // current_directory = clone_directory(kernel_directory);
    kprintf("Cloned dirs... \n");
    delay(2);
    // switch_page_directory(current_directory);
}

void switch_page_directory(page_directory_t *dir)
{
    current_directory = dir;
    asm volatile("mov %0, %%cr3" ::"r"(dir->physicalAddr));
    uint32_t cr0;
    asm volatile("mov %%cr0, %0"
                 : "=r"(cr0));
    cr0 |= 0x80000000; // Enable paging!
    asm volatile("mov %0, %%cr0" ::"r"(cr0));
}

page_t *get_page(uint32_t address, int make, page_directory_t *dir)
{
    // Turn the address into an index.
    address /= 0x1000;
    // Find the page table containing this address.
    uint32_t table_idx = address / 1024;
    if (dir->tables[table_idx]) // If this table is already assigned
    {
        return &dir->tables[table_idx]->pages[address%1024];
    }
    else if(make)
    {
        uint32_t tmp;
        dir->tables[table_idx] = (page_table_t*)kmalloc_ap(sizeof(page_table_t), &tmp);
        dir->tablesPhysical[table_idx] = tmp | 0x7; // PRESENT, RW, US.
        return &dir->tables[table_idx]->pages[address%1024];
    }
    else
    {
        return 0;
    }
}


void page_fault(regs_t regs)
{
    atomicalStart();
    // A page fault has occurred.
    // The faulting address is stored in the CR2 register.
    uint32_t faulting_address;
    asm volatile("mov %%cr2, %0" : "=r" (faulting_address));
    
    // The error code gives us details of what happened.
    int present   = !(regs.err_code & 0x1); // Page not present
    int rw = regs.err_code & 0x2;           // Write operation?
    int us = regs.err_code & 0x4;           // Processor was in user-mode?
    int reserved = regs.err_code & 0x8;     // Overwritten CPU-reserved bits of page entry?
    int id = regs.err_code & 0x10;          // Caused by an instruction fetch?

    // Output an error message.
    kprint("Page fault! ( ");
    if (present) {kprint("present ");}
    if (rw) {kprint("read-only ");}
    if (us) {kprint("user-mode ");}
    if (reserved) {kprint("reserved ");}
    kprint(") at 0x");
    kernelPrintHex(faulting_address);
    kprint("\n");
    atomicalRelease();
    delay(5);
    PANIC("Page fault");
}

static page_table_t *clone_table(page_table_t *src, uint32_t *physAddr)
{
    // Make a new page table, which is page aligned.
    page_table_t *table = (page_table_t *)kmalloc_ap(sizeof(page_table_t), physAddr);
    // Ensure that the new table is blank.
    memset(table, 0, sizeof(page_directory_t));

    // For every entry in the table...
    int i;
    for (i = 0; i < 1024; i++)
    {
        // If the source entry has a frame associated with it...
        if (src->pages[i].frame)
        {
            // Get a new frame.
            alloc_frame(&table->pages[i], 0, 0);
            // Clone the flags from source to destination.
            if (src->pages[i].present)
                table->pages[i].present = 1;
            if (src->pages[i].rw)
                table->pages[i].rw = 1;
            if (src->pages[i].user)
                table->pages[i].user = 1;
            if (src->pages[i].accessed)
                table->pages[i].accessed = 1;
            if (src->pages[i].dirty)
                table->pages[i].dirty = 1;
            // Physically copy the data across. This function is in process.s.
            copy_page_physical(src->pages[i].frame * 0x1000, table->pages[i].frame * 0x1000);
        }
    }
    return table;
}

page_directory_t *clone_directory(page_directory_t *src)
{
    uint32_t phys;
    // Make a new page directory and obtain its physical address.
    page_directory_t *dir = (page_directory_t *)kmalloc_ap(sizeof(page_directory_t), &phys);
    // Ensure that it is blank.
    memset(dir, 0, sizeof(page_directory_t));

    // Get the offset of tablesPhysical from the start of the page_directory_t structure.
    uint32_t offset = (uint32_t)dir->tablesPhysical - (uint32_t)dir;

    // Then the physical address of dir->tablesPhysical is:
    dir->physicalAddr = phys + offset;

    // Go through each page table. If the page table is in the kernel directory, do not make a new copy.
    int i;
    for (i = 0; i < 1024; i++)
    {
        if (!src->tables[i])
            continue;

        if (kernel_directory->tables[i] == src->tables[i])
        {
            // It's in the kernel, so just use the same pointer.
            dir->tables[i] = src->tables[i];
            dir->tablesPhysical[i] = src->tablesPhysical[i];
        }
        else
        {
            // Copy the table.
            uint32_t phys;
            dir->tables[i] = clone_table(src->tables[i], &phys);
            dir->tablesPhysical[i] = phys | 0x07;
        }
    }
    return dir;
}
