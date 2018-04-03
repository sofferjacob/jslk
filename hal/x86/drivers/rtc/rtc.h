#ifndef x86_rtc_h
#define x86_rtc_h

#include <stdint.h>

typedef struct _rtcTime {
    uint16_t seconds;
    uint16_t minutes;
    uint16_t hours;
    uint16_t week_day;
    uint16_t month_day;
    uint16_t month;
    uint16_t year;
    // uint16_t century;
} rtcTime, rtcTime_t;

rtcTime_t getRtcTime();

#endif