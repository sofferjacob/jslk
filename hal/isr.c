#include "isr.h"
#include "hal.h"
#include <stdlib.h>
#include <va_list.h> // for when I add flags to registerInterrupt()

#define KERNEL_DEBUG

static hiInterrupt_t hiIntHandler[TOTAL_INTERRUPTS];
static chainedInterrupt_t chainedInterrupts[TOTAL_INTERRUPTS];

static string intDescription[] = {
    "Division by zero",
    "Debug Exception",
    "Non maskable interrupt",
    "Breakpointeption Exception",
    "Into detected overflow",
    "Out of bounds exception",
    "Invalid opcode",
    "No coprocessor exception",
    "Double Fault",
    "Coprocessor segment overrun",
    "Bad TSS",
    "Segment not present",
    "Stack fault",
    "General Protection Fault",
    "Page fault",
    "Unknown interrupt exception",
    "Coprocessor fault",
    "Alignment check exception",
    "Machine check exception",
    "Reserved Interrupt",
    "Reserved Interrupt",
    "Reserved Interrupt",
    "Reserved Interrupt",
    "Reserved Interrupt",
    "Reserved Interrupt",
    "Reserved Interrupt",
    "Reserved Interrupt",
    "Reserved Interrupt",
    "Reserved Interrupt",
    "Reserved Interrupt",
    "Reserved Interrupt",
    "Reserved Interrupt",
};


void isr_handler(registers_t regs) {
    #ifdef KERNEL_DEBUG
    kprint("Received interrupt: "); kernelPrintDec(regs.int_no);
    kprint(" ("); kprint(intDescription[regs.int_no]); kprint(") \n");
    #endif
    if (hiIntHandler[regs.int_no].hasHandler == true)
    {
        hiIntHandler[regs.int_no].handler();
    }
}

int registerInterruptHandler(uint8_t num, void(*hiHand)) {
    if (num > TOTAL_INTERRUPTS) {
        return 1;
    } else if (hiIntHandler[num].isChained == true) {
        return 1;
    } else if (hiIntHandler[num].taken == true) {
        return 1;
    }
    hiIntHandler[num].hasHandler = true;
    hiIntHandler[num].handler = hiHand;
    hiIntHandler[num].isChained = false;
    return 0;
}

int freeInterrupt(uint8_t num) {
    if (hiIntHandler[num].isChained == true) {
        // Call the correct function
        freeChainedInterrupt(num);
    } else if (hiIntHandler[num].hasHandler != true) {
        // The interrupt is already free
        return 0;
    }
    atomicalStart();
    hiIntHandler[num].hasHandler = false;
    hiIntHandler[num].handler = NULL;
    atomicalRelease();
    return 0;
}

int freeChainedInterrupt(uint8_t num) {
    if (chainedInterrupts[num].freeProtect == true) {
        return 1;
    }
    atomicalStart();
    hiIntHandler[num].hasHandler = false;
    hiIntHandler[num].isChained = false;
    hiIntHandler[num].chainProtect = true;
    atomicalRelease();
    if (!chainedInterrupts[num].totalHandlers) {
        chainedInterrupts[num].handlers = NULL;
    }
    while (chainedInterrupts[num].totalHandlers) {
        chainedInterrupts[num].handlers[chainedInterrupts[num].totalHandlers] = NULL;
        if ((chainedInterrupts[num].totalHandlers - 1) == 0) {
            chainedInterrupts[num].handlers = NULL;
        }
        chainedInterrupts[num].totalHandlers--;
    }
    hiIntHandler[num].chainProtect = false;
    return 0;
}

void addDescription(uint8_t num, string description)
{
    if (num <= 19)
    {
        kprint("ERROR: Could not change interrupt description, interrupt locked");
        return;
    }
    intDescription[num] = description;
}

void irq_handler(registers_t regs) {
    // TODO: Interrupt chaining
    if (regs.int_no >= 40) {
        // Send EOI to slave
        outb(0xA0, 0x20);
    }
    // Send EOI to master
    outb(0x20, 0x20);
    if (hiIntHandler[regs.int_no].hasHandler == true)
    {
        hiIntHandler[regs.int_no].handler();
    }
}

uint8_t findFreeInterrupt() {
    atomicalStart();
    for (uint8_t i = 48; i < TOTAL_INTERRUPTS; i++) {
        if (hiIntHandler[i].hasHandler != true) {
            atomicalRelease();
            return i;
        }
    }
    atomicalRelease();
    return 0;
}

void registerChainedInterrupt(uint8_t num, void(*hihand)) {

}



/*
void cpuFaultHandler() {
    atomicalStart();
    uint32_t intno = regs.int_no;
    // Just as a temporary meassure
    switch (intno) {
        case 10:

    }
} */


