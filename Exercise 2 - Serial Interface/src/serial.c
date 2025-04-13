#include "serial.h"
#include "stm32f303xc.h"


//Transmitting buffer
volatile uint8_t tx_buffer[TX_BUFFER_SIZE];

//Buffers for double buffer interrupt function
volatile uint8_t buffer1[RX_BUFFER_SIZE];
volatile uint8_t buffer2[RX_BUFFER_SIZE];

//Pointers to switch buffers in double buffer implementation
volatile uint8_t *active_buffer = buffer1;
volatile uint8_t *processing_buffer = buffer2;

//Flags for processing data and transmitting data
volatile uint8_t processing_flag = 1;
volatile uint8_t transmitting = 0;

// Serial Port and USART struct type to enable different ports
// Note only one completion function per UART can be selected
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

// Instantiate USART1 serial port parameters and struct
SerialPort USART1_PORT = {USART1,
		GPIOC,
		RCC_APB2ENR_USART1EN, 	// bit to enable for APB2 bus
		0x00,			// bit to enable for APB1 bus
		RCC_AHBENR_GPIOCEN, 	// bit to enable for AHB bus
		0xA00,			// Pin mod value for alternate function
		0xF00,			// High speed setting 
		0x770000,  		// for USART1 PC10 and 11, this is in the AFR low register
		0x00, 			// no change to the high alternate function register
		0x00 			// default function pointer is NULL
		};


// InitialiseSerial - function to initialise serial port
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

	// Baud rates to select
	switch(baudRate){
	case BAUD_9600:
		*baud_rate_config = 0x46;  // 115200 at 8MHz
		break;
	case BAUD_19200:
		*baud_rate_config = 0x46;  // 115200 at 8MHz
		break;
	case BAUD_38400:
		*baud_rate_config = 0x46;  // 115200 at 8MHz
		break;
	case BAUD_57600:
		*baud_rate_config = 0x46;  // 115200 at 8MHz
		break;
	case BAUD_115200:
		*baud_rate_config = 0x46;  // 115200 at 8MHz
		break;
	}

	// enable serial port for tx and rx
	serial_port->UART->CR1 |= USART_CR1_TE | USART_CR1_RE | USART_CR1_UE;
}

//Function to transmit a byte into the TDR to trigger a transmission interrupt
void start_interrupt_tranmission(SerialPort *serial_port, uint8_t *data, uint8_t size){
	
	//Copy the data into the transmission buffer
	memcpy(tx_buffer, data, size);
	serial_port->UART->TDR = tx_buffer[0];
	
	//enable transmission interrupt
	USART1->CR1 |= USART_CR1_TXEIE;
}

//Transmission completion function. Short delay to prevent other operations happening too quickly afterwards
void finished_transmission(uint32_t bytes_sent, char *sent_string) {
	
	volatile uint32_t test = 0;
	for (volatile uint32_t i = 0; i < 0x8ffff; i++) {
		//delay
	}
}

//Receiving completion function to process data
void finished_receiving(uint8_t num_characters, char *received_string){

	//Disable processing flag whilst data being parsed
	uint8_t init_byte = received_string[0];
	processing_flag = 0;

	//Transmit completion message after task has finished
	uint8_t *finished_op = "Finished task. Send new prompt!";

	switch(received_string[0]){
		case 's':
			//do something here
			start_interrupt_transmission(&USART1_PORT, finished_op, strlen(finished_op));
			break;
		case 'l':
			//do something here
			start_interrupt_transmission(&USART1_PORT, finished_op, strlen(finished_op));
			break;
		case 't':
			//do something here
			start_interrupt_transmission(&USART1_PORT, finished_op, strlen(finished_op));
			break;
		case 'o':
			//do something here
			start_interrupt_transmission(&USART1_PORT, finished_op, strlen(finished_op));
			break;
	}

	//enable processing falg
	processing_flag = 1;
}

//Function to transmit a byte via polling method, called in SerialOutputString
void SerialOutputChar(uint8_t data, SerialPort *serial_port) {

	while((serial_port->UART->ISR & USART_ISR_TXE) == 0){
	}

	serial_port->UART->TDR = data;
}

//Function to transmit a string via polling method
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

