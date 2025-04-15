
#include "serial.h"
#include "stm32f303xc.h"


void enable_clocks() {
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN | RCC_AHBENR_GPIOCEN | RCC_AHBENR_GPIOEEN;
}
// initialise the discovery board I/O (just outputs: inputs are selected by default)
void initialise_board() {
	// get a pointer to the second half word of the MODER register (for outputs pe8-15)
	uint16_t *led_output_registers = ((uint16_t *)&(GPIOE->MODER)) + 1;
	*led_output_registers = 0x5555;
}

void enable_interrupt() {
    // Temporarily disable interrupts globally during setup
    __disable_irq();

    // -------- USART1 Interrupt setup (highest priority) --------
    USART1->CR1 |= USART_CR1_RXNEIE;  // Enable USART RXNE interrupt

    // Set USART1 interrupt priority to highest (0 is highest in NVIC)
    NVIC_SetPriority(USART1_IRQn, 1);
    NVIC_EnableIRQ(USART1_IRQn);


    // Enabling the interrupts again
    __enable_irq();
}



void enable_timers(uint32_t delay_us) {
    __disable_irq();

    // 1. Enable clock for TIM2
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

    // 2. Reset control & disable timer
    TIM2->CR1 = 0;
    TIM2->DIER = 0;
    TIM2->SR = 0;       // Clear interrupt flags
    TIM2->CNT = 0;

    // 3. Set prescaler and ARR
    TIM2->PSC = 8 - 1;         // 1 µs tick
    TIM2->ARR = delay_us;

    // 4. Force update event to preload ARR & PSC into shadow registers
    TIM2->EGR = TIM_EGR_UG;

    // 5. Re-clear UIF after EGR (some MCUs auto-set it)
    TIM2->SR &= ~TIM_SR_UIF;

    // 6. Enable update interrupt
    TIM2->DIER = TIM_DIER_UIE;
    NVIC_SetPriority(TIM2_IRQn, 2);
    NVIC_EnableIRQ(TIM2_IRQn);
    // 7. Only now: Start timer
    TIM2->CR1 |= TIM_CR1_CEN;

    __enable_irq();
}



// InitialiseSerial - Initialise the serial port
// Input: baudRate is from an enumerated set
void SerialInitialise(uint32_t baudRate, SerialPort *serial_port, void (*completion_function)(uint32_t)) {

	serial_port->completion_function = completion_function;

	// enable clock power, system configuration clock and GPIOC
	// common to all UARTs
	RCC->APB1ENR |= RCC_APB1ENR_PWREN;
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

	// enable the GPIO which is on the AHB bus
	RCC->AHBENR |= serial_port->MaskAHBENR;

	// set pin mode to alternate function for the specific GPIO pins
	serial_port->GPIO->MODER = serial_port->SerialPinModeValue;

	// enable high speed clock for specific GPIO pins
	serial_port->GPIO->OSPEEDR = serial_port->SerialPinSpeedValue;

	// set alternate function to enable USART to external pins
	serial_port->GPIO->AFR[0] |= serial_port->SerialPinAlternatePinValueLow;
	serial_port->GPIO->AFR[1] |= serial_port->SerialPinAlternatePinValueHigh;

	// enable the device based on the bits defined in the serial port definition
	RCC->APB1ENR |= serial_port->MaskAPB1ENR;
	RCC->APB2ENR |= serial_port->MaskAPB2ENR;

	// Get a pointer to the 16 bits of the BRR register that we want to change
	uint16_t *baud_rate_config = (uint16_t*)&serial_port->UART->BRR; // only 16 bits used!

	// Baud rate calculation from datasheet
	switch(baudRate){
	case BAUD_9600:
		// NEED TO FIX THIS !
		*baud_rate_config = 0x341;  // 115200 at 8MHz
		break;
	case BAUD_19200:
		// NEED TO FIX THIS !
		*baud_rate_config = 0x1A1;  // 115200 at 8MHz
		break;
	case BAUD_38400:
		// NEED TO FIX THIS !
		*baud_rate_config = 0xD0;  // 115200 at 8MHz
		break;
	case BAUD_57600:
		// NEED TO FIX THIS !
		*baud_rate_config = 0x8B;  // 115200 at 8MHz
		break;
	case BAUD_115200:
		*baud_rate_config = 0x46;  // 115200 at 8MHz
		break;
	}


	// enable serial port for tx and rx
	serial_port->UART->CR1 |= USART_CR1_TE | USART_CR1_RE | USART_CR1_UE;
}

// instantiate the serial port parameters
//   note: the complexity is hidden in the c file
SerialPort USART1_PORT = {USART1,
		GPIOC,
		RCC_APB2ENR_USART1EN, // bit to enable for APB2 bus
		0x00,	// bit to enable for APB1 bus
		RCC_AHBENR_GPIOCEN, // bit to enable for AHB bus
		0xA00,
		0xF00,
		0x770000,  // for USART1 PC10 and 11, this is in the AFR low register
		0x00, // no change to the high alternate function register
		0x00 // default function pointer is NULL
		};
