#include "../hal.h"
#include <stdint.h>
#include <stdbool.h>

extern uint32_t end;
uint32_t placement_address = (uint32_t)&end;

static uint32_t kmalloc_main(uint32_t sz, bool aligned, uint32_t* phys) {
    if (aligned == true && (placement_address & 0xfffff000)) {
        placement_address &= 0xffff000;
        placement_address += 0x1000;
    }
    if (phys) {
        *phys = placement_address;
    }
    uint32_t tmp = placement_address;
    placement_address += sz;
    return tmp;
}

uint32_t kmalloc_a(uint32_t sz) {
    return kmalloc_main(sz, true, 0);
}

uint32_t kmalloc_p(uint32_t sz, uint32_t* phys) {
    return kmalloc_main(sz, false, phys);
}

uint32_t kmalloc_ap(uint32_t sz, uint32_t* phys) {
    return kmalloc_main(sz, true, phys);
}

uint32_t kmalloc(uint32_t sz) {
    return kmalloc_main(sz, false, 0);
}