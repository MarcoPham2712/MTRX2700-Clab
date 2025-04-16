#include "oneshot.h"
#include "stm32f303xc.h"
#include "interrupt.h"

// Global pointer for the one-shot callback
static void (*oneshot_callback)(void) = 0;

/**
 * TIM2 Interrupt Handler for one-shot operation.
 *
 * When the timer overflow interrupt occurs, this handler
 * calls the registered callback, clears the interrupt flag, and stops the timer,
 * ensuring the callback is executed only once.
 */
void TIM2_IRQHandler(void)
{
    // Check if the update interrupt flag UIF is set.
    if (TIM2->SR & TIM_SR_UIF)
    {
        // Invoke the registered callback function.
        if (oneshot_callback)
        {
            oneshot_callback();
        }
        // Clear the update interrupt flag.
        TIM2->SR &= ~TIM_SR_UIF;
        // Stop TIM2 by clearing the counter enable CEN bit.
        TIM2->CR1 &= ~TIM_CR1_CEN;
    }
}

/**
 * Starts the one-shot timer with a specified delay.
 *
 * This function enables timer interrupts, sets the ARR
 * to the desired delay, resets the counter, and starts TIM2.
 */
void oneshot_delay_set(int delay)
{
    // Enable timer interrupts
    enable_interrupt();

    // Set TIM2's auto-reload register with the desired delay.
    TIM2->ARR = delay;

    // Reset the counter register to start counting from 0.
    TIM2->CNT = 0;

    // Start TIM2 by setting the counter enable bit.
    TIM2->CR1 |= TIM_CR1_CEN;

    // Re-enable global interrupts.
    __enable_irq();
}

/**
 * Registers the one-shot callback function.
 *
 * The callback will be invoked once when the timer expires.
 */
void oneshot_register_callback(void (*callback)(void))
{
    oneshot_callback = callback;
}

/**
 * One-shot timer callback function to flash the LEDs.
 *
 * This function is called when the one-shot timer expires.
 * It turns on all LEDs for approximately 1 second, then turns them off.
 */
void LED_oneshot_callback(void)
{
    // Get pointer to the second byte of GPIOE's Output Data Register (ODR).
    // Assuming the LEDs are connected to bits 0-7 of this byte (PE8-PE15).
    uint8_t *led_register = ((uint8_t *)&(GPIOE->ODR)) + 1;

    // Turn on all LEDs by setting all bits to 1.
    *led_register = 0xFF;

    // Simple delay loop to hold the LED on for ~1 second.
    volatile uint32_t delay;
    for (delay = 0; delay < 1000000; delay++)
    {
        // Busy-wait loop (not ideal in production, but acceptable for a demo).
    }

    // Turn off all LEDs by clearing all bits.
    *led_register = 0x00;
}
