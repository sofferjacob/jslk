#ifndef _HEAP_H
#define _HEAP_H

#include <stdint.h>
#include <stdbool.h>
#include "orderedArray.h"

#define HEAP_MAGIC 0x123890AB
#define HEAP_INDEX_SIZE 0x20000
#define HEAP_MIN_SIZE 0x70000

typedef struct header {
    uint32_t magic;
    bool isHole;
    uint32_t size;
} header_t;

typedef struct footer {
    uint32_t magic;
    header_t* header;
} footer_t;

typedef struct heap {
    orderedArray_t index;
    uint32_t startAddress;
    uint32_t endAddress;
    uint32_t maxSize;
    bool supervisor;
    bool readonly;
} heap_t;

heap_t* createHeap(uint32_t start, uint32_t end, uint32_t max, bool supervisor, bool readonly);
void* alloc(uint32_t size, bool pageAlign, heap_t* heap);
void free(void* p, heap_t* heap);

#endif