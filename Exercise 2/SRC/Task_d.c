#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include "serial.h"
#include "stm32f303xc.h"

#if !defined(__SOFT_FP__) && defined(__ARM_FP)
#warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif

#define MESSAGE_LENGTH 100


uint8_t kernel_buffer[2][MESSAGE_LENGTH];  	   // Double buffer for receiving
uint8_t active_kernel = 0;                     // Tracking current kernel buffer
uint8_t kernel_index = 0;                      // Current position in active kernel buffer
char message_terminator = '!';				   // Adjustable terminating character

// Transmission buffer
uint8_t transmit_buffer[MESSAGE_LENGTH];
uint8_t transmit_index = 0;
uint8_t transmit_length = 0;
uint8_t is_transmitting = 0;


void finished_transmission(uint8_t *user_buffer, uint32_t message_length) {

	// Null terminate
    user_buffer[message_length] = '\0';

    // Copy the completed message into the transmission buffer
    for (uint32_t i = 0; i <= message_length; i++) {
        transmit_buffer[i] = user_buffer[i];
    }

    transmit_length = message_length;	// Set length of transmission
    transmit_index = 0;					// Begin at the start
    is_transmitting = 1;				// Set to transmission mode

    USART1->CR1 |= USART_CR1_TXEIE;  // Enable transmit interrupt
}


void USART1_EXTI25_IRQHandler(void)
{
    // Receive interrupt
    if (USART1->ISR & USART_ISR_RXNE) {
        uint8_t incoming_byte = USART1->RDR;

        // Only store if there is room in buffer
        if (kernel_index < MESSAGE_LENGTH - 1) {
            kernel_buffer[active_kernel][kernel_index++] = incoming_byte;

            if (incoming_byte == message_terminator) {
                // Message complete so go to callback
                finished_transmission(kernel_buffer[active_kernel], kernel_index);

                // Switch buffers and reset index
                active_kernel ^= 1;
                kernel_index = 0;
            }
        } else {
            kernel_index = 0;  // Reset index if no room
        }
    }

    // Transmit interrupt
    if ((USART1->ISR & USART_ISR_TXE) && is_transmitting) {	// Check if transmit is ready and in transmit mode
        if (transmit_index < transmit_length) {
            USART1->TDR = transmit_buffer[transmit_index++];	// Transmit until end of string
        } else {
            USART1->CR1 &= ~USART_CR1_TXEIE;  // Disable TXE interrupt
            is_transmitting = 0;			  // Leave transmit mode
        }
    }
}


void enable_interrupt() {
    __disable_irq();

    USART1->CR1 |= USART_CR1_RXNEIE;     // Enable RXNE interrupt
    NVIC_SetPriority(USART1_IRQn, 1);	 // Set priority
    NVIC_EnableIRQ(USART1_IRQn);

    __enable_irq();
}


int main(void)
{
    SerialInitialise(BAUD_115200, &USART1_PORT, 0x00);  // Initialise USART1
    enable_interrupt();									// Enable interrupts

   for(;;);												// Loop forever
}
