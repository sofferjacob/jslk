#include "../hal.h"
#include <string.h>

void debugPrint(string msg) {
    #ifdef KERNEL_DEBUG
    kprint(msg);
    #endif
}