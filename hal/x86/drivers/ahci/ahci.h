#ifndef _AHCI_H
#define _AHCI_H

#include <stdint.h>

typedef enum {
    FIS_TYPE_REG_H2D = 0x27,  // Register FIS, Host to device
    FIS_TYPE_REG_D2H = 0x34,  // Register FIS, Device to host
    
}

#endif