#include "timer.h"
#include "dio.h"
#include "stm32f303xc.h"

// store a pointer to the timer that is called when TIM2 is run out
void (*time_out)() = 0x00;
volatile char state = '\0';  // Initial definition and value of state.
extern volatile char ready;

// Modify the Interrupt Service Routine (ISR) for status switching and callback logic.
void TIM2_IRQHandler(void) {
    if ((TIM2->SR & TIM_SR_UIF) != 0) { // Check if the update interrupt flag is set.
        TIM2->SR &= ~TIM_SR_UIF; // Clear the interrupt flag.
        ready = 1;
    }
}

void set_prescaler(int delay){
	TIM2->CR1 |= TIM_CR1_CEN; // Enable the timer.
	TIM2->PSC = 0x07; // Set the prescaler value.

	TIM2->ARR = 0x01; // Tempsaorary setting for the Auto-reload register.
	TIM2->CNT = 0x00; // Reset the counter.
	asm("NOP"); // No operation (NOP) instructions for timing.
	asm("NOP");
	asm("NOP");
	TIM2->ARR = 0xffffffff; // Restore the Auto-reload register to its maximum value.

	// Disable the interrupts while messing around with the settings
	//  otherwise can lead to strange behaviour
	__disable_irq();

	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN; // Ensure timer 2 clock is enabled.
	TIM2->PSC = 7; // Set the prescaler for the timer.
	TIM2->DIER |= TIM_DIER_UIE; // Enable update interrupt.
	TIM2->CR1 |= TIM_CR1_CEN; // Enable the timer.
	NVIC_EnableIRQ(TIM2_IRQn); // Enable timer 2 interrupt in the NVIC.
	TIM2->ARR = delay*1000; // Initial setting for 3 seconds interrupt.
	// Re-enable all interrupts (now that we are finished)
	__enable_irq();
}

void timer(int delay){
	TIM2->CR1 |= TIM_CR1_CEN; // Enable the timer.
	TIM2->PSC = 0x07; // Set the prescaler value.

	TIM2->ARR = 0x01; // Temporary setting for the Auto-reload register.
	TIM2->CNT = 0x00; // Reset the counter.
	asm("NOP"); // No operation (NOP) instructions for timing.
	asm("NOP");
	asm("NOP");
	TIM2->ARR = 0xffffffff; // Restore the Auto-reload register to its maximum value.

	// Disable the interrupts while messing around with the settings
	//  otherwise can lead to strange behaviour
	__disable_irq();
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN; // Ensure timer 2 clock is enabled.
	TIM2->PSC = 7; // Set the prescaler for the timer.
	TIM2->DIER |= TIM_DIER_UIE; // Enable update interrupt.
	TIM2->CR1 |= TIM_CR1_CEN; // Enable the timer.
	NVIC_EnableIRQ(TIM2_IRQn); // Enable timer 2 interrupt in the NVIC.
	TIM2->ARR = delay*1000; // Initial setting for 3 seconds interrupt.
	// Re-enable all interrupts (now that we are finished)
	__enable_irq();
	state = 't';
}

void one_shot(int delay){
	TIM2->CR1 |= TIM_CR1_CEN; // Enable the timer.
	TIM2->PSC = 0x07; // Set the prescaler value.

	TIM2->ARR = 0x01; // Temporary setting for the Auto-reload register.
	TIM2->CNT = 0x00; // Reset the counter.
	asm("NOP"); // No operation (NOP) instructions for timing.
	asm("NOP");
	asm("NOP");
	TIM2->ARR = 0xffffffff; // Restore the Auto-reload register to its maximum value.

	// Disable the interrupts while messing around with the settings
	//  otherwise can lead to strange behaviour
	__disable_irq();

	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN; // Ensure timer 2 clock is enabled.
	TIM2->PSC = 7; // Set the prescaler for the timer.
	TIM2->DIER |= TIM_DIER_UIE; // Enable update interrupt.
	TIM2->CR1 |= TIM_CR1_CEN; // Enable the timer.
	NVIC_EnableIRQ(TIM2_IRQn); // Enable timer 2 interrupt in the NVIC.
	TIM2->ARR = delay*1000; // Initial setting for 3 seconds interrupt.
	// Re-enable all interrupts (now that we are finished)
	__enable_irq();
	uint8_t *led_register = ((uint8_t*)&(GPIOE->ODR)) + 1;
	*led_register = 1;
	state = 'o';
}


void dis_enable_timer_interrupt(){
	// Disable the interrupts while messing around with the settings
	//  otherwise can lead to strange behaviour
	__disable_irq();

	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN; // Ensure timer 2 clock is enabled.
	TIM2->PSC = 50000; // Set the prescaler for the timer.
	TIM2->DIER |= TIM_DIER_UIE; // Enable update interrupt.
	TIM2->CR1 |= TIM_CR1_CEN; // Enable the timer.
	TIM2->ARR = 0xffffff; // Initial setting for 3 seconds interrupt.
	// Re-enable all interrupts (now that we are finished)
	__enable_irq();
}
