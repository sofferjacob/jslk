#include "descriptor_tables.h"
#include "hal.h"
#include <string.h>
#include <stdbool.h>

// Lets us access our ASM functions from our C code.
extern void gdt_flush(uint32_t);
extern void idt_flush(uint32_t);

// Internal function prototypes.
static void init_gdt();
static void init_idt();
static void gdt_set_gate(uint32_t, uint32_t, uint32_t, uint8_t, uint8_t);
static void idt_set_gate(uint8_t, uint32_t, uint16_t, uint8_t);

gdt_entry_t gdt_entries[3];
gdt_ptr_t gdt_ptr;
bool segmentStatus[3];
const uint8_t gdtMaxSegments = 3;
idt_entry_t idt_entries[256];
idt_ptr_t idt_ptr;

// Initialisation routine - zeroes all the interrupt service routines,
// initialises the GDT and IDT.
void init_descriptor_tables()
{

    // Initialise the global descriptor table.
    init_gdt();
    // Initialise the interrupt descriptor table.
    init_idt();
}

static void init_gdt() {
    gdt_ptr.limit = (sizeof(gdt_entry_t) * 5) - 1;
    gdt_ptr.base = (uint32_t)&gdt_entries;

    gdt_set_gate(0, 0, 0, 0, 0);                // Null segment
    gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); // Code segment
    gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF); // Data segment
    gdt_flush((uint32_t)&gdt_ptr);
}

static void gdt_set_gate(uint32_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t granularity)
{
    if (num > (gdtMaxSegments-1) || segmentStatus[num] == true)
    {
       KERNEL_PANIC("HAL Startup Error: Could not start GDT");
    }
    gdt_entries[num].baseLow = base & 0xffff;
    gdt_entries[num].baseMiddle = (base >> 16) & 0xff;
    gdt_entries[num].baseHigh = (base >> 24) & 0xff;

    gdt_entries[num].limit_low = (limit & 0xFFFF);

    gdt_entries[num].flags = access;
    gdt_entries[num].gran = (limit >> 16) & 0x0f;
    gdt_entries[num].gran |= granularity & 0xf0;

    // segmentStatus[num] = taken;
}

static void init_idt()
{
    idt_ptr.limit = sizeof(idt_entry_t) * 256 - 1;
    idt_ptr.base = (uint32_t)&idt_entries;

    memset(&idt_entries, 0, sizeof(idt_entry_t) * 256);

    idt_set_gate(0, (uint32_t)isr0, 0x08, 0x8E);
    idt_set_gate(1, (uint32_t)isr1, 0x08, 0x8E);
    idt_set_gate(2, (uint32_t)isr2, 0x08, 0x8E);
    idt_set_gate(3, (uint32_t)isr3, 0x08, 0x8E);
    idt_set_gate(4, (uint32_t)isr4, 0x08, 0x8E);
    idt_set_gate(5, (uint32_t)isr5, 0x08, 0x8E);
    idt_set_gate(6, (uint32_t)isr6, 0x08, 0x8E);
    idt_set_gate(7, (uint32_t)isr7, 0x08, 0x8E);
    idt_set_gate(8, (uint32_t)isr8, 0x08, 0x8E);
    idt_set_gate(9, (uint32_t)isr9, 0x08, 0x8E);
    idt_set_gate(10, (uint32_t)isr10, 0x08, 0x8E);
    idt_set_gate(11, (uint32_t)isr11, 0x08, 0x8E);
    idt_set_gate(12, (uint32_t)isr12, 0x08, 0x8E);
    idt_set_gate(13, (uint32_t)isr13, 0x08, 0x8E);
    idt_set_gate(14, (uint32_t)isr14, 0x08, 0x8E);
    idt_set_gate(15, (uint32_t)isr15, 0x08, 0x8E);
    idt_set_gate(16, (uint32_t)isr16, 0x08, 0x8E);
    idt_set_gate(17, (uint32_t)isr17, 0x08, 0x8E);
    idt_set_gate(18, (uint32_t)isr18, 0x08, 0x8E);
    idt_set_gate(19, (uint32_t)isr19, 0x08, 0x8E);
    idt_set_gate(20, (uint32_t)isr20, 0x08, 0x8E);
    idt_set_gate(21, (uint32_t)isr21, 0x08, 0x8E);
    idt_set_gate(22, (uint32_t)isr22, 0x08, 0x8E);
    idt_set_gate(23, (uint32_t)isr23, 0x08, 0x8E);
    idt_set_gate(24, (uint32_t)isr24, 0x08, 0x8E);
    idt_set_gate(25, (uint32_t)isr25, 0x08, 0x8E);
    idt_set_gate(26, (uint32_t)isr26, 0x08, 0x8E);
    idt_set_gate(27, (uint32_t)isr27, 0x08, 0x8E);
    idt_set_gate(28, (uint32_t)isr28, 0x08, 0x8E);
    idt_set_gate(29, (uint32_t)isr29, 0x08, 0x8E);
    idt_set_gate(30, (uint32_t)isr30, 0x08, 0x8E);
    idt_set_gate(31, (uint32_t)isr31, 0x08, 0x8E);

    idt_flush((uint32_t)&idt_ptr);
}

