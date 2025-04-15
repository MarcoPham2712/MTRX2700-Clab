#include <stdint.h>
#include <stdlib.h>
#include "stm32f303xc.h"
#include "serial.h"

// Buffer to store string
uint8_t string[100];
// Counter to track length of string
uint8_t counter = 0;
// Terminating character
char terminator = '\r';

void finished_transmission(uint8_t *rx_string, uint32_t bytes_sent) {
    rx_string[bytes_sent] = '\0';  // Null terminate the string
    SerialOutputString(rx_string, &USART1_PORT);  // Print the string to the terminal
}

void USART1_EXTI25_IRQHandler(void)
{
    // Check if ready to receive
    if (USART1->ISR & USART_ISR_RXNE) {
        // Read the incoming character
        uint8_t data = USART1->RDR;
        // Check for buffer overflow
        if (counter < sizeof(string) - 1) {
            // Store the character and increment the counter
            string[counter++] = data;
            // Check for terminator
            if (data == terminator) {
                // Call the callback function
                finished_transmission(string, counter);
                // Reset counter for the next string
                counter = 0;
            }
        } else {
            // Reset counter
            counter = 0;
        }
    }
}
void enable_interrupt() {
    __disable_irq();  // Disable interrupts

    USART1->CR1 |= USART_CR1_RXNEIE;      // Enable RX interrupt
    NVIC_SetPriority(USART1_IRQn, 1);     // Set priority for USART1 interrupt
    NVIC_EnableIRQ(USART1_IRQn);          // Enable USART1 interrupt in NVIC

    __enable_irq();   // Re-enable interrupts
}
