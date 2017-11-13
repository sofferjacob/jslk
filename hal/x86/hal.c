#include "hal.h"
#include "tables/descriptor_tables.h"

extern void startConsole();

void halInitialize() {
    startConsole();
    init_descriptor_tables();
    start_pit(100);
}
