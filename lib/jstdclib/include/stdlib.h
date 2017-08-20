#ifndef _jstdclib_stdlib_h
#define _jstdclib_stdlib_h

#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned size_t;

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1

#ifdef __cplusplus
#define NULL 0
}
#else
#define NULL (void*)0
#endif

#endif