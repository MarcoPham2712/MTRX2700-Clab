#ifndef DIO_H
#define DIO_H

#include <stdint.h>

void DIO_Init(void);
void DIO_ButtonInterruptHandler(void);
void DIO_UpdateLEDs(void);

void DIO_EnableClocks(void);
void DIO_InitialiseBoard(void);
void DIO_EnableInterrupt(void);

#endif // DIO_H
