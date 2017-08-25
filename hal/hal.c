#include "hal.h"

extern void startConsole();
// extern int start_gdt();
extern void init_idt();

void halInitialize() {
    startConsole();
    init_idt();
}