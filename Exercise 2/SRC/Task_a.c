#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include "serial.h"
#include "movement.h"
#include "stm32f303xc.h"

#if !defined(__SOFT_FP__) && defined(__ARM_FP)
#warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif

// Function pointer for button press handler
void (*on_button_press)() = 0x00;

// Buffer to store the received string
uint8_t received_string[128];  // Adjust size as needed

// Function that sends the received string when the button is pressed
void transmit_on_button(void) {
	SerialOutputString(received_string, &USART1_PORT);
}

// EXTI0 interrupt handler for PA0
void EXTI0_IRQHandler(void)
{
	if (on_button_press != 0x00) {
		on_button_press();
	}
	EXTI->PR |= EXTI_PR_PR0;  // Clear the pending interrupt flag
}

// Set up external interrupt for PA0 (rising edge)
void enable_interrupt() {
	__disable_irq();

	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

	SYSCFG->EXTICR[0] = SYSCFG_EXTICR1_EXTI0_PA;  // Map EXTI0 to PA0
	EXTI->RTSR |= EXTI_RTSR_TR0;                  // Trigger on rising edge
	EXTI->IMR  |= EXTI_IMR_MR0;                   // Unmask EXTI0

	NVIC_SetPriority(EXTI0_IRQn, 1);
	NVIC_EnableIRQ(EXTI0_IRQn);

	__enable_irq();
}

// Called when transmission completes
void finished_transmission(uint32_t bytes_sent) {
	// No delay needed
}

// Main function
int main(void)
{
	// 1. Initialize USART1
	SerialInitialise(BAUD_115200, &USART1_PORT, &finished_transmission);

	// 2. Prompt the user
	// 3. Receive a string until '!' is encountered
	SerialInputUntil(received_string, sizeof(received_string), '\r', &USART1_PORT);

	// 4. Set button action to retransmit the string
	on_button_press = transmit_on_button;

	// 5. Enable external interrupt for the button
	enable_interrupt();

	// 6. Infinite loop – button will trigger sending
	while (1) {
		// Do nothing, wait for interrupts
	}
}
