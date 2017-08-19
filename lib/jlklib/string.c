#include <string.h>

size_t strlen(string str) {
    size_t len = 0;
    while (str[len])
        len++;
    return len;
}