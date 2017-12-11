#ifndef _jlk_string_h
#define _jlk_string_h

#include <stdint.h>
#include <stdlib.h>

typedef char* string;
size_t strlen(char *str);
void *memset(void *str, int c, size_t n);
char *strcat(string dest, const string src);
void append(string str, char c);
void itoa(char *buf, int base, int d);
int strcmp(string s1, string s2);
void memcpy(uint8_t *dest, const uint8_t *src, uint32_t len);
char *strcpy(char *dest, const char *src);

#endif