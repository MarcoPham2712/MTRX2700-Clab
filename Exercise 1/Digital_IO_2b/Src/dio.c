#include "dio.h"
#include "stm32f303xc.h"

static void (*callback_fn)(void) = 0x00;
static uint8_t bitmask = 0x00;


void DIO_EnableClocks(void) {
    // Enable GPIOA (button), GPIOE (LEDs), and SYSCFG (for interrupt config)
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN | RCC_AHBENR_GPIOEEN;
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
}

void DIO_InitialiseBoard(void) {
    // Set PE8–PE15 as output (MODER bits = 01 for each pin)
    // We do it all at once by writing 0x5555 to the upper half of the MODER register
    uint16_t *led_output_registers = ((uint16_t *)&(GPIOE->MODER)) + 1;
    *led_output_registers = 0x5555;

    // Clear all LEDs initially
    GPIOE->ODR &= ~(0xFF << 8);  // Clear bits 8–15
}

void DIO_EnableInterrupt(void) {
    __disable_irq();

    // Connect EXTI0 to PA0
    SYSCFG->EXTICR[0] = SYSCFG_EXTICR1_EXTI0_PA;

    // Trigger interrupt on rising edge for EXTI0 (PA0)
    EXTI->RTSR |= EXTI_RTSR_TR0;
    EXTI->IMR  |= EXTI_IMR_MR0;

    // Enable and prioritize EXTI0 in NVIC
    NVIC_SetPriority(EXTI0_IRQn, 1);
    NVIC_EnableIRQ(EXTI0_IRQn);

    __enable_irq();
}

void DIO_Init(void (*buttonCallback)(void)) {
	callback_fn = buttonCallback;
    DIO_EnableClocks();
    DIO_InitialiseBoard();
    DIO_EnableInterrupt();
}

void DIO_ButtonInterruptHandler(void) {
    if (callback_fn != 0x00) {
        callback_fn();  // Call application logic
    }
    EXTI->PR |= EXTI_PR_PR0;  // Clear interrupt flag
}

// Set the bitmask (e.g., "00110011" = 0x33)
void DIO_SetBitmaskFromString(const char* binaryString) {
    uint8_t mask = 0;
    if (strlen(binaryString) != 8) return;

    for (int i = 0; i < 8; i++) {
        if (binaryString[i] == '1') {
            mask |= (1 << (7 - i));  // MSB on left, LSB on right
        }
    }
    bitmask = mask;
}

uint8_t DIO_GetBitmask(void){
	return bitmask;
}
