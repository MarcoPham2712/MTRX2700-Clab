#ifndef CHASE_LED_H
#define CHASE_LED_H

#include <stdint.h>

// Function prototypes
void TIM2_IRQHandler(void);
void led_chase_direction(char direction);
void chase_led_c(void);
void chase_led_a(void);

// Pointer to button-press handler function
extern void (*on_button_press)(void);

#endif // CHASE_LED_H
