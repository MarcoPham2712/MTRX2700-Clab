#include "dio.h"
#include "stm32f303xc.h"

static void (*callback_fn)(void) = 0x00;
static uint8_t bitmask = 0x00;

static uint8_t led_index = 0;
static uint8_t direction_up = 1;  // 1 = increment, 0 = decrement

volatile char ready = 1;


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

	static uint8_t first_press = 1;


    if (ready) {
    	if (first_press) {
			GPIOE->ODR &= ~(0xFF << 8);  // Clear PE8–PE15 once
			first_press = 0;
		}

        ready = 0;             // Lock LED update
        DIO_UpdateLEDs();      // Perform LED chase
        timer(1000);            // Schedule re-enable
    }
}

void DIO_UpdateLEDs(void) {
    if (direction_up) {
        if (led_index < 8) {
            GPIOE->ODR |= (1 << (8 + led_index));
            led_index++;
        }
        if (led_index == 8) {
            direction_up = 0;  // Flip direction only after all ON
            led_index--;       // Immediately prepare to turn one OFF
        }
    } else {
        if (led_index < 8) {  // Safety check: led_index must stay in range 0–7
            GPIOE->ODR &= ~(1 << (8 + led_index));
        }
        if (led_index > 0) {
            led_index--;
        }
        if (led_index == 0) {
            direction_up = 1; // Flip direction only after all OFF
        }
    }
}


// Set the bitmask
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
