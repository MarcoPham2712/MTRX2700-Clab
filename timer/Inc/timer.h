#ifndef TIMER_H
#define TIMER_H

#include "stm32f303xc.h"

extern volatile char state;  // Declaration of the global state variable.

extern void (*time_out)();

void TIM2_IRQHandler(void);

void set_prescaler(int delay);

void one_shot(int delay);

void timer(int delay);

void dis_enable_timer_interrupt();

#endif
