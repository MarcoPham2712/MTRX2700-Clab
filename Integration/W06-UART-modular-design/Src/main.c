#include "stm32f303xc.h"
#include "serial.h"
#include "interrupt.h"
#include "commands.h"
#include "chase_led.h"


int main(void)
{
    enable_clocks();
    initialise_board();
    SerialInitialise(BAUD_115200, &USART1_PORT, 0x00);
    enable_interrupt();

    for (;;);
}
