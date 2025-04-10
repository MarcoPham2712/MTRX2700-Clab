#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include "serial.h"
#include "stm32f303xc.h"

#if !defined(__SOFT_FP__) && defined(__ARM_FP)
#warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif


#define MAX_LEN 100

uint8_t rx_buffer[2][MAX_LEN];   // double buffer for reception
uint8_t active_rx = 0;           // 0 or 1
uint8_t rx_index = 0;            // current position in active buffer
char terminator = '\r';

uint8_t tx_buffer[MAX_LEN];      // buffer for transmission
uint8_t tx_index = 0;
uint8_t tx_length = 0;
uint8_t tx_sending = 0;


void finished_transmission(uint8_t *rx_string, uint32_t bytes_sent) {
    rx_string[bytes_sent] = '\0';

    // Copy received string to tx_buffer for transmission
    for (uint32_t i = 0; i <= bytes_sent; i++) {
        tx_buffer[i] = rx_string[i];
    }

    tx_length = bytes_sent;
    tx_index = 0;
    tx_sending = 1;

    USART1->CR1 |= USART_CR1_TXEIE;  // Enable TXE interrupt
}


void USART1_EXTI25_IRQHandler(void)
{
    // RX handler
    if (USART1->ISR & USART_ISR_RXNE) {
        uint8_t data = USART1->RDR;

        if (rx_index < MAX_LEN - 1) {
            rx_buffer[active_rx][rx_index++] = data;

            if (data == terminator) {
                // Message complete – call callback
                finished_transmission(rx_buffer[active_rx], rx_index);

                // Switch buffers and reset index
                active_rx ^= 1;
                rx_index = 0;
            }
        } else {
            rx_index = 0;  // overflow protection
        }
    }

    // TX handler
    if ((USART1->ISR & USART_ISR_TXE) && tx_sending) {
        if (tx_index < tx_length) {
            USART1->TDR = tx_buffer[tx_index++];
        } else {
            USART1->CR1 &= ~USART_CR1_TXEIE;  // disable TXE interrupt
            tx_sending = 0;
        }
    }
}


void enable_interrupt() {
    __disable_irq();

    USART1->CR1 |= USART_CR1_RXNEIE;     // Enable RXNE interrupt
    NVIC_SetPriority(USART1_IRQn, 1);
    NVIC_EnableIRQ(USART1_IRQn);

    __enable_irq();
}


int main(void)
{
    SerialInitialise(BAUD_115200, &USART1_PORT, 0x00);  // callback handled manually
    enable_interrupt();

   for(;;);
}
