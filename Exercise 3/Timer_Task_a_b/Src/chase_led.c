#include "chase_led.h"
#include <stdint.h>
#include "stm32f303xc.h"
#include "interrupt.h"

/*
 * Global function pointer for the callback function.
 * Initially set to NULL so that it will not be called until defined.
 */
void (*on_button_press)(void) = 0x00;

/**
 * TIM2 Interrupt Handler.
 *
 * This function handles TIM2 interrupts. It checks if the UIF
 * is set in the status register, indicating a timer overflow event.
 * If so, it calls the registered callback function to update the LED state.
 * Finally, it clears the interrupt flag to prevent repeated triggers.
 */
void TIM2_IRQHandler(void)
{
    // Check if the UIF is set overflow event
    if ((TIM2->SR & TIM_SR_UIF) != 0) {
        // If a callback function is registered, call it
        if (on_button_press) {
            on_button_press();
        }
        // Clear the update interrupt flag to avoid re-triggering immediately
        TIM2->SR &= ~TIM_SR_UIF;
    }
}

/**
 * Set LED chase direction.
 *
 * This function configures the direction in which the LED chase effect will operate.
 * It selects the corresponding callback function based on the parameter:
 *  - 'c' for clockwise chase calls chase_led_c
 *  - 'a' for anticlockwise chase calls chase_led_a
 */
void led_chase_direction(char direction)
{
    switch (direction) {
        case 'c':
            on_button_press = &chase_led_c;
            break;
        case 'a':
            on_button_press = &chase_led_a;
            break;
    }
}

/**
 * LED chase clockwise effect.
 *
 * Shifts the LED pattern to the left by one position.
 * If all LEDs are off after the shift, it reinitializes the first LED to on.
 */
void chase_led_c()
{
    // Get pointer to the second byte of GPIOE ODR
    // This pointer controls the LED states on pins PE8-PE15.
    uint8_t *led_register = ((uint8_t*)&(GPIOE->ODR)) + 1;

    *led_register <<= 1;  // Shift the LED pattern left to move the active LED clockwise.

    // If shifting causes all LEDs to be off, reinitialize the pattern by turning on the first LED.
    if (*led_register == 0) {
        *led_register = 1;
    }
}

/**
 * LED chase anticlockwise effect.
 *
 * Shifts the LED pattern to the right by one position.
 * If all LEDs are off after the shift, it reinitializes the first LED to on.
 */
void chase_led_a()
{
    // Get pointer to the second byte of GPIOE ODR
    uint8_t *led_register = ((uint8_t*)&(GPIOE->ODR)) + 1;

    *led_register >>= 1;  // Shift the LED pattern right to move the active LED anticlockwise.

    // If shifting causes all LEDs to be off, reinitialize the pattern by turning on the first LED.
    if (*led_register == 0) {
        *led_register = 1;
    }
}

/**
 * Set the timer delay.
 *
 * Configures the timer's ARR with the provided delay value.
 * This value determines how long the timer counts before triggering an update interrupt.
 * The function enables the necessary interrupts, updates the timer period, and then re-enables global interrupts.
 */
void delay_set(int delay)
{
    // Enable timer interrupts before configuring timer registers
    enable_interrupt();

    // Set the timer's auto-reload register to the desired delay value.
    // For example, a delay of 1000000 corresponds to 1 second if each count equals 1 microsecond.
    TIM2->ARR = delay;

    // Re-enable all interrupts, now that timer configuration is complete.
    __enable_irq();
}
