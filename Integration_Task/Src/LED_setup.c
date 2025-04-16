#include <LED_setup.h>
#include <stdint.h>
#include "stm32f303xc.h"

//
// Global function pointer for LED pattern callbacks.
// This pointer will be set to a function that executes when the TIM2 interrupt occurs.
//
void (*on_button_press)(void) = 0x00;

/**
 * TIM2 Interrupt Service Routine.
 *
 * This ISR is triggered by TIM2's update event timer overflow. It performs the following:
 *  - Checks the UIF in TIM2's status register.
 *  - Clears the UIF flag to prevent repeated interrupts.
 *  - Calls the function pointed to by on_button_press to update the LED pattern.
 */
void TIM2_IRQHandler(void)
{
    // If the update interrupt flag is set, indicating a timer overflow:
    if ((TIM2->SR & TIM_SR_UIF) != 0)
    {
        TIM2->SR &= ~TIM_SR_UIF; // Clear the update interrupt flag.

        // Execute the registered callback function to update LED state.
        on_button_press();
    }
}

/**
 * Configures the LED chase pattern based on the given direction.
 *
 * This function sets the global function pointer on_button_press to the appropriate
 * LED handling function depending on the parameter:
 *   - 'c': Set for clockwise LED shifting.
 *   - 'a': Set for an alternate LED blinking pattern.
 *
 * @param direction Character indicating the desired LED pattern.
 */
void led_chase_direction(char direction)
{
    switch (direction)
    {
        case 'c':
            on_button_press = &chase_led_c; // Use the clockwise shift pattern.
            break;
        case 'a':
            on_button_press = &chase_led_a; // Use the alternate blinking pattern.
            break;
        default:
            // Optionally, you can set a default pattern or handle invalid input.
            break;
    }
}

/**
 * Clockwise LED chase pattern.
 *
 * This function shifts the current LED pattern one position to the left.
 * If shifting results in all LEDs being off, it resets the pattern by turning on the first LED.
 */
void chase_led_c()
{
    // Get a pointer to the second byte of GPIOE's Output Data Register,
    // which controls the state of LEDs on pins PE8 to PE15.
    uint8_t *led_register = ((uint8_t *)&(GPIOE->ODR)) + 1;

    *led_register <<= 1;  // Shift the LED pattern left by one position.

    // If all LEDs are off after shifting, reset by turning on the first LED.
    if (*led_register == 0)
    {
        *led_register = 1;
    }
}

/**
 * @brief Alternate LED blinking pattern.
 *
 * This function toggles the LEDs in an alternate pattern:
 * - If any LED is currently on, it turns all LEDs off.
 * - If all LEDs are off, it turns all LEDs on.
 */
void chase_led_a()
{
    // Get a pointer to the second byte of GPIOE's Output Data Register.
    uint8_t *led_register = ((uint8_t *)&(GPIOE->ODR)) + 1;

    if (*led_register != 0)
    {
        *led_register = 0;    // Turn all LEDs off if any LED is on.
    }
    else
    {
        *led_register = 0xFF; // Turn all LEDs on if all LEDs are off.
    }
}

