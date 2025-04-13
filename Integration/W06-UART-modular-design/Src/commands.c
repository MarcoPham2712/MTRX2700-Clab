#include <stdlib.h>
#include <string.h>
#include "commands.h"
#include "chase_led.h"
#include "stm32f303xc.h"

// External variables for transmission defined elsewhere (interrupt.c)
extern uint8_t transmit_buffer[];
extern uint8_t transmit_index;
extern uint8_t transmit_length;
extern uint8_t is_transmitting;
uint8_t *led_register = ((uint8_t*)&(GPIOE->ODR)) + 1;
void finished_transmission(uint8_t *user_buffer, uint32_t message_length) {
    user_buffer[message_length] = '\0';

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

void led_command(char *operand) {
	TIM2->CR1 &= ~TIM_CR1_CEN;
	TIM2->DIER &= ~(TIM_DIER_UIE | TIM_DIER_CC1IE);
    uint8_t value = (uint8_t)strtol(operand, NULL, 2);
    *led_register = value;
}

void serial_command(char *operand) {
	*led_register = 0;
	TIM2->CR1 &= ~TIM_CR1_CEN;
	TIM2->DIER &= ~(TIM_DIER_UIE | TIM_DIER_CC1IE);
    strcpy((char*)transmit_buffer, operand);
    transmit_length = strlen(operand);
    transmit_index = 0;
    is_transmitting = 1;
    USART1->CR1 |= USART_CR1_TXEIE;
}

void oneshot_command(char *operand){
    // 1. Turn all LEDs ON immediately
    uint8_t *led_register = ((uint8_t*)&(GPIOE->ODR)) + 1;
    *led_register = 0xFF;

    uint32_t arr_value = (uint32_t)strtol(operand, 0x00, 10);

    led_chase_direction('a');
    TIM2->CR1 = TIM_CR1_OPM;  // One-pulse mode
    enable_timers(arr_value*1000);

}

void timer_command(char *operand){

	*led_register = 1;

	TIM2->CR1 &= ~TIM_CR1_OPM;
	uint32_t arr_value = (uint32_t)strtol(operand, 0x00, 10);
	enable_timers(arr_value*1000);
	led_chase_direction('c');


}


