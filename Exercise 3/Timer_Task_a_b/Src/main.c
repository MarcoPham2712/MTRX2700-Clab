/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Zhenyu Dai
 * @brief          : Main program body
 *
 * This sample code demonstrates an LED chase effect on an STM32 Discovery board.
 * The project is structured into several modules to emphasize modularity and encapsulation:
 *
 *  - initialise.c: Contains board initialization routines, including clock and GPIO configuration.
 *
 *  - interrupt.c: Configures TIM2 to generate a timer interrupt. Only the update (overflow) interrupt
 *    is enabled to ensure that the callback function is triggered only once per timer period.
 *
 *  - chase_led.c: Implements the LED chase logic with two modes (clockwise and anticlockwise).
 *    A function pointer callback mechanism is used to update the LED state in response to timer events.
 *
 * In main.c, the modules are integrated by setting up the LED chase direction, initializing the
 * timer with a specified delay, and then entering the main loop.
 *
 * This design promotes easy maintenance and expansion by isolating functionalities into separate
 * modules and protecting internal variables via encapsulated get/set interfaces.
 *
 * @attention
 *
 * Copyright (c) 2023 STMicroelectronics.
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
#include "interrupt.h"
#include "chase_led.h"

#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif

/**
 * Main program entry point.
 *
 * This function initializes the hardware, sets up the LED chase effect direction,
 * configures the timer delay which triggers the LED update via interrupts,
 * and then enters an infinite loop.
 */
int main(void)
{
    // Enable clocks for required peripherals
    enable_clocks();

    // Initialize board settings
    initialise_board();

    // Choose the LED chase direction:
    // 'c' for clockwise and 'a' for anticlockwise.
    // This determines which LED control function will be registered as a callback.
    char direction = 'c';

    // Set the delay interval for the timer.
    // The value 1000000 represents the timer period in microseconds ( 1000000 us = 1 second).
    int delay = 1000000;

    // Set up the LED chase direction by registering the corresponding callback function.
    // The function led_chase_direction() will use the 'direction' parameter to choose
    // between clockwise and anticlockwise LED patterns.
    led_chase_direction(direction);

    // Configure the timer delay interface.
    // The delay_set() function sets the timer's period using the given 'delay' value.
    // Each timer period triggers a single callback function execution.
    delay_set(delay);

    //infinite loop
    for(;;);
}
