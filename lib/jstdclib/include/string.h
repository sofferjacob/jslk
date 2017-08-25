#ifndef _jlk_string_h
#define _jlk_string_h

#include <stdint.h>
#include <stdlib.h>

size_t strlen(char *str);
void *memset(void *str, int c, size_t n);
typedef char* string;

#endif