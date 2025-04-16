#include <LED_setup.h>
#include <stdlib.h>
#include <string.h>
#include "commands.h"
#include "interrupt.h"
#include "stm32f303xc.h"

extern uint8_t transmit_buffer[];	// Buffer to transmit with
extern uint8_t transmit_index;
extern uint8_t transmit_length;
extern uint8_t is_transmitting;		// Transmit mode

// Callback function when string is received
void finished_transmission(uint8_t *user_buffer, uint32_t message_length) {
    user_buffer[message_length] = '\0';  // Null terminate the received string

    // Extract the command
    char *command = strtok((char*)user_buffer, " ");
    char *operand = strtok(0x00, "");  // Next token is the operand

    // Decide which command
    if (command) {
        // Compare the command and call right function
        if (strcmp(command, "led") == 0 && operand) {
            led_command(operand);
        } else if (strcmp(command, "serial") == 0 && operand) {
            serial_command(operand);
        } else if (strcmp(command, "oneshot") == 0 && operand) {
            oneshot_command(operand);
        } else if (strcmp(command, "timer") == 0 && operand) {
            timer_command(operand);
        } else {
            // If it isn't a proper command, prompt to try again
            strcpy((char*)transmit_buffer, "Invalid input! Try Again.\r\n");
            transmit_length = strlen((char*)transmit_buffer);
            transmit_index = 0;
            is_transmitting = 1;			 // Set transmit mode
            USART1->CR1 |= USART_CR1_TXEIE;  // Enable transmit interrupt
        }
    }
}

void led_command(char *operand) {
    TIM2->CR1 &= ~TIM_CR1_CEN;  // Disable Timer 2 to stop interrupts
    uint8_t *led_register = ((uint8_t*)&(GPIOE->ODR)) + 1;  // Set LED pins
    uint8_t value = (uint8_t)strtol(operand, 0x00, 2);      // Convert binary string to number
    *led_register = value;  // Output the value to LEDs
}

void serial_command(char *operand) {
    TIM2->CR1 &= ~TIM_CR1_CEN;  // Disable Timer 2 to stop interrupts
    uint8_t *led_register = ((uint8_t*)&(GPIOE->ODR)) + 1;
    *led_register = 0;  // Turn off all LEDs before sending

    strcpy((char*)transmit_buffer, operand);      // Copy operand to transmit buffer
    transmit_length = strlen(operand);            // Set the number of characters to send
    transmit_index = 0;
    is_transmitting = 1;
    USART1->CR1 |= USART_CR1_TXEIE;               // Enable transmit interrupt
}

void oneshot_command(char *operand){
    uint8_t *led_register = ((uint8_t*)&(GPIOE->ODR)) + 1;
    *led_register = 0xFF;  // Turn on all LEDs

    // Convert the operand to a decimal number
    uint32_t arr_value = (uint32_t)strtol(operand, 0x00, 10);

    enable_timers(arr_value);        // Setup timer
    TIM2->CR1 |= TIM_CR1_OPM;        // Enable one-pulse mode
    led_chase_direction('a');        // Flash LED pattern
}

void timer_command(char *operand){
    uint8_t *led_register = ((uint8_t*)&(GPIOE->ODR)) + 1;
    *led_register = 1;  // Turn on first LED

    // Convert string to decimal number
    uint32_t arr_value = (uint32_t)strtol(operand, 0x00, 10);
    enable_timers(arr_value);        // Set auto reload value
    led_chase_direction('c');        // Set LED pattern
}






