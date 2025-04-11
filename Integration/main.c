#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "serial.h"
#include "stm32f303xc.h"

#define MESSAGE_LENGTH 100

uint8_t kernel_buffer[2][MESSAGE_LENGTH];
uint8_t active_kernel = 0;
uint8_t kernel_index = 0;
char message_terminator = '\r';

uint8_t transmit_buffer[MESSAGE_LENGTH];
uint8_t transmit_index = 0;
uint8_t transmit_length = 0;
uint8_t is_transmitting = 0;

// Prototypes for handling each command
void led_command(char *operand);
void serial_command(char *operand);
void oneshot_command(char *operand);
void timer_command(char *operand);

void finished_transmission(uint8_t *user_buffer, uint32_t message_length) {
    user_buffer[message_length] = '\0';

    char *command = strtok((char*)user_buffer, " ");
    char *operand = strtok(0x00, "");

    if (command != 0x00) {
        if (strcmp(command, "led") == 0 && operand != 0x00) {
            led_command(operand);
        } else if (strcmp(command, "serial") == 0 && operand != 0x00) {
            serial_command(operand);
        } else if (strcmp(command, "oneshot") == 0 && operand != 0x00) {
            oneshot_command(operand);
        } else if (strcmp(command, "timer") == 0 && operand != 0x00) {
            timer_command(operand);
        }
    }
}


void USART1_EXTI25_IRQHandler(void)
{
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
            kernel_index = 0;
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



int main(void)
{
	enable_clocks();
	initialise_board();
    SerialInitialise(BAUD_115200, &USART1_PORT, 0x00);
    enable_interrupt();

    for(;;);
}

// Functions for commands
void led_command(char *operand) {
	    // Convert binary string to numeric value
		uint8_t value = (uint8_t)strtol(operand, 0x00, 2);

	    // Assign to LED register (high byte of GPIOE->ODR)
	    uint8_t *led_register = ((uint8_t*)&(GPIOE->ODR)) + 1;
	    *led_register = value;
}

void serial_command(char *operand) {
    strcpy((char*)transmit_buffer, operand);
    transmit_length = strlen(operand);
    transmit_index = 0;
    is_transmitting = 1;

    USART1->CR1 |= USART_CR1_TXEIE;
}

void oneshot_command(char *operand) {

}

void timer_command(char *operand) {

}
