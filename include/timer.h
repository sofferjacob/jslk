#ifndef _timer_h
#define _timer_h

#include <stdint.h>

typedef struct timers {
  float timer;
  void(*listener)();
  uint8_t timer_status; // 0 = running 1 = done
} timer_t;

void register_timer(float time, void (*listener)());
void delay(float time);

/*
TODO:
+ Remove timers when done
*/

#endif
