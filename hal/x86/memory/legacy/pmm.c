#include "pmm.h"
#include "../hal.h"
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

int pmmMapFirstFrees(size_t size) {
    if (size == 0)
        return -1;

    if (size == 1)
        return pmmMapFirstFree();

    for (uint32_t i = 0; i < pmmGetBlockCount() / 32; i++)
        if (pmm_memory_map[i] != 0xffffffff)
            for (int j = 0; j < 32; j++)
            { //! test each bit in the dword

                int bit = 1 << j;
                if (!(pmm_memory_map[i] & bit))
                {

                    int startingBit = i * 32;
                    startingBit += bit; //get the free bit in the dword at index i

                    uint32_t free = 0; //loop through each bit to see if its enough space
                    for (uint32_t count = 0; count <= size; count++)
                    {

                        if (!pmm_test(startingBit + count))
                            free++; // this bit is clear (free frame)

                        if (free == size)
                            return i * 4 * 8 + j; //free count==size needed; return index
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
    int blocks = size / PMM_BLOCK_SIZE;

    for ( ; blocks > 0; blocks--) {
        pmm_set(align++);
        pmm_used_blocks++;
    }
}

// allocate a block, returns address of alocated block
void* pmmAllocBlock() {
    atomicalStart();
    if (pmmGetBlockCount() <= 0) {
        // return 0;  // No available blocks
        system_panic("Not enough memory \n");
    }
    int frame = pmmMapFirstFree();
    if (frame == -1) {
        // return 0;  // No available blocks
        system_panic("Not enough memory \n");
    }
    pmm_set(frame);
    pmm_used_blocks++;
    physaddr addr = frame * PMM_BLOCK_SIZE;
    atomicalRelease();
    return (void*)addr;
}

void* pmmAllocBlocks(size_t size) {
    atomicalStart();
    if (pmmGetBlockCount() <= 0) {
        // return 0;  // Not enough blocks
        system_panic("Not enough memory \n");
    }
    if (size == 1) {
        return pmmAllocBlock;
    } else if (size <= 0) {
        return 0;
    }
    int frame = pmmMapFirstFrees(size);
    for (uint32_t i = 0; i < size; i++) {
        pmm_set(frame + i);
    }
    pmm_used_blocks += size;
    physaddr addr = frame * PMM_BLOCK_ALIGN;
    atomicalRelease();
    return (void*)addr;
}

// deallocate a block
void pmmFreeBlock(void *p) {
    physaddr address = (physaddr)p;
    int frame = address / PMM_BLOCK_ALIGN;
    pmm_unset(frame);
    pmm_used_blocks--;
}

void pmmFreeBlocks(void *p, size_t size) {
    physaddr address = (physaddr)p;
    int frame = address / PMM_BLOCK_ALIGN;
    for (uint32_t i = 0; i < size; i++) {
        pmm_unset(frame + i);
    }
    pmm_used_blocks -= size;
}

uint32_t getPmmSize() {
    return sizeof(pmm_memory_map);
}
