#include <stdlib.h>
#include <hal.h> // Kernel header

void abort() {
    // We are the program, halt the kernel.
    void abort() {
        KERNEL_PANIC(__FILE__);
    }
}