//Double buffer interrupt handler for receiving and transmitting data via interrupt
//Sends received string to processing buffer provided message has finished being sent AND processing buffer is ready to swap
//Function triggered automatically for receiving data, trigerred manually for transmitting via start_interrupt_transmission
void USART1_EXTI25_IRQHandler(void){

	//Handling receiving interrupts
	if (USART1_PORT.UART->ISR & USART_ISR_RXNE){

		static uint8_t rx_index = 0;
		char received_byte = USART1_PORT.UART->RDR;

		// If buffer not full, store byte
		if(rx_index < RX_BUFFER_SIZE -1){
			active_buffer[rx_index] = received_byte;
			rx_index++;
		}

		// If buffer full or carriage return received:
		if (received_byte == '\r' || rx_index >= RX_BUFFER_SIZE - 1) {
			
			//Null-terminate string so string.h can be used
			active_buffer[rx_index] = '\0';  

			//Check if processing buffer is ready, and there is no data currently being transmitted
			if(processing_flag == 1 && transmitting == 0){

				//If ready, swap buffers
				volatile uint8_t * temp = active_buffer;
				active_buffer = processing_buffer;
				processing_buffer = temp;

				//Clear index, clear processing flag, clear buffer that will now receive new data
				uint8_t length = rx_index;
				rx_index = 0;
				memset((void*)active_buffer, 0, RX_BUFFER_SIZE);
				processing_flag = 0;

				//Initiate completion function for data processing
				USART1_PORT.completion_function(length, processing_buffer);
			}
			else{
				//If processing flag not ready, discared data and clear buffer
				rx_index = 0;
				memset((void*)active_buffer, 0, RX_BUFFER_SIZE);
			}
		}

	}

	//Transmission interrupt handling
	if (USART1_PORT.UART->ISR & USART_ISR_TXE){
		
		//Send data until end of string length is reached
		if (tx_index < tx_length) {
			USART1_PORT.UART->TDR = tx_buffer[tx_index+1];
			tx_index++;
		}
		//Clear all transmission flags and indexes
		else{
			tx_index = 0;
			tx_length = 0;
			transmitting = 0;
			USART1->CR1 &= ~USART_CR1_TXEIE;
		}
	}

}

/*
// Interrupt handler for Part 2.3.2c - Interrupt Handler Receiving only
//Function uses active_buffer only as double buffer implentation not accounted for
void USART1_EXTI25_IRQHandler(void){

	// Check data in RDR register
	if (USART1_PORT.UART->ISR & USART_ISR_RXNE){

		static uint8_t rx_index = 0;
		char received_byte = USART1_PORT.UART->RDR;

		// Store byte into active buffer if not filled
		if(rx_index < RX_BUFFER_SIZE -1){
			active_buffer[rx_index] = received_byte;
			rx_index++;
		}
  
  		//If buffer full, append with null termination character for processing
		else{
			active_buffer[rx_index] = '\0';
			USART1_PORT.completion_function(rx_index, active_buffer);
			rx_index = 0;
		}

		// Check if terminating character is received, if so start completion function to process
		if(received_byte == '\r'){

			USART1_PORT.completion_function(rx_index, active_buffer);
			rx_index = 0;
		}

	}
}
*/

//Function to enable USART1 interrupts
void enable_USART_interrupt() {
	
	// Disable the interrupts whilst configuring
	__disable_irq();

	//enable the USART 1 clock
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN;

	//RXNE, receiver, transmitter and USART interrupt enable
	USART1->CR1 |= USART_CR1_RXNEIE;
	USART1->CR1 |= USART_CR1_RE | USART_CR1_TE; 
	USART1->CR1 |= USART_CR1_UE; 

	// Setting NVIC module USART1 priorities
	NVIC_SetPriority(USART1_IRQn, 1);  
	NVIC_EnableIRQ(USART1_IRQn);

	// Re-enable interrupts
	__enable_irq();
}


//Function to receive a byte from the RDR (Receive data register)
void SerialInputChar(uint8_t *data, SerialPort *serial_port){

	while((serial_port->UART->ISR & USART_ISR_RXNE) == 0){
	}
	*data = (uint8_t)(serial_port->UART->RDR);
}

//Function to Receive String over selected UART Port
//Receives data until carriage return is detected
void SerialInputString(uint8_t buffer_size, SerialPort *serial_port, char *buffer){

	uint8_t buffer_counter = 0;
	uint8_t character;
	while (1){
		SerialInputChar(&character, serial_port);
		if (character == '\r'){
			break;
		}
		if (buffer_counter < buffer_size-1){
			buffer[buffer_counter] = character;
			buffer_counter++;
		}
	}
	buffer[buffer_counter] = '\0';
	serial_port->completion_function(buffer_counter, buffer);
}



