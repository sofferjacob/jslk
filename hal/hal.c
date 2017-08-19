#include "hal.h"

extern void startConsole();
extern int start_gdt();

void halInitialize() {
    startConsole();
}