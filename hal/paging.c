#include "paging.h"
#include "hal.h"
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <timer.h>

#define VM_PAGE_SIZE 4096

void pageFault();

pageDirectory_t* currentDirectory = 0;
pageDirectory_t* kernelDirectory = 0;

extern uint32_t end;
uint32_t placementAddr = &(end);

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
  pmmFreeBlock((void*)page->frame);
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
  while (identityIndex < placementAddr) {
    allocFrame(getPage(identityIndex, 1, kernelDirectory), 0, 0);
    identityIndex += 0x1000/*VM_PAGE_SIZE*/;
  }
  registerInterruptHandler(14, pageFault);
  // Finally enable paging
  switchPageDirectory(kernelDirectory);
}

// THE BUG IS HERE!!
void switchPageDirectory(pageDirectory_t* dir) {
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
    return &dir->tables[tableIndex]->pages[addr%1024];
  } else if (make == true) {
    uint32_t tmp;
    dir->tables[tableIndex] = (pageTable_t*)pmmAllocBlocks(sizeof(pageTable_t));
    memset(dir->tables[tableIndex], 0, 0x1000);
    dir->physicalTables[tableIndex] = tmp | 0x7; // Present, R/W, etc...
    return &dir->tables[tableIndex]->pages[addr%1024];
  } else {
    return 0;
  }
}

void pageFault() {
  atomicalStart();
  setMenubarText("WARNING!");
  uint8_t faultColor = getColor(vga_white, vga_red);
  writeStyledString("PAGE FAULT! \n", faultColor);
  uint32_t faultAddress;
  asm volatile("mov %%cr2, %0" : "=r" (faultAddress));
  kernelPrintHex(faultAddress); kprint("\n");
  delay(15.0);
  system_panic("Non recoverable page fault. \n");
}
