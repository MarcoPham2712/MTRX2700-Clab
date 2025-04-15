#ifndef INTERRUPT_H
#define INTERRUPT_H

#include <stdint.h>

#define MESSAGE_LENGTH 100

void enable_interrupt(void);

// Shared buffers
extern uint8_t kernel_buffer[2][MESSAGE_LENGTH];
extern uint8_t active_kernel;
extern uint8_t kernel_index;
extern char message_terminator;

extern uint8_t transmit_buffer[MESSAGE_LENGTH];
extern uint8_t transmit_index;
extern uint8_t transmit_length;
extern uint8_t is_transmitting;

#endif // INTERRUPT_H
