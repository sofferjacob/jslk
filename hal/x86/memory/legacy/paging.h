#ifndef _paging_h
#define _paging_h

#include <stdint.h>
#include <stdbool.h>
#include "../hal.h"

typedef struct regis
{
    uint32_t ds;                                     // Data segment selector
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax; // Pushed by pusha.
    uint32_t int_no, err_code;                       // Interrupt number and error code (if applicable)
    uint32_t eip, cs, eflags, useresp, ss;           // Pushed by the processor automatically.
} regis_t;

typedef struct page {
    uint32_t present  : 1;
    uint32_t rw       : 1;
    uint32_t user     : 1;
    uint32_t accessed : 1;
    uint32_t dirty    : 1;
    uint32_t avail    : 7;  // Available for kernel use
    uint32_t frame    : 20;
} page_t;

typedef struct pageTable {
    page_t pages[1024];
} pageTable_t;

typedef struct pageDirectory {
    pageTable_t* tables[1024];
    uint32_t tablesPhysical[1024];
    uint32_t physaddress;
} pageDirectory_t;

// VM
void initPaging();
void loadPageDirectory();
page_t *getPage(uint32_t addr, bool make, pageDirectory_t *dir);

#endif