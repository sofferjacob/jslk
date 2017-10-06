#include "hal.h"
#include "descriptor_tables.h"

extern void startConsole();
extern void checkTimers();

void halInitialize() {
    startConsole();
    init_descriptor_tables();
    start_pit(100);
}
