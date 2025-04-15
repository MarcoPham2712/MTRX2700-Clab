#include <LED_setup.h>
#include <stdlib.h>
#include <string.h>
#include "commands.h"
#include "stm32f303xc.h"

// External variables for transmission defined elsewhere (interrupt.c)
extern uint8_t transmit_buffer[];	// Buffer holding message
extern uint8_t transmit_index;		// Tracking current index of sending
extern uint8_t transmit_length;		// Length of message
extern uint8_t is_transmitting;		// Treated like a flag indicating the transmission is active
uint8_t *led_register = ((uint8_t*)&(GPIOE->ODR)) + 1;	// Storing LED pattern

void finished_transmission(uint8_t *user_buffer, uint32_t message_length) {
    user_buffer[message_length] = '\0';	// Setup the received string buffer as NULL

    // Splitting string for command and operand
    char *command = strtok((char*)user_buffer, " ");
    char *operand = strtok(0x00, "");

    if (command) {
        if (strcmp(command, "led") == 0 && operand) {
            led_command(operand);
        } else if (strcmp(command, "serial") == 0 && operand) {
            serial_command(operand);
        } else if (strcmp(command, "oneshot") == 0 && operand) {
            oneshot_command(operand);
        }else if (strcmp(command, "timer") == 0 && operand) {
            timer_command(operand);
        }
    }
}

// LED MODE
void led_command(char *operand) {
	// Disable timer
	TIM2->CR1 &= ~TIM_CR1_CEN;
	TIM2->DIER &= ~(TIM_DIER_UIE | TIM_DIER_CC1IE);

	// Convert string to uint8_t, saved as LED pattern
    uint8_t value = (uint8_t)strtol(operand, NULL, 2);
    *led_register = value;
}

// SERIAL MODE
void serial_command(char *operand) {
	*led_register = 0; // Clear the state of LED
	// Disable timer
	TIM2->CR1 &= ~TIM_CR1_CEN;
	TIM2->DIER &= ~(TIM_DIER_UIE | TIM_DIER_CC1IE);

	// Load operand into the transmitted buffer
    strcpy((char*)transmit_buffer, operand);
    transmit_length = strlen(operand);
    transmit_index = 0;
    is_transmitting = 1;

    // Enabling UART transmit interrupt
    USART1->CR1 |= USART_CR1_TXEIE;
}

// ONESHOT MODE
void oneshot_command(char *operand){
    // 1. Turn all LEDs ON immediately
    uint8_t *led_register = ((uint8_t*)&(GPIOE->ODR)) + 1;
    *led_register = 0xFF;

    // Value of oneshot mode by splitting
    uint32_t arr_value = (uint32_t)strtol(operand, 0x00, 10);

    led_chase_direction('a');
    TIM2->CR1 = TIM_CR1_OPM;  // One-pulse mode
    enable_timers(arr_value*1000);

}

// TIMER MODE
void timer_command(char *operand){
	// Initial LED pattern
	*led_register = 1;


	TIM2->CR1 &= ~TIM_CR1_OPM;

	// Get delay value by splitting
	uint32_t arr_value = (uint32_t)strtol(operand, 0x00, 10);
	enable_timers(arr_value*1000);
	led_chase_direction('c');


}


