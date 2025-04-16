#include <LED_setup.h>             // Header for LED setup functions
#include <stdint.h>                // Standard library for fixed-width integer types
#include "stm32f303xc.h"           // Device-specific header for STM32F303

// Function pointer for when interrupt occurs
void (*on_button_press)() = 0x00;

void TIM2_IRQHandler(void)
{
    // Check if the update interrupt flag is set
    if ((TIM2->SR & TIM_SR_UIF) != 0){
        TIM2->SR &= ~TIM_SR_UIF;   // Clear the interrupt flag

        // Call function pointer
        on_button_press();
    }
}

// Function to decide which LED pattern to use
void led_chase_direction(char direction)
{
    switch (direction) {
        case 'c':
            on_button_press = &chase_led_c;  // Assign clockwise LED shift
            break;
        case 'a':
            on_button_press = &chase_led_a;  // Assign blinking
            break;
    }
}

void chase_led_c()
{
    // Setting LED pins
    uint8_t *led_register = ((uint8_t*)&(GPIOE->ODR)) + 1;

    *led_register <<= 1;  // Shift LEDs by 1

    // If all LEDs are off then turn the first on
    if (*led_register == 0) {
        *led_register = 1;
    }
}

void chase_led_a()
{
    uint8_t *led_register = ((uint8_t*)&(GPIOE->ODR)) + 1;

    if (*led_register != 0){
        *led_register = 0;       // If any LED is on, turn all off
    }
    else{
        *led_register = 0xFF;    // If all LEDs are off, turn all on
    }
}
