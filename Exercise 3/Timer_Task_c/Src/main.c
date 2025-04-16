/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Zhenyu Dai
 * @brief          : Main program body
 *
 * This code demonstrates the one-shot timer functionality on an STM32 board.
 * When the timer expires (after a specified delay), the registered callback function
 * is invoked, which turns on all the LEDs (connected to PE8-PE15) for 1 second and then turns them off.
 *
 * @attention
 *
 * Copyright (c) 2025 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */

#include <stdint.h>
#include "stm32f303xc.h"
#include "initialise.h"
#include "oneshot.h"

#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif

/**
 * Main program entry point.
 *
 * The program initializes the board and one-shot timer module.
 * It then registers the one-shot callback and sets the timer delay.
 * When the timer expires, the callback function is executed, flashing the LEDs.
 */
int main(void)
{
    // Enable clocks for peripherals
    enable_clocks();

    // Initialize board settings
    initialise_board();

    // Initially ensure that all LEDs are turned off.
    uint8_t *led_register = ((uint8_t *)&(GPIOE->ODR)) + 1;
    *led_register = 0x00;  // All LEDs off.

    // Register the one-shot timer callback that will flash the LEDs.
    oneshot_register_callback(LED_oneshot_callback);

    // Set the one-shot timer delay value.
    // For example, 1000000 microseconds corresponds to a 1-second delay.
    oneshot_delay_set(1000000);

    for(;;);
}
