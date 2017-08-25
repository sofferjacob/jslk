#include "hal.h"
#include "idt.h"
#include "isr.h"
#include <stdint.h>
#include <stdbool.h>

extern void refreshIdt(uint32_t);

idt_descriptor_t idtDesc[256];
idtptr_t idt_ptr;
bool intStatus[256];

static void idtSetGate(uint8_t num, uint8_t base, uint16_t selector, uint8_t flags) {
    if (num > 256 || intStatus[num] == true) {
        kprint("ERROR: Could not create interrupt gate \n");
        return;
    }
    idtDesc[num].baseLo = base & 0xffff;
    idtDesc[num].sel = selector;
    idtDesc[num].reserved = 0;
    idtDesc[num].flags = flags;
    idtDesc[num].baseHi = (base >> 16) & 0xffff;
}

void init_idt() {
    idt_ptr.limit = sizeof(idt_descriptor_t) * 256 -1;
    idt_ptr.base = (uint32_t)&idtDesc;

    memset(&idtDesc, 0, sizeof(idt_descriptor_t)*256);

    idtSetGate(0, (uint32_t)isr0, KCODESEL, IDTK32COMMON);
    idtSetGate(1, (uint32_t)isr1, KCODESEL, IDTK32COMMON);
    idtSetGate(2, (uint32_t)isr2, KCODESEL, IDTK32COMMON);
    idtSetGate(3, (uint32_t)isr3, KCODESEL, IDTK32COMMON);
    idtSetGate(4, (uint32_t)isr4, KCODESEL, IDTK32COMMON);
    idtSetGate(5, (uint32_t)isr5, KCODESEL, IDTK32COMMON);
    idtSetGate(6, (uint32_t)isr6, KCODESEL, IDTK32COMMON);
    idtSetGate(7, (uint32_t)isr7, KCODESEL, IDTK32COMMON);
    idtSetGate(8, (uint32_t)isr8, KCODESEL, IDTK32COMMON);
    idtSetGate(9, (uint32_t)isr9, KCODESEL, IDTK32COMMON);
    idtSetGate(10, (uint32_t)isr10, KCODESEL, IDTK32COMMON);
    idtSetGate(11, (uint32_t)isr11, KCODESEL, IDTK32COMMON);
    idtSetGate(12, (uint32_t)isr12, KCODESEL, IDTK32COMMON);
    idtSetGate(13, (uint32_t)isr13, KCODESEL, IDTK32COMMON);
    idtSetGate(14, (uint32_t)isr14, KCODESEL, IDTK32COMMON);
    idtSetGate(15, (uint32_t)isr15, KCODESEL, IDTK32COMMON);
    idtSetGate(16, (uint32_t)isr16, KCODESEL, IDTK32COMMON);
    idtSetGate(17, (uint32_t)isr17, KCODESEL, IDTK32COMMON);
    idtSetGate(18, (uint32_t)isr18, KCODESEL, IDTK32COMMON);
    idtSetGate(19, (uint32_t)isr19, KCODESEL, IDTK32COMMON);
    idtSetGate(20, (uint32_t)isr20, KCODESEL, IDTK32COMMON);
    idtSetGate(21, (uint32_t)isr21, KCODESEL, IDTK32COMMON);
    idtSetGate(22, (uint32_t)isr22, KCODESEL, IDTK32COMMON);
    idtSetGate(23, (uint32_t)isr23, KCODESEL, IDTK32COMMON);
    idtSetGate(24, (uint32_t)isr24, KCODESEL, IDTK32COMMON);
    idtSetGate(25, (uint32_t)isr25, KCODESEL, IDTK32COMMON);
    idtSetGate(26, (uint32_t)isr26, KCODESEL, IDTK32COMMON);
    idtSetGate(27, (uint32_t)isr27, KCODESEL, IDTK32COMMON);
    idtSetGate(28, (uint32_t)isr28, KCODESEL, IDTK32COMMON);
    idtSetGate(29, (uint32_t)isr29, KCODESEL, IDTK32COMMON);
    idtSetGate(30, (uint32_t)isr30, KCODESEL, IDTK32COMMON);
    idtSetGate(31, (uint32_t)isr31, KCODESEL, IDTK32COMMON);

    refreshIdt((uint32_t)&idt_ptr);
}