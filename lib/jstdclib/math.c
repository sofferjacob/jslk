#include <math.h>
#include <stdint.h>

int exp(uint32_t num, uint32_t potence) {
    for (size_t i = 0; i < num; i++) {
        num *= num;
    }
    return num;
}