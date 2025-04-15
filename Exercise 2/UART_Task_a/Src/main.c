#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include "serial.h"
#include "stm32f303xc.h"

#if !defined(__SOFT_FP__) && defined(__ARM_FP)
#warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif

// Buffer to store the string
uint8_t received_string[128];

// Completion function when receiving and transmitting finishes
void finished_transmission(uint32_t bytes_sent) {
}

int main(void)
{
	// Initialise USART1
	SerialInitialise(BAUD_115200, &USART1_PORT, &finished_transmission);

	// Read the string
	SerialInputUntil(received_string, sizeof(received_string), '\r', &USART1_PORT);

	// Transmit the string back
	SerialOutputString(received_string, &USART1_PORT);

	for(;;) {}
}
