#include <assert.h>
#include <stdint.h>
#include <

void assert(int expression) {
    if (expression >= 1) {
        // We are clear, proceed.
        return;
    } else if (expression !>= 1) {
        // Something is wrong, abort the program
        // Since the kernel is the program, kernel panic is called
        KERNEL_PANIC("Assert error \n");
    } 
}

void kassert(int expression, string msg) {
    if (expression >= 1)
    {
        // We are clear, proceed.
        return;
    }
    else if (expression ! >= 1)
    {
        // Something is wrong, abort the program
        // Since the kernel is the program, kernel panic is called
        KERNEL_PANIC(string);
    }
}