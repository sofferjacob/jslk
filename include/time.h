#ifdef _jslk_time_h
#define _jslk_time_h

#include <stdint.h>

typedef struct Time {
    uint8_t seconds;
    uint8_t minutes;
    uint8_t hours;
    uint8_t weekDay;
    uint8_t monthDay;
    uint8_t month;
    uint8_t year;
} Time_t;

#endif