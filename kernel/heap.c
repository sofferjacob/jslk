#include <heap.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include <orderedArray.h>
#include <hal.h>

static int32_t findSmallestHole(uint32_t size, bool pageAlign, heap_t* heap) {
    size_t iterator = 0;
    while (iterator < heap->index.size) {
        header_t* header = (header_t*)lookupOrderedArray(iterator, &heap->index);
        if (pageAlign == true) {
            uint32_t location = (uint32_t)header;
            int32_t offset = 0;
            if ((location+sizeof(header_t)) & 0xFFFFF000 != 0) {
                offset = 0x1000 - (location+sizeof(header_t))%0x1000;
            } 
            int32_t holeSize = (int32_t)header->size - offset;
            if (holeSize >= (int32_t)size) {
                break;
            }
        } else if (header->size >= size) {
            break;
        }
        iterator++;
    }
    if (iterator == heap->index.size) {
        return -1;  // We didn't find a hole
    } else {
        return iterator;
    }
}

static int8_t heatherLessThan(void* a, void* b) {
    return (((header_t*)a)->size < ((header_t*)b)->size)?1:0;
}

heap_t* createHeap(uint32_t start, uint32_t end, uint32_t max, bool supervisor, bool readonly) {
    heap_t* heap = (heap_t*)kmalloc(sizeof(heap_t));
    assert(start%0x1000 == 0);
    assert(end%0x1000 == 0);
    heap->index = placeOrderedArray((void*)start, HEAP_INDEX_SIZE, &headerLessThan);
    start += sizeof(type_t)*HEAP_INDEX_SIZE;
    if (start & 0xFFFFF000 != 0) {
        start &= 0xFFFFF000;
        start += 0x1000;
    }
    heap->startAddress = start;
    heap->endAddress = end;
    heap->maxSize = max;
    heap->supervisor = supervisor;
    heap->readonly = readonly;

    header_t* hole = (header_t*)start;
    hole->size = end-start;
    hole->magic = HEAP_MAGIC;
    hole->isHole = true;
    insertOrderedArray(&heap->index, (void*)hole);

    return heap;
}

static void expand(uint32_t newSize, heap_t* heap) {
    /*
    An assertion isn't really needed, I just added it to push myself to add
    a proper handler in the future.
    */
    assert(newSize > heap->endAddress - heap->startAddress);
    if (newSize & 0xFFFFF000 != 0) {
        // Page-align the address
        newSize &= 0xFFFFF000;
        newSize += 0x1000;
    }
    assert(heap->startAddress + newSize <= heap->maxSize);
    uint32_t oldSize = heap->endAddress-heap->startAddress;
    uint32_t i = oldSize;
    while (i < newSize) {
        alloc_frame(get_page(heap->startAddress+i, 1, kernel_directory), (heap->supervisor)?1:0, (heap->readonly)?0:1);
        i+=0x1000;
    }
    heap->endAddress = heap->startAddress+newSize;
}

static uint32_t contract(uint32_t newSize, heap_t* heap) {
    /*
    An assertion isn't really needed, I just added it to push myself to add
    a proper handler in the future.
    */
    assert(newSize < heap->endAddress - heap->startAddress);
    if (newSize&0x1000) {
        newSize &= 0x1000;
        newSize += 0x1000;
    }
    if (newSize < HEAP_MIN_SIZE) {
        newSize = HEAP_MIN_SIZE;
    }
    uint32_t oldSize = heap->endAddress-heap->startAddress;
    uint32_t i = oldSize - 0x1000;
    while (newSize < i) {
        free_frame(get_page(heap->startAddress+i, 0, kernel_directory));
        i -= 0x1000;
    }
    heap->endAddress = heap->startAddress + newSize;
    return newSize;
}

void* alloc(uint32_t size, bool pageAlign, heap_t* heap) {
    uint32_t newSize = size + sizeof(header_t) + sizeof(footer_t);
    int32_t iterator = findSmallestHole(newSize, pageAlign, heap);
    if (iterator == -1) {
        //
    }
    header_t *origHoleHeader = (header_t *)lookupOrderedArray(iterator, &heap->index);
    uint32_t origHolePos = (uint32_t)origHolePos;
    uint32_t origHoleSize = origHoleHeader->size;
}