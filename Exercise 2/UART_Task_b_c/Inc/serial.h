#ifndef SERIAL_PORT_HEADER
#define SERIAL_PORT_HEADER

#include <stdint.h>
#include "stm32f303xc.h"

// Defining the serial port struct, the definition is hidden in the
// c file as no one really needs to know this.
struct _SerialPort;
typedef struct _SerialPort SerialPort;

// Make any number of instances of the serial port (they are extern because
// they are fixed, unique values)
extern SerialPort USART1_PORT;

// The user might want to select the baud rate
enum {
  BAUD_9600,
  BAUD_19200,
  BAUD_38400,
  BAUD_57600,
  BAUD_115200
};

// SerialInitialise - initialise the serial port
// Input: baud rate as defined in the enum
void SerialInitialise(uint32_t baudRate, SerialPort *serial_port, void (*completion_function)(uint8_t *, uint32_t));

// SerialOutputChar - output a char to the serial port
//  note: this version waits until the port is ready (not using interrupts)
void SerialOutputChar(uint8_t data, SerialPort *serial_port);

// SerialOutputString - output a NULL TERMINATED string to the serial port
void SerialOutputString(uint8_t *pt, SerialPort *serial_port);

// SerialInputUntil - blocking input until a terminating character
void SerialInputUntil(uint8_t *buffer, uint32_t max_length, char terminator, SerialPort *serial_port);

void enable_clocks();
// initialise the discovery board I/O (just outputs: inputs are selected by default)
void initialise_board();
void enable_interrupt();
#endif
