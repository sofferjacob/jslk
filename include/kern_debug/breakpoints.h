#ifndef _JSLK_BREAKPOINTS_H
#define _JSLK_BREAKPOINTS_H

void breakpoint(float delay); // Breakpoint that halts execution for a period of time.
void Breakpoint(float delay, char* msg);  // Breakpoint that halts execution for a period of time and displays a message on the console.
void intBreakpoint();   // Breakpoint that halts execution until an ISR is thrown

#endif