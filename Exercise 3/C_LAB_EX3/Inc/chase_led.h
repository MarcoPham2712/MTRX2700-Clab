#ifndef CHASE_LED
#define CHASE_LED

void TIM2_IRQHandler(void);
void led_chase_direction(char direction);
void chase_led_c();
void chase_led_a();
void delay_set(int delay);
#endif
