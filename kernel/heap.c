#include <heap.h>
#include <stdint.h>
#include <stdbool.h>
#include <orderedArray.h>

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

