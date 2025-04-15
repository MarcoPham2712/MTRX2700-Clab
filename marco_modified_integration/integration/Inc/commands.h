#ifndef COMMANDS_H
#define COMMANDS_H

#include <stdint.h>

void led_command(char *operand);
void serial_command(char *operand);
void oneshot_command(char *operand);
void timer_command(char *operand);
void finished_transmission(uint8_t *user_buffer, uint32_t message_length);

#endif // COMMANDS_H
