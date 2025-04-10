#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include "serial.h"
#include "stm32f303xc.h"

#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif


uint8_t string[100];
uint8_t counter = 0;
char terminator = '\r';


void finished_transmission(uint8_t *rx_string, uint32_t bytes_sent) {
    rx_string[bytes_sent] = '\0';  // null-terminate
    SerialOutputString(rx_string, &USART1_PORT);  // echo back
}



void USART1_EXTI25_IRQHandler(void)
{
    if (USART1->ISR & USART_ISR_RXNE) {
        uint8_t data = USART1->RDR;

        if (counter < sizeof(string) - 1) {
            string[counter++] = data;

            if (data == terminator) {
                finished_transmission(string, counter);
                counter = 0;  // ready for next message
            }
        } else {
            counter = 0;  // overflow protection
        }
    }
}

void enable_interrupt() {
    __disable_irq();

    USART1->CR1 |= USART_CR1_RXNEIE;      // Enable RXNE interrupt
    NVIC_SetPriority(USART1_IRQn, 1);     // Set priority
    NVIC_EnableIRQ(USART1_IRQn);          // Enable IRQ

    __enable_irq();
}

int main(void)
{
    SerialInitialise(BAUD_115200, &USART1_PORT, 0x00);  // callback handled manually
    enable_interrupt();


   for(;;);
}
