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

void itoa(char *buf, int base, int d) {
    char *p = buf;
    char *p1, *p2;
    unsigned long ud = d;
    int divisor = 10;

    /*  If %d is specified and D is minus, put `-' in the head. */
    if (base == 'd' && d < 0)
    {
        *p++ = '-';
        buf++;
        ud = -d;
    }
    else if (base == 'x')
        divisor = 16;

    /*  Divide UD by DIVISOR until UD == 0. */
    do
    {
        int remainder = ud % divisor;

        *p++ = (remainder < 10) ? remainder + '0' : remainder + 'a' - 10;
    } while (ud /= divisor);

    /*  Terminate BUF. */
    *p = 0;

    /*  Reverse BUF. */
    p1 = buf;
    p2 = p - 1;
    while (p1 < p2)
    {
        char tmp = *p1;
        *p1 = *p2;
        *p2 = tmp;
        p1++;
        p2--;
    }
}

void append(string str, char c) {
    size_t len = strlen(str);
    str[len] = c;
    str[len+1] = '\0';
}

char* strcat(string dest, const string src) {
    size_t slen = strlen(src);
    size_t dlen= strlen(dest);
    for (size_t i = 0; i < slen; i++) {
        dest[dlen+i] = src[i];
    }
    dlen = strlen(dest);
    dest[dlen - 1] = '\0';
    return &dest[0];
}

int strcmp(string s1, string s2) {
    int i;
    for (i = 0; s1[i] == s2[i]; i++) {
        if (s1[i] == '\0') return 0;
    }
    return s1[i] - s2[i];
}

char *strcpy(char *dest, const char *src)
{
    do
    {
        *dest++ = *src++;
    } while (*src != 0);
}

// Copy len bytes from src to dest.
void memcpy(uint8_t *dest, const uint8_t *src, uint32_t len)
{
    const uint8_t *sp = (const uint8_t *)src;
    uint8_t *dp = (uint8_t *)dest;
    for (; len != 0; len--)
        *dp++ = *sp++;
}