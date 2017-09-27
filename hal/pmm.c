#include "pmm.h"
#include "hal.h"
#include <stdint.h>
#include <string.h>

// Based on BrokenThorn Tutorials

static void pmm_set(int bit) {
    pmm_memory_map[bit / 32] |= (1 << (bit % 32));
}

static void pmm_unset(int bit) {
    pmm_memory_map[bit / 32] &= ~ (1 << (bit % 32));
}

static int pmm_test(int bit) {
    return pmm_memory_map[bit / 32] & (1 << (bit % 32));
}

static uint32_t pmmGetBlockCount() {
    return pmm_total_block_count;
}

static size_t pmmGetMemSize() {
    return pmm_mem_size;
}

int pmmMapFirstFree() {
    int bit_to_map;
    for (size_t i = 0; i < (pmmGetBlockCount() / 32); i++) {
        if (pmm_memory_map[i] != 0xffffffff) {
            for (int j = 0; j < 32; j++) {
                int bitToMap = 1 << j;
                if (! (pmm_memory_map[i] & bitToMap)) {
                        return i*4*8+j;
                }
            }
        }
    }
    return -1;
}

void pmmInit(size_t memSize, physaddr bitmap) {
    pmm_mem_size = memSize;
    pmm_memory_map = (uint32_t *)bitmap;
    pmm_total_block_count = (pmmGetMemSize() * 1024 / PMM_BLOCK_SIZE);
    pmm_used_blocks = pmmGetBlockCount();
    // Set all memmory to be in use
    memset(pmm_memory_map, 0xf, pmmGetBlockCount() / PMM_BLOCKS_PER_BYTE);
}

// init a memmory region
void pmmInitRegion(physaddr base, size_t size) {
    int align = base / PMM_BLOCK_SIZE;
    int blocks = size / PMM_BLOCK_SIZE;

    for ( ; blocks > 0; blocks--) {
        pmm_unset(align++);
        pmm_used_blocks--;
    }
    pmm_set(0);
}

// deinit a memmory region
void pmmDeinitRegion(physaddr base, size_t size) {
    int align = base / PMM_BLOCK_SIZE;
    int size = size / PMM_BLOCK_SIZE;

    for ( ; blocks > 0; blocks--) {
        pmm_set(align++);
        pmm_used_blocks++;
    }
}

// allocate a block, returns address of alocated block
void* pmmAllocBlock() {
    if (pmmGetBlockCount() <= 0) {
        return 0;  // No available blocks
    }
    int frame = pmmMapFirstFree();
    if (frame == -1) {
        return 0;  // No available blocks
    }
    pmm_set(frame);
    pmm_used_blocks++;
    physaddr addr = frame * PMM_BLOCK_SIZE;
    return (void*)addr;
}

// deallocate a block
void pmmFreeBlock(void *p) {
    physaddr address = (physaddr)p;
    int frame = address / PMM_BLOCK_ALIGN;
    pmm_unset(frame);
    pmm_used_blocks--;
}


int pmmAllocBlocks(size_t num) {
    for (size_t i = 0; i < num; i++) {
        int results[num];
        results[i] = pmmAllocBlock();
        if (results[i] == 0) {
            // There was an error, undo what we've done
            i--;
            while (i != 0) {
                pmmFreeBlocks(results[i]);
                i--;
            }
            return -1;
        }
    }
    return 0;
}

/*============= TODO =============
* pmmAllocBlocks
* pmmFreeBlocks
* Initialize memory regions at startup.
==================================*/