static void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags)
{
    idt_entries[num].base_lo = base & 0xFFFF;
    idt_entries[num].base_hi = (base >> 16) & 0xFFFF;

    idt_entries[num].sel = sel;
    idt_entries[num].always0 = 0;
    // We must uncomment the OR below when we get to using user-mode.
    // It sets the interrupt gate's privilege level to 3.
    idt_entries[num].flags = flags /* | 0x60 */;
}

void callInterrupt(uint8_t num) {
    switch (num) {
        case 0: {
            asm volatile ("int $0x0");
            break;
        }
        case 1: {
            asm volatile("int $0x1");
            break;
        }
        case 2: {
            asm volatile("int $0x2");
            break;
        }
        case 3: {
            asm volatile("int $0x3");
            break;
        }
        case 4:
        {
            asm volatile("int $0x4");
            break;
        }
        case 5:
        {
            asm volatile("int $0x5");
            break;
        }
        case 6:
        {
            asm volatile("int $0x6");
            break;
        }
        case 7:
        {
            asm volatile("int $0x7");
            break;
        }
        case 8:
        {
            asm volatile("int $0x8");
            break;
        }
        case 9:
        {
            asm volatile("int $0x9");
            break;
        }
        case 10:
        {
            asm volatile("int $0x10");
            break;
        }
        case 11:
        {
            asm volatile("int $0x11");
            break;
        }
        case 12:
        {
            asm volatile("int $0x12");
            break;
        }
        case 13:
        {
            asm volatile("int $0x13");
            break;
        }
        case 14:
        {
            asm volatile("int $0x14");
            break;
        }
        case 15:
        {
            asm volatile("int $0x15");
            break;
        }
        case 16:
        {
            asm volatile("int $0x16");
            break;
        }
        case 17:
        {
            asm volatile("int $0x17");
            break;
        }
        case 18:
        {
            asm volatile("int $0x18");
            break;
        }
        case 19:
        {
            asm volatile("int $0x19");
            break;
        }
        case 20:
        {
            asm volatile("int $0x20");
            break;
        }
        case 21:
        {
            asm volatile("int $0x21");
            break;
        }
        case 22:
        {
            asm volatile("int $0x22");
            break;
        }
        case 23:
        {
            asm volatile("int $0x23");
            break;
        }
        case 24:
        {
            asm volatile("int $0x24");
            break;
        }
        case 25:
        {
            asm volatile("int $0x25");
            break;
        }
        case 26:
        {
            asm volatile("int $0x26");
            break;
        }
        case 27:
        {
            asm volatile("int $0x27");
            break;
        }
        case 28:
        {
            asm volatile("int $0x28");
            break;
        }
        case 29:
        {
            asm volatile("int $0x29");
            break;
        }
        case 30:
        {
            asm volatile("int $0x30");
            break;
        }
        case 31:
        {
            asm volatile("int $0x31");
            break;
        }
        default: {
            break;
        }
    }
}