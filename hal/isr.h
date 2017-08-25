#ifndef _x86_isr_h
#define _x86_isr_h

#include <string.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct registers
{
   uint32_t ds;                  // Data segment selector
   uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax; // Pushed by pusha.
   uint32_t int_no, err_code;    // Interrupt number and error code (if applicable)
   uint32_t eip, cs, eflags, useresp, ss; // Pushed by the processor automatically.
} registers_t;

typedef struct highHandler {
    bool hasHandler;
    void (*handler)();
} highHandler_t;

// Function prototypes
void registerHighHandler(uint8_t num, void(*hiHand));
void addDescription(uint8_t num, string description);

#endif