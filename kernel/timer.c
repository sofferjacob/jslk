#include <timer.h>
#include <stdint.h>
#include <hal.h>

static size_t totalTimers = 0;
static timer_t sysTimers[10];

void register_timer(float time, void(*listener)()) {
    size_t time_at_start = readSystemTime();
    float total_time = (time * TICKSTOSEC) + time_at_start;  // Every 100 ticks is a second in x86
    /*while (readSystemTime() < total_time) {
        // Wait for the times to match
    }*/
    // TODO: Use an interrupt to execute listener
    // listener();
    sysTimers[totalTimers].timer = total_time;
    sysTimers[totalTimers].listener = listener;
    sysTimers[totalTimers].timer_status = 0;
    totalTimers += 1;
}

void checkTimers() {
  if (totalTimers <= 0) {
    // There are no set timers
    return;
  }
  for (int i = 0; i < totalTimers; i++) {
    if ((sysTimers[i].timer <= readSystemTime()) && (sysTimers[i].timer_status != 1)) {
      sysTimers[i].timer_status = 1;
      sysTimers[i].listener();
    }
  }
}

void delay(float time) {
    size_t time_at_start = readSystemTime();
    float total_time = (time * TICKSTOSEC) + time_at_start; // Every 100 ticks is a second in x86
    while (readSystemTime() < total_time) {
        // Wait for the times to match
    }
}
