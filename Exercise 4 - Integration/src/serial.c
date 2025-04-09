#include "serial.h"

#include "stm32f303xc.h"


//transmitting buffer
volatile uint8_t tx_buffer[TX_BUFFER_SIZE];

//Receiving buffers for double buffer interrupt function
volatile uint8_t buffer1[RX_BUFFER_SIZE];
volatile uint8_t buffer2[RX_BUFFER_SIZE];

// Variables to switch buffers in double buffer implementation
volatile uint8_t *active_buffer = buffer1;
volatile uint8_t *processing_buffer = buffer2;



// We store the pointers to the GPIO and USART that are used
//  for a specific serial port. To add another serial port
//  you need to select the appropriate values.
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
	void (*completion_function)(uint32_t, char *);
};


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


// InitialiseSerial - Initialise the serial port
// Input: baudRate is from an enumerated set
void SerialInitialise(uint32_t baudRate, SerialPort *serial_port, void (*completion_function)(uint32_t, char *)) {

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
		*baud_rate_config = 0x46;  // 115200 at 8MHz
		break;
	case BAUD_19200:
		// NEED TO FIX THIS !
		*baud_rate_config = 0x46;  // 115200 at 8MHz
		break;
	case BAUD_38400:
		// NEED TO FIX THIS !
		*baud_rate_config = 0x46;  // 115200 at 8MHz
		break;
	case BAUD_57600:
		// NEED TO FIX THIS !
		*baud_rate_config = 0x46;  // 115200 at 8MHz
		break;
	case BAUD_115200:
		*baud_rate_config = 0x46;  // 115200 at 8MHz
		break;
	}


	// enable serial port for tx and rx
	serial_port->UART->CR1 |= USART_CR1_TE | USART_CR1_RE | USART_CR1_UE;
}

void finished_transmission(uint32_t bytes_sent, char *sent_string) {
	// This function will be called after a transmission is complete

	volatile uint32_t test = 0;
	// make a very simple delay
	for (volatile uint32_t i = 0; i < 0x8ffff; i++) {
		// waste time !
	}
}

void finished_receiving(uint8_t num_characters, char *received_string){

	int init_byte = received_string[0];

	switch(init_byte){
	case(init_byte == 's'):
		received_string += 7;
		start_interrupt_transmission(&USART1_PORT, received_string, strlen(received_string));
		break;
	case(init_byte == 'l'):

		break;
	case(init_byte == 'o'):

		break;
	case(init_byte == 't'):

		break;
	default:
		uint8_t *error_message = "Invalid string entered. Please try again!\r";
		start_interrupt_tranmission(&USART1_PORT, error_message, strlen(error_message));
		break;
	}


	volatile uint32_t test = 0;
	// make a very simple delay
	for (volatile uint32_t i = 0; i < 0x8ffff; i++) {
		// waste time !
	}
}


void SerialOutputChar(uint8_t data, SerialPort *serial_port) {

	while((serial_port->UART->ISR & USART_ISR_TXE) == 0){
	}

	serial_port->UART->TDR = data;
}

void SerialOutputString(uint8_t *pt, SerialPort *serial_port) {

	uint32_t counter = 0;
	char start_string = pt;
	while(*pt) {
		SerialOutputChar(*pt, serial_port);
		counter++;
		pt++;
	}

	serial_port->completion_function(counter, (char *)start_string);
}

void USART1_EXTI25_IRQHandler(void){

	/* (USART1->ISR & USART_ISR_ORE) {
	    USART1->ICR |= USART_ICR_ORECF; // Clear overrun error flag
	}
	if (USART1->ISR & USART_ISR_NE) {
	    USART1->ICR |= USART_ICR_NCF; // Clear noise error flag
	}*/

	if (USART1_PORT.UART->ISR & USART_ISR_RXNE){

		static uint8_t rx_index = 0;
		char received_byte = USART1_PORT.UART->RDR;

		// Store byte into active buffer if not filled
		if(rx_index < RX_BUFFER_SIZE -1){
			active_buffer[rx_index] = received_byte;
			rx_index++;
		}
		else{
			//Wrap around buffer, overwriting data but avoiding polling
			//situation created by entering completion function here
			//Data overwritten anyway if completion called - too long
			rx_index = 0;
		}

		// Check if terminating character is received
		if(received_byte == '!'){

			uint8_t length = rx_index;
			rx_index = 0;


			volatile uint8_t * temp = active_buffer;
			active_buffer = processing_buffer;
			processing_buffer = temp;

			USART1_PORT.completion_function(length, processing_buffer);

		}

	}

	if (USART1_PORT.UART->ISR & USART_ISR_TXE){
		static uint8_t tx_index = 1;

		if(tx_index < TX_BUFFER_SIZE){
			USART1_PORT.UART->TDR = tx_buffer[tx_index];
			if(tx_buffer[tx_index] == '!'){
				tx_index = 0;
				USART1->CR1 &= ~USART_CR1_TXEIE;
			}
			else{
				tx_index++;
			}
		}
		else{
			tx_index = 0; //in case terminating character is never sent OR buffer overflow
		}
	}

}


void enable_USART_interrupt() {
	// Disable the interrupts while messing around with the settings
	//  otherwise can lead to strange behaviour
	__disable_irq();

	//enable the USART 1 clock
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN;

	//RXNE interrupt enable
	USART1->CR1 |= USART_CR1_RXNEIE;
	USART1->CR1 |= USART_CR1_RE | USART_CR1_TE; //Enable receiver + transmitter
	USART1->CR1 |= USART_CR1_UE; //enable USART


	// Tell the NVIC module that USART1 interrupts should be handled
	NVIC_SetPriority(USART1_IRQn, 1);  // Set Priority
	NVIC_EnableIRQ(USART1_IRQn);

	// Re-enable all interrupts (now that we are finished)
	__enable_irq();
}

void start_interrupt_tranmission(SerialPort *serial_port, uint8_t *data, uint8_t size){
	memcpy(tx_buffer, data, size);
	serial_port->UART->TDR = tx_buffer[0];
	//enable transmission interrupt
	USART1->CR1 |= USART_CR1_TXEIE;
}


void SerialInputChar(uint8_t *data, SerialPort *serial_port){

	while((serial_port->UART->ISR & USART_ISR_RXNE) == 0){
	}
	*data = (uint8_t)(serial_port->UART->RDR);
}

void SerialInputString(uint8_t buffer_size, SerialPort *serial_port, char *buffer){

	uint8_t buffer_counter = 0;
	uint8_t character;
	while (1){
		SerialInputChar(&character, serial_port);
		if (character == '!'){
			break;
		}
		if (buffer_counter < buffer_size-1){
			buffer[buffer_counter] = character;
			buffer_counter++;
		}
	}
	buffer[buffer_counter] = '!';
	serial_port->completion_function(buffer_counter, buffer);
}
