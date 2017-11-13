// Physical Memmory Manager
#ifndef _x86_pmm_h
#define _x86_pmm_h

#include <stdint.h>

#define PMM_BLOCKS_PER_BYTE 8
#define PMM_BLOCK_SIZE 4096
#define PMM_BLOCK_ALIGN PMM_BLOCK_SIZE

static uint32_t pmm_mem_size = 0;   // Size of physical memmory
static uint32_t pmm_used_blocks = 0; // Currently used blocks
static uint32_t* pmm_memory_map = 0; // Memmory map bit array
static uint32_t pmm_total_block_count = 0;

// Physical Address Type
typedef uint32_t physaddr;

#endif