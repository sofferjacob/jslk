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
    RTC_YEAR = 9,
    RTC_REGB = 0x0B
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
    uint16_t seconds;
    uint16_t minutes;
    uint16_t hours;
    uint16_t year;
    uint16_t week_day;
    uint16_t month_day;
    uint16_t month;
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
            if (!(RTC_REGB & 0x04))
            {
                seconds = (seconds & 0x0F) + ((seconds / 16) * 10);
                minutes = (minutes & 0x0F) + ((minutes / 16) * 10);
                hours = ((hours & 0x0F) + (((hours & 0x70) / 16) * 10)) | (hours & 0x80);
                week_day = (week_day & 0x0F) + ((week_day / 16) * 10);
                month_day = (month_day & 0x0F) + ((month_day / 16) * 10);
                month = (month & 0x0F) + ((month / 16) * 10);
                year = (year & 0x0F) + ((year / 16) * 10);
            }
            if (!(RTC_REGB & 0x02) && (hours & 0x80))
            {
                hours = ((hours & 0x7F) + 12) % 24;
            }
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
        if (!(RTC_REGB & 0x04))
        {
            seconds = (seconds & 0x0F) + ((seconds / 16) * 10);
            minutes = (minutes & 0x0F) + ((minutes / 16) * 10);
            hours = ((hours & 0x0F) + (((hours & 0x70) / 16) * 10)) | (hours & 0x80);
            week_day = (week_day & 0x0F) + ((week_day / 16) * 10);
            month_day = (month_day & 0x0F) + ((month_day / 16) * 10);
            month = (month & 0x0F) + ((month / 16) * 10);
            year = (year & 0x0F) + ((year / 16) * 10);
        }
        if (!(RTC_REGB & 0x02) && (hours & 0x80))
        {
            hours = ((hours & 0x7F) + 12) % 24;
        }
    } while (seconds != result.seconds ||
             minutes != result.minutes ||
             hours != result.hours ||
             week_day != result.week_day ||
             month_day != result.month_day ||
             month != result.month ||
             year != result.year);
    return result;
}

/*
// 0 seconds, 1 minutes, 2 hours, 3 weekday
// 4 month day, 5 month, 6 year
void rtcWrite(uint16_t val, uint8_t pos) {
    if (!(RTC_REGB & 0x04)) {
        if (pos == 2) {
            val = ( (val  0x0F))
        }
    }
}*/