#include <LED_setup.h>
#include "stm32f303xc.h"
#include "serial.h"
#include "interrupt.h"
#include "commands.h"



int main(void)
{
    enable_clocks();	// Enable peripherals
    initialise_board();	// Initialise board
    SerialInitialise(BAUD_115200, &USART1_PORT, 0x00);	// Set USART1
    enable_interrupt();	// Enable interrupts

    for (;;);
}
