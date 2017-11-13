#include "isr.h"
#include "../hal.h"
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

void chainedIsrHandler(regs_t regs) {
    if (chainedInterrupts[regs.int_no].totalHandlers == 0) {
        chainedInterrupts[regs.int_no].handlers[0](regs);
        return;
    }
    for (size_t i = 0; i < chainedInterrupts[regs.int_no].totalHandlers + 1; i++) {
        chainedInterrupts[regs.int_no].handlers[i](regs);
    }
}

void isr_handler(regs_t regs) {
    #ifdef KERNEL_DEBUG
    kprint("Received interrupt: "); kernelPrintDec(regs.int_no);
    kprint(" ("); kprint(intDescription[regs.int_no]); kprint(") \n");
    #endif
    // Check if we are dealing with a chained interrupt
    if (hiIntHandler[regs.int_no].isChained == true) {
        chainedIsrHandler(regs);
    } else if (hiIntHandler[regs.int_no].hasHandler == true) {
        hiIntHandler[regs.int_no].handler(regs);
    }
}

/*
int registerInterruptHandler(uint8_t num, void(*hiHand)) {
    if (num > TOTAL_INTERRUPTS) {
        return 1;
    } else if (hiIntHandler[num].isChained == true) {
        return 1;
    }
    hiIntHandler[num].hasHandler = true;
    hiIntHandler[num].handler = hiHand;
    hiIntHandler[num].isChained = false;
    return 0;
}*/

int registerInterruptHandler(uint8_t num, hiHand_t handler, ...) {
    atomicalStart();
    if (num > TOTAL_INTERRUPTS)
    {
        atomicalRelease();
        return 1;
    }
    va_list args;
    va_start(args, handler);
    size_t totalFlags = 0;
    int flags = 0;
    bool isChained, chainProtect, freeProtect, chainLock;
    while (flags != 5)
    {
        flags = va_arg(args, int);
        switch (flags)
        {
        case 1:
            isChained = true;
            break;
        case 2:
            chainProtect = true;
            break;
        case 3:
            freeProtect = true;
            break;
        case 4:
            chainLock = true;
            break;
        default:
            break;
        }
        if (++totalFlags > 5)
        {
            break;
        }
    }
    va_end(args);
    // Check if we should create a chained interrupt
    if (isChained == true)
    {
        // Check if the specified interrupt is available
        if ((hiIntHandler[num].hasHandler == true) && (hiIntHandler[num].isChained != true) && (hiIntHandler[num].chainProtect == true))
        {
            atomicalRelease();
            return 1;
        }
        // Check if the specified interrupt can be chained
        if ((hiIntHandler[num].chainProtect == true) || (chainedInterrupts[num].chainLock == true))
        {
            // The specified interrupt cannot be chained, return.
            atomicalRelease();
            return 1;
        }
        hiIntHandler[num].hasHandler = true; // The interrupt is now ours!
        hiIntHandler[num].isChained = true;
        if (hiIntHandler[num].initialized == true)
        {
            chainedInterrupts[num].handlers[0] = hiIntHandler[num].handler;
            chainedInterrupts[num].totalHandlers = 0;
            chainedInterrupts[num].initialized = true;
        }
        if (chainedInterrupts[num].initialized != true)
        {
            chainedInterrupts[num].initialized = true;
            chainedInterrupts[num].totalHandlers = 0;
        }
        else
        {
            chainedInterrupts[num].totalHandlers++;
        }
        // Now lets add the handler to the interrupt
        chainedInterrupts[num].handlers[chainedInterrupts[num].totalHandlers] = handler;
        // And check if we need to set any  flags
        if (freeProtect == true)
        {
            chainedInterrupts[num].freeProtect = true;
        }
        if (chainLock == true)
        {
            chainedInterrupts[num].chainLock = true;
        }
        atomicalRelease();
        return 0;
    }
    else
    {
        if ((hiIntHandler[num].hasHandler == true) || (hiIntHandler[num].isChained == true))
        {
            atomicalRelease();
            return 1;
        }
        hiIntHandler[num].hasHandler = true;
        hiIntHandler[num].handler = handler;
        hiIntHandler[num].isChained = false;
        hiIntHandler[num].initialized = true;
        if (chainProtect = true)
        {
            hiIntHandler[num].chainProtect = true;
        }
        atomicalRelease();
        return 0;
    }
    atomicalRelease();
    return 1;
}

// Function prototype
int clearChainedInterrupt(uint8_t num);

int clearInterrupt(uint8_t num) {
    if (hiIntHandler[num].isChained == true) {
        // Call the correct function
        clearChainedInterrupt(num);
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

int clearChainedInterrupt(uint8_t num) {
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
    chainedInterrupts[num].initialized = false;
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

void irq_handler(regs_t regs) {
    // TODO: Interrupt chaining
    if (regs.int_no >= 40) {
        // Send EOI to slave
        outb(0xA0, 0x20);
    }
    // Send EOI to master
    outb(0x20, 0x20);
    if (hiIntHandler[regs.int_no].hasHandler == true)
    {
        hiIntHandler[regs.int_no].handler(regs);
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

/*
void cpuFaultHandler() {
    atomicalStart();
    uint32_t intno = regs.int_no;
    // Just as a temporary meassure
    switch (intno) {
        case 10:

    }
} */


