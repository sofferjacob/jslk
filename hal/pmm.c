#include "pmm.h"
#include "hal.h"
#include <stdint.h>
#include <string.h>

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


