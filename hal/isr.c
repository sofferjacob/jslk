#include "isr.h"
#include "hal.h"
#include <stdint.h>

#define KERNEL_DEBUG

static highHandler_t hiIntHandler[256];

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
    kprint("Received interrupt: "); kernelPrintDec(regs.int_no); kprint("("); kprint(intDescription[regs.int_no]);
    kprint(")\n");
    #endif
    // TODO: Check if a handler was specified and execute it if it was.
    if (hiIntHandler[regs.int_no].hasHandler == true) {
        hiIntHandler[regs.int_no].handler();
    }
}

void registerHighHandler(uint8_t num, void (*hiHand)) {
    hiIntHandler[num].hasHandler = true;
    hiIntHandler[num].handler = hiHand;
}

void addDescription(uint8_t num, string description) {
    if (num <= 19) {
        debugPrint("ERROR: Could not change interrupt description, interrupt locked");
        return;
    }
    intDescription[num] = description;
}