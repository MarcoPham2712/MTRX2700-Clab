#include <stdint.h>
#include "serial.h"
#include "interrupt.h"
#include "stm32f303xc.h"

int main(void)
{
    SerialInitialise(BAUD_115200, &USART1_PORT, &finished_transmission);  // Initialise USART1
    enable_interrupt();

    for(;;); // Infinite loop
}
