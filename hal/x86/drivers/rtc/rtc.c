#include "rtc.h"
#include "../../hal.h"

enum {
    RTC_STATUS = 0x0A,
    RTC_SECONDS = 0,
    RTC_MINUTES = 2,
    RTC_HOURS = 4,
    RTC_WEEKDAY = 6,
    RTC_MONTHDAY = 7,
    RTC_MONTH = 8,
    RTC_YEAR = 9
};

bool checkRtcUpdate() {
    outb(0x70, RTC_STATUS);
    return (inb(0x71) & 0x80);
}

uint8_t getRtcReg(int8_t reg) {
    outb(0x70, reg);
    return inb(0x71);
}

rtcTime_t getRtcTime() {
    rtcTime_t result;
    uint8_t seconds;
    uint8_t minutes;
    uint8_t hours;
    uint8_t year;
    uint8_t week_day;
    uint8_t month_day;
    uint8_t month;
    do {
        while (checkRtcUpdate()) {}
        seconds = getRtcReg(RTC_SECONDS);
        minutes = getRtcReg(RTC_MINUTES);
        hours = getRtcReg(RTC_HOURS);
        week_day = getRtcReg(RTC_WEEKDAY);
        month_day = getRtcReg(RTC_MONTHDAY);
        month = getRtcReg(RTC_MONTH);
        year = getRtcReg(RTC_YEAR);
        do {
            result.seconds = seconds;
            result.minutes = minutes;
            result.hours = hours;
            result.week_day = week_day;
            result.month_day = month_day;
            result.month = month;
            result.year = year;
        } while (checkRtcUpdate());
        seconds = getRtcReg(RTC_SECONDS);
        minutes = getRtcReg(RTC_MINUTES);
        hours = getRtcReg(RTC_HOURS);
        week_day = getRtcReg(RTC_WEEKDAY);
        month_day = getRtcReg(RTC_MONTHDAY);
        month = getRtcReg(RTC_MONTH);
        year = getRtcReg(RTC_YEAR);
    } while (seconds != result.seconds ||
             minutes != result.minutes ||
             hours != result.hours ||
             week_day != result.week_day ||
             month_day != result.month_day ||
             month != result.month ||
             year != result.year);
    return result;
}