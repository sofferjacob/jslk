#include "paging.h"
#include "hal.h"
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#define VM_PAGE_SIZE 4096

extern size_t memSize;

pageDirectory_t* currentDirectory = 0;
pageDirectory_t* kernelDirectory = 0;

int allocFrame(pageEntry_t* page, bool kernelMode, bool rw) {
  if (page->frame != 0) {
    return 1;
  }
  atomicalStart();
  uint32_t physFrame = (uint32_t)pmmAllocBlock();
  if (!physFrame) {
    system_panic("Out of memory");
  }
  page->present = 1;
  page->rw = (rw)?1:0;
  page->mode = (kernelMode)?0:1;
  page->frame = physFrame;
  atomicalRelease();
}

int freeFrame(pageEntry_t* page) {
  atomicalStart();
  if (page->frame == 0) {
    // Page wasn't allocated
    atomicalRelease();
    return 0;
  }
  pmmFreeBlock(page->frame);
  page->frame = 0x0;
  atomicalRelease();
}

void initPaging() {
  // Create a directory to be used as the kernels address space
  kernelDirectory = (pageDirectory_t*)pmmAllocBlocks(sizeof(pageDirectory_t));
  memset(kernelDirectory, 0, sizeof(pageDirectory_t));
  currentDirectory = kernelDirectory;
  // Identity map
  size_t identityIndex = 0;
  while (i < memSize) {
    allocFrame(getPage(i, 1, kernelDirectory), 0, 0);
    i += VM_PAGE_SIZE;
  }
  registerInterruptHandler(14, pageFault);
  // Finally enable paging
  switchPageDirectory(kernelDirectory);
}

void switchPageDirectory(pageDirectory_t dir) {
  currentDirectory = dir;
  asm volatile("mov %0, %%cr3":: "r"(&dir->physicalTables));
  uint32_t cr0;
  asm volatile("mov %%cr0, %0": "=r"(cr0));
  cr0 |= 0x80000000; // Enable paging!
  asm volatile("mov %0, %%cr0":: "r"(cr0));
}

pageEntry_t* getPage(uint32_t addr, bool make, pageDirectory_t* dir) {
  uint32_t tableIndex = addr / 1024;
  if (dir->tables[tableIndex]) {
    return &dir->tables[tableIndex]->pages->[addr%1024];
  } else if (make == true) {
    uint32_t tmp;
    dir->tables[tableIndex] = (pageTable_t*)pmmAllocBlocks(sizeof(pageTable_t));
    memset(dir->tables[tableIndex], 0, 0x1000);
    dir->physicalTables[tableIndex] = tmp | 0x7; // Present, R/W, etc...
    return &dir->tables[tableIndex]->pages->[addr%1024];
  } else {
    return 0;
  }
}
