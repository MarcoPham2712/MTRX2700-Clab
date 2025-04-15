#ifndef INITIALISE_H
#define INITIALISE_H

#include <stdint.h>
#include "serial.h"

// Function pointers for interrupt handlers (defined in main)
void enable_clocks();
// initialise the discovery board I/O (just outputs: inputs are selected by default)
void initialise_board();
void enable_interrupt();

// SerialInitialise - initialise the serial port
// Input: baud rate as defined in the enum
void SerialInitialise(uint32_t baudRate, SerialPort *serial_port, void (*completion_function)(uint32_t));

void enable_timers();
#endif // LED_THING_H
