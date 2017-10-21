#include "hal.h"

void atomicalStart() {
    _interrupts(off);
}

void atomicalRelease() {
    _interrupts(on);
}