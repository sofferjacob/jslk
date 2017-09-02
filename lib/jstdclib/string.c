#include <string.h>

size_t strlen(char* str) {
    size_t len = 0;
    while (str[len])
        len++;
    return len;
}

void *memset(void *str, int c, size_t n) {
    uint8_t *dest = (uint8_t *)str;
    for (; n != 0; n--) {
        *dest++ = c;
    }
    return str;
}

char *strcat(string dest, const string src) {
    int i = 0;
    int start = strlen(dest);
    int finish = strlen(src);
    while (i < finish) {
        dest[start] = src[i];
        i++;
        start++;
    }
    return dest;
}