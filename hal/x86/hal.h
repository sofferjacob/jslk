#ifndef _x86_hal_h
#define _x86_hal_h

#ifdef _cplusplus
extern "C" {
#endif

// Include auxiliary header files
#include "io/vga.h"
#include "memory/paging.h"
#include "memory/kheap.h"
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct regs
{
    uint32_t ds;                                     // Data segment selector
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax; // Pushed by pusha.
    uint32_t int_no, err_code;                       // Interrupt number and error code (if applicable)
    uint32_t eip, cs, eflags, useresp, ss;           // Pushed by the processor automatically.
} regs_t;

// Screen I/O
void kputc(char c);
void kprint(char *c);
void writeStyledString(char *c, uint8_t color);
void consolePutChar(char c, uint8_t color, size_t x, size_t y);
void setConsoleColor(uint8_t color);
uint8_t getColor(vga_color text, vga_color background);
void clear_console();
void debugPrint(string msg);
void kernelPrintDec(uint32_t n);
void kernelPrintHex(uint32_t n);
int kprintf(string c, ...);
int cprintf(string c, uint8_t color, ...);
void setMenubarText(string text);
void backspace();
void rightArrow();

// Port I/O
void outb(uint16_t port, uint8_t value);
uint8_t inb(uint16_t port);
uint16_t inw(uint16_t port);

// HAL Functions and Datatypes
void halInitialize();

enum mod {
    on,
    off,
};

typedef enum mod mode_t;

// System Operation Modes
uint8_t _interrupts(mode_t mode);
void _halt();
void _syscritical(mode_t mode);
void system_panic(string msg);
void full_system_panic(string msg, string file, uint32_t line);

// System interrupt handler
#define TOTAL_INTERRUPTS 47
#define CHAINED_INTERRUPT 1
#define CHAIN_PROTECT 2
#define FREE_PROTECT 3
#define CHAIN_LOCK 4
#define NF 5 // No more flags
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
typedef void (*hiHand_t)(regs_t);
int registerInterruptHandler(uint8_t num, hiHand_t handler, ...);
void addDescription(uint8_t num, string description);
void genInterrupt(uint8_t num);
string getintDescription();
uint8_t findFreeInterrupt();
int clearChainedInterrupt(uint8_t num);

// System Timer Related functions
void start_pit(uint32_t freq);
size_t readSystemTime();
#define TICKSTOSEC 100 // 100 ticks = 1 sec at a frequency of 100 Mhz

// Used in the kernel keap
#define KHEAP_START 0xC0000000
#define KHEAP_INITIAL_SIZE 0x100000

/*
// System Physical Memmory Manager (PMM)
typedef uint32_t physaddr; // So it doesn't go out of scope...
int pmmMapFirstFree();  // Returns a free frame
void pmmInit(size_t memSize, physaddr bitmap);  // Initializes the PMM
uint32_t getPmmSize();
void pmmFreeBlocks(void *p, size_t size);
void pmmFreeBlock(void *p);
void *pmmAllocBlocks(size_t size);
void *pmmAllocBlock();
void pmmDeinitRegion(physaddr base, size_t size);
void pmmInitRegion(physaddr base, size_t size);*/

/*
// System Virtual Memory Manager (Paging)
int allocFrame(pageEntry_t* page, bool kernelMode, bool rw);
int freeFrame(pageEntry_t* page);
void initPaging();
void switchPageDirectory(pageDirectory_t* dir);
pageEntry_t* getPage(uint32_t addr, bool make, pageDirectory_t* dir); */

// Kernel First Stage Memory Manager
uint32_t kmalloc(uint32_t sz);  // Vainilla
uint32_t kmalloc_a(uint32_t sz); // Page Aligned Addresses
uint32_t kmalloc_p(uint32_t sz, uint32_t* phys);  // Returns physical address
uint32_t kmalloc_ap(uint32_t sz, uint32_t *phys); // Page Aligned, returns physical address
// kfree() not availabe in this memory maneger!

// Keyboard
void keyboard_install();
static char inbuf[1024];
char* read();

// Other functions
#define PANIC(X) full_system_panic(X, __FILE__, __LINE__)

// Atomical Functions
/*========== WARNING ===========
Use only when necessary and in
quick functions only, else
interrupt latency  and kernel's
performance may be affected
or the kernel may get stuck.
=================================*/
void atomicalStart();
void atomicalRelease();

#ifdef _cplusplus
}
#endif

#endif
