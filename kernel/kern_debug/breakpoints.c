#include <hal.h>
#include <timer.h>
#include <kern_debug/breakpoints.h>

#define _WBREAKPOINTS

void breakpoint(float tim) {
    #if defined _WBREAKPOINTS
    if (delay <= 0) {
        __halt();
    } else {
        delay(tim);
    }
    #else
    return;
    #endif
} 

void Breakpoint(float tim, char* msg) {
    #if defined _WBREAKPOINTS
    if (delay <= 0) {
        __halt();
    } else {
        delay(tim);
        kprint(msg);
    }
    #else
    return;
    #endif
}

void intBreakpoint() {
    #if defined _WBREAKPOINTS
    kprint("Execution halted by breakpoint, press a key to resume \n");
    asm volatile ("hlt");
    #else
    return;
    #endif
}