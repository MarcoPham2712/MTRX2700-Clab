#ifndef SERIAL_PORT_HEADER
#define SERIAL_PORT_HEADER

#include <stdint.h>
#include "stm32f303xc.h"

// Defining the serial port struct, the definition is hidden in the
// c file as no one really needs to know this.
struct _SerialPort {
	USART_TypeDef *UART;
	GPIO_TypeDef *GPIO;
	volatile uint32_t MaskAPB2ENR;	// mask to enable RCC APB2 bus registers
	volatile uint32_t MaskAPB1ENR;	// mask to enable RCC APB1 bus registers
	volatile uint32_t MaskAHBENR;	// mask to enable RCC AHB bus registers
	volatile uint32_t SerialPinModeValue;
	volatile uint32_t SerialPinSpeedValue;
	volatile uint32_t SerialPinAlternatePinValueLow;
	volatile uint32_t SerialPinAlternatePinValueHigh;
	void (*completion_function)(uint32_t);
};
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


#endif
