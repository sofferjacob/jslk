#ifndef x86_rtc_h
#define x86_rtc_h

#include <stdint.h>

typedef struct _rtcTime {
    uint8_t seconds;
    uint8_t minutes;
    uint8_t hours;
    uint8_t week_day;
    uint8_t month_day;
    uint8_t month;
    uint8_t year;
    // uint16_t century;
} rtcTime, rtcTime_t;

rtcTime_t getRtcTime();

#endif