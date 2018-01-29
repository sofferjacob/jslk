#include "rtc.h"
#include "../../hal.h"

enum {
    RTC_SECONDS = 0,
    RTC_MINUTES = 2,
    RTC_HOURS = 4,
    RTC_WEEKDAY = 6,
    RTC_MONTHDAY = 7,
    RTC_MONTH = 8,
    RTC_YEAR = 9
};

bool checkRtcUpdate() {
    bool isSet;
    asm volatile ("cli");
    outb(0x70, (0x0A | 0x80));
    isSet = inb(0x71);
    outb(0x70, 0x0);  // just to re-enable NMI's
    asm volatile ("sti");
    return isSet;
}

void dumpRtc(uint16_t* dump) {
    asm volatile ("cli");
    for (uint8_t i = 0; i < 10; i++) {
        outb(0x70, (i|0x80));
        dump[i] = inb(0x71);
    }
    outb(0x70, 0x0);
    asm volatile ("sti");
}

rtcTime getCurrentTime() {
    uint16_t* firstTake;
    uint16_t* secondTake;
    do {
        if (!checkRtcUpdate()) {
            dumpRtc(firstTake);
        }
        if (!checkRtcUpdate()) {
          dumpRtc(secondTake);
        }
    } while ((firstTake[RTC_SECONDS] != secondTake[RTC_SECONDS]) ||
            (firstTake[RTC_MINUTES] != secondTake[RTC_MINUTES]) ||
            (firstTake[RTC_HOURS] != secondTake[RTC_HOURS]) ||
            (firstTake[RTC_WEEKDAY] != secondTake[RTC_WEEKDAY]) ||
            (firstTake[RTC_MONTHDAY] != secondTake[RTC_MONTHDAY]) ||
            (firstTake[RTC_MONTH] != secondTake[RTC_MONTH]) ||
            (firstTake[RTC_YEAR] != secondTake[RTC_YEAR]));
    kprintf("hours: %i \n", secondTake[RTC_HOURS]);
}