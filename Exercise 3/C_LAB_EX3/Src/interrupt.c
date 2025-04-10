#include "interrupt.h"
#include <stdint.h>
#include "stm32f303xc.h"

void enable_interrupt() {
	// Disable the interrupts while messing around with the settings
	//  otherwise can lead to strange behaviour
	__disable_irq();

	// enable the timer 2 through the RCC registers
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

	// set the prescaler so that 1 count is 1 microsecond
	//  8MHz = 0.000000125, 1 microsecond is 0.000001,
	//	prescaler 0.000001/0.000000125 = 8
	TIM2->PSC = 8;  // 1 microsecond / count

	// make the timer2 trigger an interrupt when there is an overflow
	TIM2->DIER |= TIM_DIER_UIE;

	// make the timer2 trigger an interrupt when there is
	//  a successful output compare (on channel 1)
	TIM2->DIER |= TIM_DIER_CC1IE;

	// finally, enable the timer2
	TIM2->CR1 |= TIM_CR1_CEN;

	// Tell the NVIC module that timer2 interrupts should be handled
	NVIC_EnableIRQ(TIM2_IRQn);

	// Enable the output Capture/Compare for channel 1
	TIM2->CCER |= TIM_CCER_CC1E;
	TIM2->CCR1 = 100000; // 50% duty cycle (500000/1000000)

}
