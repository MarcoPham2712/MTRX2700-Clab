#include "interrupt.h"
#include "commands.h"
#include "stm32f303xc.h"

uint8_t kernel_buffer[2][MESSAGE_LENGTH];  // Two buffers for received messages
uint8_t active_kernel = 0;                 // Tracks which buffer is being written to
uint8_t kernel_index = 0;                  // Index into the active buffer
char message_terminator = '\r';            // Message ends when carriage return is received

uint8_t transmit_buffer[MESSAGE_LENGTH];   // Holds outgoing data
uint8_t transmit_index = 0;                // Current sending position
uint8_t transmit_length = 0;               // Total length of the outgoing message
uint8_t is_transmitting = 0;               // Flag to indicate transmission is active

void USART1_EXTI25_IRQHandler(void) {
    // If data is available to read
    if (USART1->ISR & USART_ISR_RXNE) {
        uint8_t incoming_byte = USART1->RDR;  // Read the received character

        // Check if there's space left in the buffer
        if (kernel_index < MESSAGE_LENGTH - 1) {
            kernel_buffer[active_kernel][kernel_index++] = incoming_byte;  // Store the character

            // Check for terminator
            if (incoming_byte == message_terminator) {
                finished_transmission(kernel_buffer[active_kernel], kernel_index);  // Callback function
                active_kernel ^= 1;     // Swap to the other buffer
                kernel_index = 0;       // Reset buffer index for next message
            }
        } else {
            kernel_index = 0;  // Reset if buffer overflows
        }
    }

    // If ready to transmit data
    if ((USART1->ISR & USART_ISR_TXE) && is_transmitting) {
        if (transmit_index < transmit_length) {
            USART1->TDR = transmit_buffer[transmit_index++];  // Send next character
        } else {
            USART1->CR1 &= ~USART_CR1_TXEIE;  // Disable TX interrupt when done
            is_transmitting = 0;              // Disable transmit
        }
    }
}

void enable_timers(uint32_t delay_us) {
    __disable_irq();  // Temporarily disable global interrupts

    // Enable the clock for TIM2
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

    // Reset timer settings
    TIM2->CR1 = 0;
    TIM2->DIER = 0;
    TIM2->SR = 0;     // Clear all timer interrupt flags
    TIM2->CNT = 0;    // Reset the counter

    TIM2->PSC = 7999;     // 8 MHz / (7999 + 1) = 1 ms tick
    TIM2->ARR = delay_us; // Number of ticks to wait

    // Force update event to set ARR value
    TIM2->EGR = TIM_EGR_UG;

    // Clear interrupts
    TIM2->SR &= ~TIM_SR_UIF;

    // Enable timer update interrupt
    TIM2->DIER = TIM_DIER_UIE;

    NVIC_SetPriority(TIM2_IRQn, 2);     // Set interrupt priority
    NVIC_EnableIRQ(TIM2_IRQn);          // Enable TIM2 interrupt in NVIC

    // Start the timer
    TIM2->CR1 |= TIM_CR1_CEN;

    __enable_irq();  // Re-enable global interrupts
}
