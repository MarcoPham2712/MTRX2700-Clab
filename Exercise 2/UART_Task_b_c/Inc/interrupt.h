#ifndef INTERRUPT_H
#define INTERRUPT_H

#include <stdint.h>

// Sets up RX interrupt
void enable_interrupt(void);

// Optional: expose finished_transmission if used elsewhere
void finished_transmission(uint8_t *rx_string, uint32_t bytes_sent);

#endif // INTERRUPT_H
