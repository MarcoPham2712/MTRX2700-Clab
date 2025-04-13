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
    if (USART1->ISR & USART_ISR_RXNE) {
        uint8_t incoming_byte = USART1->RDR;

        if (kernel_index < MESSAGE_LENGTH - 1) {
            kernel_buffer[active_kernel][kernel_index++] = incoming_byte;

            if (incoming_byte == message_terminator) {
                finished_transmission(kernel_buffer[active_kernel], kernel_index);
                active_kernel ^= 1;
                kernel_index = 0;
            }
        } else {
            kernel_index = 0;  // buffer overflow handling
        }
    }

    if ((USART1->ISR & USART_ISR_TXE) && is_transmitting) {
        if (transmit_index < transmit_length) {
            USART1->TDR = transmit_buffer[transmit_index++];
        } else {
            USART1->CR1 &= ~USART_CR1_TXEIE;
            is_transmitting = 0;
        }
    }
}



void enable_timers(uint32_t delay_us) {
    __disable_irq();

    // 1. Enable clock for TIM2
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

    // 2. Reset control & disable timer
    TIM2->CR1 = 0;
    TIM2->DIER = 0;
    TIM2->SR = 0;       // Clear interrupt flags
    TIM2->CNT = 0;

    // 3. Set prescaler and ARR
    TIM2->PSC = 8 - 1;         // 1 µs tick
    TIM2->ARR = delay_us;

    // 4. Force update event to preload ARR & PSC into shadow registers
    TIM2->EGR = TIM_EGR_UG;

    // 5. Re-clear UIF after EGR (some MCUs auto-set it)
    TIM2->SR &= ~TIM_SR_UIF;

    // 6. Enable update interrupt
    TIM2->DIER = TIM_DIER_UIE;
    NVIC_SetPriority(TIM2_IRQn, 2);
    NVIC_EnableIRQ(TIM2_IRQn);
    // 7. Only now: Start timer
    TIM2->CR1 |= TIM_CR1_CEN;

    __enable_irq();
}
