#ifndef _x86_paging_hal
#define _x86_paging_hal

#include <stdint.h>

typedef struct pageEntry {
  uint32_t present : 1;
  uint32_t rw : 1;
  uint32_t mode : 1;
  uint32_t accessed : 1;
  uint32_t dirty : 1;
  uint32_t unused : 7;
  uint32_t frame : 20;
} pageEntry_t;

typedef struct pageTable {
  pageEntry_t pages[1024];
} pageTable_t;

typedef struct pageDirectory {
  pageTable_t* tables[1024];
  uint32_t physicalTables[1024];
  uint32_t physicalAddr;
} pageDirectory_t;

#endif
