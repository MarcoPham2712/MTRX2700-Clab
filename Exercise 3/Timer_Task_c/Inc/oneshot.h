#ifndef ONESHOT_H
#define ONESHOT_H

#include <stdint.h>

void oneshot_delay_set(int delay);
void oneshot_register_callback(void (*callback)(void));
void LED_oneshot_callback(void);

#endif
