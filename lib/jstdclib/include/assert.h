#ifndef _jlklib_aseert_h
#define _jlklib_aseert_h

#include <hal.h>

#define assert(b) ((b) ? (void)0 : PANIC("Assertion failed"))

#endif