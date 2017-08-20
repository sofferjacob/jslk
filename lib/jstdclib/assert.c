#include <assert.h>
#include <stdint.h>
#include <string.h>

void assert(int expression) {
    if (expression >= 1) {
        // We are clear, proceed.
        return;
    } else if (expression < 1) {
        // Something is wrong, abort the program
        // Since the kernel is the program, kernel panic is called
        asm volatile ("hlt");
    } 
}

void kassert(int expression, char* msg) {
    if (expression >= 1)
    {
        // We are clear, proceed.
        return;
    }
    else if (expression < 1)
    {
        // Something is wrong, abort the program
        // Since the kernel is the program, kernel panic is called
        asm volatile ("hlt");
    }
}