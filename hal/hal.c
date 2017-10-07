#include "hal.h"
#include "descriptor_tables.h"

extern void startConsole();

void halInitialize() {
    startConsole();
    init_descriptor_tables();
    start_pit(100);
}
