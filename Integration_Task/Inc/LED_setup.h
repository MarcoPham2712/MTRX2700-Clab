#ifndef CHASE_LED_H
#define CHASE_LED_H

#include <stdint.h>

void TIM2_IRQHandler(void);
void led_chase_direction(char direction);
void chase_led_c(void);
void chase_led_a(void);

extern void (*on_button_press)(void);

#endif
