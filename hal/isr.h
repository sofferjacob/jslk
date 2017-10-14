#include <stdint.h>
#include <string.h>
#include <stdbool.h>

typedef struct registers
{
    uint32_t ds;                                     // Data segment selector
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax; // Pushed by pusha.
    uint32_t int_no, err_code;                       // Interrupt number and error code (if applicable)
    uint32_t eip, cs, eflags, useresp, ss;           // Pushed by the processor automatically.
} registers_t;

// All of this should be migrated to C++ when possible

typedef void (*hiHand_t)();

typedef struct hiInterrupt {
    bool initialized;
    bool hasHandler;
    hiHand_t handler;
    bool isChained;
    bool chainProtect;
} hiInterrupt_t;

typedef struct chainedInterrupt {
    bool initialized;
    uint8_t totalHandlers;
    hiHand_t* handlers;
    bool freeProtect;
    bool chainLock;  // if this is set, no more handlers will be added to this interrupt.
} chainedInterrupt_t;

#define CHAINED_INTERRUPT 1
#define CHAIN_PROTECT     2
#define FREE_PROTECT      3
#define CHAIN_LOCK        4
#define NF                5  // No more flags

#define IRQ0 32
#define IRQ1 33
#define IRQ2 34
#define IRQ3 35
#define IRQ4 36
#define IRQ5 37
#define IRQ6 38
#define IRQ7 39
#define IRQ8 40
#define IRQ9 41
#define IRQ10 42
#define IRQ11 43
#define IRQ12 44
#define IRQ13 45
#define IRQ14 46
#define IRQ15 47