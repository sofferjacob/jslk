#ifndef _x86_hal_h
#define _x86_hal_h

// Include auxiliary header files
#include "vga.h"
#include <types.h>

// Screen I/O
void kputc(char c);
void kprint(char *c);
void writeStyledString(char *c, uint8_t color);
void consolePutChar(char c, uint8_t color, size_t x, size_t y);
void setConsoleColor(uint8_t color);
uint8_t getColor(vga_color text, vga_color background);
void clear_screen();

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

// Other functions
mode_t readInterruptStatus();
#define KERNEL_PANIC(X) system_panic(X);

#endif