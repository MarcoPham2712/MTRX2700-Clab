#include "interrupt.h"
#include "commands.h"
#include "stm32f303xc.h"

uint8_t kernel_buffer[2][MESSAGE_LENGTH];
uint8_t active_kernel = 0;
uint8_t kernel_index = 0;
char message_terminator = '\r';

uint8_t transmit_buffer[MESSAGE_LENGTH];
uint8_t transmit_index = 0;
uint8_t transmit_length = 0;
uint8_t is_transmitting = 0;


void USART1_EXTI25_IRQHandler(void) {
    if (USART1->ISR & USART_ISR_RXNE) {	// check if the RXNE flag is raised
        uint8_t incoming_byte = USART1->RDR;	// Read the string by USART1_RDR then store in the incoming_byte

        // Storing in buffer
        if (kernel_index < MESSAGE_LENGTH - 1) {
            kernel_buffer[active_kernel][kernel_index++] = incoming_byte;

            // If meet the terminating character
            if (incoming_byte == message_terminator) {
            	// Parse and execute the message ---> commands.c
                finished_transmission(kernel_buffer[active_kernel], kernel_index);
                //reset buffer for next message
                active_kernel ^= 1;
                kernel_index = 0;
            }
        } else {
            kernel_index = 0;  // buffer overflow handling
        }
    }

    // Check if the TXE flag is raised
    if ((USART1->ISR & USART_ISR_TXE) && is_transmitting) {
    	// Sending serial character
        if (transmit_index < transmit_length) {
            USART1->TDR = transmit_buffer[transmit_index++];
        } else {
        	// Finish transmitting and clear the flag
            USART1->CR1 &= ~USART_CR1_TXEIE;
            is_transmitting = 0;
        }
    }
}



