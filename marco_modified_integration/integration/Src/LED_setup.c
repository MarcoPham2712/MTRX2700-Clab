#include <LED_setup.h>
#include <stdint.h>
#include "stm32f303xc.h"

// store a pointer to the function that is called when a button is pressed
// set a default value of NULL so that it won't be called until the
// function pointer is defined
void (*on_button_press)() = 0x00;

void TIM2_IRQHandler(void)
{
	// the status register tells us why the interrupt was called
	// UIF is from an overflow type event
	if ((TIM2->SR & TIM_SR_UIF) != 0){
		// set the LED state to be a specific value
		TIM2->SR &= ~TIM_SR_UIF;
		on_button_press();

	}
}

void led_chase_direction(char direction)
{
	TIM2->CNT = 0;
	switch (direction) {
		case 'c':
			on_button_press = &chase_led_c;	// Chasing LED for timer function
			break;
		case 'a':
			on_button_press = &chase_led_a; // Turn on LED for oneshot function
			break;
	}
}

void chase_led_c()
{
	uint8_t *led_register = ((uint8_t*)&(GPIOE->ODR)) + 1;

	*led_register <<= 1; // Shift LED clockwise by 1
	if (*led_register == 0) {
		*led_register = 1; // If shift causes all lights off, turn first LED on
	}
}

void chase_led_a()
{
	uint8_t *led_register = ((uint8_t*)&(GPIOE->ODR)) + 1;

	*led_register =0; // Turn on all LEDs
}
