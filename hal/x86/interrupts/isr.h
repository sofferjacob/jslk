#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <hal.h>

typedef struct registers
{
    uint32_t ds;                                     // Data segment selector
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax; // Pushed by pusha.
    uint32_t int_no, err_code;                       // Interrupt number and error code (if applicable)
    uint32_t eip, cs, eflags, useresp, ss;           // Pushed by the processor automatically.
} registers_t;

// All of this should be migrated to C++ when possible
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
