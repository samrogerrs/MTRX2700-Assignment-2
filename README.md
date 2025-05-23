
# MTRX2700-Assignment-2 🤖
## Group Number 3

### **Authors ✍️:**  
- **Connor Stock**: 520466054  
- **Sam Rogers**: 540723526  
- **Penelope Brown**: 530506519 

## Roles and Responsibilities 👷‍♂️

| Name            | Role                  | Responsibilities                      |  
|----------------|----------------------|--------------------------------------|  
| Connor  | Exercise 3: Timer Interface     | All of exercise 3, all timer functions in Integration |  
| Sam      | Exercise 1: Digital IO   | All of exercise 1, all LED functions in Integration |  
| Penelope  | Exercise 2: Serial Interface    | All of exercise 2, all serial communication components in Integration|  

## Project Overview 📜
This repository was made to meet requirements for the ASM Lab (Assignment 2) as part of coursework in MTRX2700 (Mechatronics 2) at the **University of Sydney**. The code is mostly in the **C** coding language and was written for, and tested on, **STM32F3 Discovery** micro-controllers.  

The code is broken into separate modules, with sub-tasks for each module, each entailing their own respective files. Descriptions of each task, and how to run the code are explained in the drop-downs in the **Module Overview** section.  

It is recommended to create a new project in **STM32CubeIDE** using the downloaded Git repository (see details below). You can then run each sub-project by navigating into its respective directory or by manually copying individual files into a new project.

### How to Download the Repo ⬇️
Clone the repository in your terminal using:
   ```bash
   git clone https://github.com/samrogerrs/MTRX2700-Assignment-2.git
   ```
Note that the repository is not publically available, for reasons pertaining to academic integrity.
## Module Overview 📂

## Requirements
### Software Requirements
- **STM32CubeIDE** (Required for building, debugging, and uploading code to the STM32F3Discovery board).
- **Serial Monitor Tool**
  - **Windows:** [PuTTY](https://www.putty.org/)
  - **MacOS:** [CuteCom](https://cutecom.sourceforge.io/)

### Hardware Requirements
- **One STM32F3 Discovery Boards**
  - All modules and integration can be completed with a single STM32F3Discovery board and a virtual connection via USB to PC or Laptop for serial communication.

## Installation & Setup
### STM32CubeIDE Installation
1. Download STM32CubeIDE from [ST’s official website](https://www.st.com/en/development-tools/stm32cubeide.html).
2. Follow the installation instructions provided on the website for your operating system.

### Serial Monitor Installation
- **Windows (PuTTY):**
  1. Download from [PuTTY’s official website](https://www.putty.org/).
  2. Install using the provided installer.
- ***PuTTY Interface Recommendations:***
  1. Open new PuTTy terminal
  2. Select "serial" under "Connection Type" and type in COM port (can be found using device manager)
  3. Type in Board Speed (ensure same as selected in board initialisation function)
  4. In Terminal Category --> Line Discipline Options select "Force On" (enables typed words to be viewed on PuTTy terminal)
  5. In Serial Category --> Flow Control select "NONE" (serial communication is asynchronous)
- **MacOS (CuteCom):**
  1. Install via Homebrew (Recommended):
     ```bash
     brew install cutecom
     ```
  2. Or download from the [CuteCom website](https://cutecom.sourceforge.io/).


<details>
<summary><strong>Exercise 1 - Digital I/O 💡</strong></summary>

<details>
<summary><strong>Task 1A</strong></summary>

#### Description
This module makes it easy to control the LEDs and respond to button presses on the STM32F303 Discovery board. When you set it up, pressing the user button automatically toggles between lighting the top half and bottom half of the board's LEDs.

#### Usage
To use this module, include the dio_init() call:

```c
#include "dio.h"

//main function - loop
int main(void)
{
    // initialise digital i/o
    dio_init();

    // loop 4ever
    for(;;) {
    }
}
```

#### Testing
To check if everything is working:

1. Load the program onto your STM32F303 Discovery board
2. When it starts running, you should see the bottom half of the LEDs light up
3. Press the blue user button on the board
4. The top half of LEDs should now light up (and the bottom half should turn off)
5. Press the button again and it should switch back


</details>

<details>
<summary><strong>Task 1B</strong></summary>

#### Description
This module makes it easy to control the LEDs and respond to button presses on the STM32F303 Discovery board. When you set it up, pressing the user button automatically toggles between lighting the top half and bottom half of the board's LEDs. You can customize the button behavior by passing your own callback function.

#### Usage
To use this module with a custom button handler:
```c
#include "dio.h"

// Define what happens when button is pressed
void button_pressed(void)
{
    dio_toggle_led_halves();
}

// Main function - loop
int main(void)
{
    // Initialize digital I/O with callback
    dio_init(&button_pressed);
    
    // Loop forever
    for(;;) {
    }
}
```

#### Testing
To check if everything is working:
1. Load the program onto your STM32F303 Discovery board
2. When it starts running, the bottom half of the LEDs should light up
3. Press the blue user button on the board
4. The top half of LEDs should now light up (and the bottom half should turn off)
5. Press the button again and it should switch back
</details>

<details>
<summary><strong>Task 1C</strong></summary>

#### Description
This module makes it easy to control the LEDs and respond to button presses on the STM32F303 Discovery board. The LED state is encapsulated within the module and can only be accessed through get/set functions. When you set it up, pressing the user button toggles between lighting the top half and bottom half of the board's LEDs.

#### Usage
To use this module with get/set functions for LED control:
```c
#include "dio.h"

// Define what happens when button is pressed
void button_pressed(void)
{
    uint8_t current_state = dio_get_led_state();
    
    // Check which half is currently lit
    if ((current_state & 0xF0) == 0xF0) {
        // Switch to bottom half
        dio_set_led_state(0x0F);
    } else {
        // Switch to top half
        dio_set_led_state(0xF0);
    }
}

// Main function - loop
int main(void)
{
    // Initialize digital I/O with callback
    dio_init(&button_pressed);
    
    // Set initial LED state (bottom half lit)
    dio_set_led_state(0x0F);
    
    // Loop forever
    for(;;) {
        // Interrupts handle all processing
    }
}
```

#### Testing
To check if everything is working:
1. Load the program onto your STM32F303 Discovery board
2. When it starts running, the bottom half of the LEDs should light up
3. Press the blue user button on the board
4. The top half of LEDs should now light up (and the bottom half should turn off)
5. Press the button again and it should switch back
</details>
<details>
<summary><strong>Task 1D</strong></summary>

#### Description
This module makes it easy to control the LEDs and respond to button presses on the STM32F303 Discovery board. It includes rate limiting functionality that prevents LEDs from changing states too quickly. The LED state is encapsulated within the module and can only be accessed through get/set functions. When you set it up, pressing the user button toggles between lighting the top half and bottom half of the board's LEDs.

#### Usage
To use this module with rate limiting:
```c
#include "dio.h"

// Define what happens when button is pressed
void button_pressed(void)
{
    uint8_t current_state = dio_get_led_state();
    
    // Check which half is currently lit
    if ((current_state & 0xF0) == 0xF0) {
        // Switch to bottom half
        dio_set_led_state(0x0F);
    } else {
        // Switch to top half
        dio_set_led_state(0xF0);
    }
}

// Main function - loop
int main(void)
{
    // Initialize digital I/O with callback
    dio_init(&button_pressed);
    
    // Set initial LED state (bottom half lit)
    dio_set_led_state(0x0F);
    
    // Set rate limit to 2 seconds
    dio_set_led_rate(2000);
    
    // Loop forever
    for(;;) {
        // Interrupts handle all processing
    }
}
```

#### Testing
To check if everything is working:
1. Load the program onto your STM32F303 Discovery board
2. When it starts running, the bottom half of the LEDs should light up
3. Press the blue user button on the board
4. The top half of LEDs should now light up (and the bottom half should turn off)
5. Press the button again and it should switch back
6. Try pressing the button rapidly - the LEDs should only change once every 2 seconds due to the rate limiting
</details>

</details>


---

<details>
<summary><strong>Exercise 2 - Serial Interface 📡</strong></summary>

<details>
<summary><strong>Task 2.3.2A</strong></summary>

#### **Description**
These functions are designed to enable serial communication over a specified UART Port to receive and transmit strings of data through polling methods. The user is able to modify buffer sizes to accomodate different applications. Before receiving or transmitting any data, the user needs to initialise the UART port through the initialisation function, and then can send and receive data as much as they want. The polling receiving function considers a carriage return ('\r') character as the terminating character in the string, so will continue receiving the data until that character is reached or until the buffer is full. The transmission polling function will send the buffer until a '\r' character is detected or until the end of the buffer is reached. 

#### **Usage**
To use the transmission and receiving polling function, the UART port must first be initialised in main:

```c
#include serial.h

int main(void) {
	SerialInitialise(<BaudRate>, &<UART_PORT>, &<selected_completion_function>);

	for(;;);
}

```
The user can select Baud Rates of: 9600, 19200, 38400, 57600, and 115200. The baud rate should be selected as "BAUD_<baud rate>" for example, "BAUD_115200". 

Once initialised, the polling transmission function can be called to send a string of data as follows: 
```c
#include serial.h

int main(void) {

	SerialInitialise(<BaudRate>, &<UART_PORT>, &<selected_completion_function>);

	//Write a string to send:
	uint8_t * string_to_send = "This is a string! \r\n";

	//Use transmission function to send it:
	SerialOutputString(string_to_send, &USART1_PORT);
}

```
A buffer must be initialised for the receiving function to store the data:
#include serial.h
```c
#include serial.h

#define BUFFER_SIZE <define size here>

int main(void) {

	SerialInitialise(<BaudRate>, &<UART_PORT>, &<selected_completion_function>);

	char buffer[BUFFER_SIZE];
	SerialInputString(BUFFER_SIZE, &USART1_PORT, buffer);

	for(;;);
}
```
### **Testing**
When SerialInputString is sent a message larger than 64 characters, it stores the first 63 characters then appends a null termination character to the end as expected. The remaining data is lost, so the user should be careful to not overflow the buffer. 

When SerialInputString is sent non-alphanumeric characters such as '#', '%', '\r' '\n', in the centre of the message, it can handle the single character cases, however, be warned it will store '\n' as '\''n'. Therefore newline, carriage return and null-termination characters should be appended on a string once received to be further processed (e.g. with string.h functions such as strlen). 

SerialInputString can handle whitespace, even empty buffer of just spaces. However, because it stops storing into the buffer once '\r' is detected, sending just \r does not store anything into the buffer. If two carriage returns are typed, it still stores nothing. It has also been tested for a long delay (thirty seconds between typing two letters) as well as a short delay (holding down a key to type the same letter very quickly); both appear to have no impact on the string being stored. However, longer delays should be tested before being used to ensure there are no timeouts or other issues encountered. 

The received string can be easily accessed by pasting the hexadecimal memory address into the STM32 interface memory browser. The full string should appear unless the inputted string exceeds 63 characters (last buffer character is reserved for carriage return). 

</details>

<details>
<summary><strong>Task 2.3.2B</strong></summary>

#### **Description**
There are two callback functions, one called after a string has been received, and one after a string has been transmitted. The transmission callback function features a short delay function, to prevent timing issues by calling another function or process too early. It takes in the pointer to the transmitted buffer as well as the number of characters. The receiving callback function takes in a pointer to the received string buffer and the number of characters. 

#### **Usage**
The callback function is initialised during SerialInitialise, enabling it to be stored in the UART struct to be easily accessed. Both SerialOutputString and SerialInputString utilise the initialised callback function.  As such, there is no need to call it in the main function. 

### **Testing**
On initialising the UART with the finished_receiving function, stepping through the debugger can demonstrate it being called in the last line of SerialInputString. Equally, the finished_transmitting function can be debugged that way to ensure the completion function is being called correctly. However, one limitation of initialising the completion function as part of the UART struct is that the SerialInputString and SerialOutputString cannot be used without re-initialising the board, as only one completion function is linked at a time. 

</details>

<details>
<summary><strong>Task 2.3.2C</strong></summary>

### **Description**
Three functions make up the interrupt module; the interrupt handler function 'USART1_EXTI25_IRQHandler', the initialisation of the USART1 interrupt function and the finished_receiving completion function as outlined in 2.3.2B. The interrupt handler, once initialised, is triggered when a byte is detected in the RDR (receive data register), tripping the RXNE flag. The STM32 then searches for the interrupt handler function under its definition within stm32f303xc.h. The data is stored in the buffer until either a carriage return character is detected, or until the buffer is full. A null terminated character is appended onto the end of the string, enabling it to be processed with string.h functions such as strlen. It then calls the completion function finished_receiving (initialised by SerialInitialise), sending through the number of characters received and a pointer to the buffer for further processing. 

### **Usage**
To use the receiving interrupt function, the UART initialisation function and the interrupt initialisation function must be called in main as follows:
```c
#include serial.h

#define BUFFER_SIZE <define size here>

int main(void) {

	SerialInitialise(<BaudRate>, &<UART_PORT>, &<selected_completion_function>);
	enable_USART_interrupt();

	for(;;);
}
```

### **Testing**
Exceeding buffer length: Stores first 63 characters only

Whitespace: Stores into buffer with no issue, as long as the whitespace does not exceed 63 bytes (if it does only 63 bytes of whitespace will be stored). 

Carriage Return Only (empty string): The carriage return does store in the buffer, as all data is stored before it is checked against the carriage return character to terminate taking in data. This can be seen by going into the memory browser and seeing the hex 0x0D character. 

Double Carriage Return: Unlike during polling, where the carriage return terminates reading from RDR until the function is called again, typing a double carriage return stores them both into the buffer. However, since the receiving buffer index is reset to zero after each string is sent, only one carriage return appears in the buffer after the two have been typed (can check the first one really does store by pausing the code and looking at the memory browser between them) as it overwrites the first. 

Delay (1 minute between letters): Stores with no issue

Quick input (holding down a key to send letters as fast as possible): Stores with no issue

Debugging:
Some typical issue that might be encountered include some data being lost (particularly end of string) or the data not being stored into the buffer correctly. For the former, check that when the null termination character is appended, it isn't overwriting the last letter of the string. If your string is over 63 bytes you can expect data to cut off there. 

If no data is being stored into the buffer, check the definition of the handler function is identical to the one defined in stm32f303xc.h. Set a breakpoint at the start of the handler to check the interrupt is being triggered. If the function definition is correct but the handler still isn't being trigger, the issue is probably with the enable_USART1_interrupt function - ensure correct priority is set and interrupt is correctly enabled. 

If data is writing into the middle of the buffer instead of the start, ensure the receiving index is set to zero following the carriage return being sent or following the buffer overflow case. If only one byte is being stored, ensure the index is incrementing after each byte is stored. If the function is missing characters, something is likely interfering with receiving; ensure nothing is being transmitted at the same time or this could lead to jumbled data. 

</details>
	
<details>
<summary><strong>Task 2.3.2D</strong></summary>

### **Description**
Transmitting Interrupt: The transmitting interrupt works differently to the receiving interrupt. Unlike when receiving data, when the RXNE flag is tripped via data being sent to the RDR register, the user must trigger the transmission interrupt by sending the first byte of data in a buffer to the Transmission Data Register (TDR) in order to trigger the interrupt and send the rest of the data in the buffer. As such, the transmission interrupt module consists of three functions: enable_USART_interrupt (used also for the receiving interrupt initialisation), USART1_EXTI25_IRQHandler (the same function called when the receiving interrupt is triggered), and start_interrupt_transmission, the function that sends the first byte of data into the TDR. start_interrupt_transmission only runs if the size of the transmitted buffer is not zero and there is not another string already transmitting. It initialises a buffer that is four bytes larger than the string to be transmitted, in order to append \r\n to either end. As such, when sent to an interface such as PuTTy, it will send on a line beneath the input message, and then begin a new line to send a new input. 

Double Buffer: The purpose of the double buffer is to prevent data being scrambled from new data being received whilst old data is still being processed or transmitted out. As the name implies, the double buffer utilises two separate buffers, an 'active' buffer for receiving and a 'processing' buffer that is being parsed or transmitted. When serial data is sent to the STM32 microcontroller, the interrupt handler function is triggered, and the data is stored into the active buffer until the buffer is full or a carriage return character is detected. Once the active buffer has completed storing the data, it checks that the processing buffer has finished being parsed or transmitted. If this is so, the buffers swap over so the newly received data can be processed whilst new serial communication can stored. If not, the buffers do not swap, and the receiving buffer is cleared in order to store newly received data until the processing buffer is ready. 

### **Usage**
The double buffer is implemented within USART1_EXTI25_IRQHandler. Buffer sizes of the active and processing buffers can be altered by the user in the serial.h file to cater to different applications. As such, there is no need to call anything in the main function apart from enabling the USART1 interrupt and initialising the USART1 port over serial communication. It is important to note that each UART has a separate interrupt initialisations. As such, if UART2 is initialised instead of USART1, USART1_EXTI25_IRQHandler will not be triggered and no data will be received or transmitted through interrupts. This means main should take care to initialise USART1:
```c
#include serial.h

#define BUFFER_SIZE <define size here>

int main(void) {

	SerialInitialise(<BaudRate>, &USART1_PORT, &finished_receiving);
	enable_USART_interrupt();

	for(;;);
}
```

### **Testing**

Buffer Overflow: Our code prevents buffer overflow rather than instigating something like a wrap-around (which might jumble the data and lead to an invalid message anyway). If buffer overflow occurs it assumes the message was invalid, and transmits back a message to send a shorter string. It then resets the buffer index and clears the buffer so that new information can be received. As such, only valid inputs will be stored and processed. 

Multiple Buffer Overlows: No information is swapped into the processing buffer, as it clears the invalid input each time and sends an automatice error message each time.  

Whitespace: Stores without issue

Sending data quickly (holding down key to type very fast): Stores without issue

Carriage return only: Stores into the buffer as expected. 

Double carriage return: Stores one into the first buffer, and one into the second as expected. The first carriage return triggers a check of the flags and then swaps the buffers. The second carriage return will repeat the process. 

Change of Buffers - do they actually swap?: 
A good way to test this is to first comment out the memset line that clears the new active buffer so it can receive data that is shorter than the old message it might have received. Pause after sending the first string - it should be in buffer1. Then pause after second string - it should be in buffer2. When a third string is sent it should overwrite the first string. 

Once memset is uncommented, you should have the effect of after the first string is sent, it stores into buffer1, and buffer2 is empty. When the next string is sent, you'll find that it is stored in buffer2 AND that the string in buffer1 has been erased, as the memset function has cleared it so it can be the new active buffer. 

Debugging:
If the received strings are not appearing in either of the buffers, it is likely an error either with the USART initialisation, or the pointer to the buffers being initialised incorrectly. Use breakpoints to check if the interrupt handler function is being triggered when data is typed into PuTTy or Cutecom; if it is, the buffers are likely the issue. If it isn't, the user should check they have the correct handler name, and that they have initialised the interrupt correctly. 

If the buffers are not swapping, check if the flags are changing values. Load their address into the memory browser and step through the code. Ensure flags are being reset to zero when operations are finished (e.g. after processing data for the processing buffer, remember to set the finished_processing flag back to 1 to indicate buffers can swap). 


</details>

</details>

---

<details>
<summary><strong>Exercise 3 - Timer Interface ⏳</strong></summary>

<details>
<summary><strong>Task 3A</strong></summary>

#### **Description**
This task implements a timer module that triggers a user-defined callback function at regular intervals using TIM2. The interval (in milliseconds) is passed during initialisation. Function pointers are used to register the callback, allowing modular and reusable design.

#### **Usage**
```c
#include "timer_module.h"

void my_callback(void) {
    // Code to run every interval
}

int main(void) {
    __enable_irq(); 
    timer_init(100, my_callback); // Trigger every 100ms

    while (1) {} // Main loop left empty – logic is interrupt-driven
}
```

### **Testing**
Confirmed correct timing by toggling LEDs every 100ms.

Used PE15 as a debug pulse to verify callback execution.

All timer setup and ISR handling occurs in timer_module.c.

</details>

<details>
<summary><strong>Task 3B</strong></summary>

#### **Description**
Adds support to dynamically modify the periodic timer’s interval using set_period(). The timer period variable is private to the module and only accessible through get_period() and set_period() to ensure encapsulation and prevent direct modification from other files.

#### **Usage**
```c 
set_period(500);    // Change blinking interval to 500ms
uint32_t p = get_period();  // Retrieve the current interval
```

### **Testing**
Verified period can be changed at runtime without restarting the program.

Ensured the timer reconfigures cleanly and continues triggering callbacks at the new interval.
</details>

<details>
<summary><strong>Task 3C</strong></summary>
  
#### **Description**
Implements a one-shot timer using TIM3. This timer triggers a specified callback after a single delay (in milliseconds), then stops. It does not repeat. The function uses a second function pointer and dedicated hardware timer to isolate one-shot logic.

#### **Usage**
```c
void delayed_action(void) {
    // Code to run once after the delay
}

start_oneshot(4000, delayed_action); // Trigger after 4 seconds
```

### **Testing**
Used to pause LED blinking for 2 seconds after an initial 4-second delay.

Confirmed callback only runs once.

Confirmed timer disables itself cleanly after execution.

</details>



</details>

---

<details>
<summary><strong>Exercise 4 - Integration 🔄</strong></summary>

#### **Description**
The code takes in input via the serial communication to perform four different operations. If an invalid input is sent, the code will respond with an error message and ask the user to try again. 

If provided with the input "serial <message>" the <message> will be transmitted back via the transmitting interrupt module onto the interface (PuTTy or Cutecom). It is designed to send the message back framed as \r\n<message>\r\n such that it will start on a new line underneath the input, and a new input can commence underneath the returned message to prevent the interface overwriting. 

If provided with the input "led <led_pattern>, the LEDs on the microcontroller will be lit up accoding to the specified pattern. For example, "led 10101010" would turn on every second LED whilst "led 11110000" would turn on the first four. 

If provided with the input "timer <number>", the LEDs will begin to flash on an off as specified by the number of milliseconds provided. For example, the input "timer 1000" will turn the current LED pattern on and off at intervals of 1000 milliseconds. 

If provided with the input "oneshot <number>", the set LED pattern currently on the microcontrollers will invert (but will not stop the flashing if a previous timer command is called). For instance, if provided with "oneshot 500" as the input, if the current LED pattern is 10101010, after 500 milliseconds it will inver to 01010101, and then continue flashing if a previous timer function has been called. If no previous timer function has been called, such that the LEDs are all off, it will simply turn them all on. 

#### **Usage**
To run the integrated code, the serial communication UART (USART1 for this code) must be initialised, alongside enabling all required interrupts for serial input, serial transmission, timers, clocks and LEDs. This can be done by calling the following functions in main:

```c
#include serial.h
#include dio.h
#include timer_module.h

int main(void) {
	//Initialise serial communication
    	SerialInitialise(BAUD_115200, &USART1_PORT, &finished_receiving);

	//enable all interrupts and configurations
	enable_USART_interrupt();
	dio_init();
	enable_timer_interrupts();

	//infinite loop to run program
	for(;;);
}
```

### **Testing**
Overflow: The same double buffer implementation was used as in Exercise 2 - Serial Interface Part 2D. As such, the error message is successfully transmitted as soon as there is a buffer overflow, and resets the active_buffer to handle new incoming information, discarding the invalid message. New valid inputs after the overflow work as normal. 

Serial input - just \r entered: stores into the buffer and is tansmittted back. That is, it will do a double line space as now there is \r\r\n being transmitted back. 

Timer before Oneshot Input:
If the timer is called before the one-shot input it works as per instructed. The current led bitmask will start flashing on and off at the input rate (ms). 

Oneshot before Timer Input:
If the Oneshot is called before the timer, it correctly inverts the current led bitmask after the input delay in ms. i.e if the pre oneshot led bitmask was 10101010, and then oneshot 1000 is typed into the serial input, after 1 second the led bitmask will invert to 010101010. 

Calling oneshot function before LED input has been set:
Calling the oneshot function before the LED function still inverts the bitmask from 00000000 to 11111111.

Calling timer function before LED input has been set:
If the led bitmask has not been set (i.e 00000000). The timer will start but just not flash. i.e it will swap between the current bitmask (00000000) and off (00000000) every input time period. The result of this is no visible blinking. If the led bitmask is subsequently set, the blinking will appear.

More than 8 numbers entered for LED call:
As long as there are no inputs other than 1s and 0s, if the first 8 led string characters are valid the remaining input is discarded (that is, the extra characters). For example, if the input is: "led 111100001111" just the first four LED lights turn on as expected, with the additional 1111 being discarded. 

Less than 8 numbers entered for LED call:
As long as the inputs are 0s and 1s, the pattern will set for the number of inputs entered. For example, if "led 001" is inputted, the first three leds will be set. The rest of the LEDs will turn off, and will not remain in their previous state.  

Different numbers than 1 and 0 in LED call:
The binary to integer function operates in a while loop until a character that is not 0 or 1 is detected. As such, if the input was, "Led a0010101" no LEDs would turn on. However, if the input is "led 1010a011" the two LEDs preceeding the non-valid character would still operate, turning on. As such, the pattern of led input is valid until a corrupting character is encountered, with the LEDs succeeding the invalid character will not be set and will return to default (off). 

Debugging:
If the serial input has a message transmitting back, but incorrectly, check first that the pointer to the string is in the correct position in the finished_receiving function. If it is, the issue will probably be through the interrupt handler, which can be confirmed by checking the memory browser to see what is stored in each buffer. Ensure rx_index is not setting to zero in strange places and is incrementing correctly. 




</details>

---




## Testing and Troubleshooting 🧪
### Project Setup ⚙️
1. Move all files from each folder in this project into your STM32CubeIDE workspace.
2. Open STM32CubeIDE and import the project files as necessary.
3. Build the project and ensure there are no errors.


### Debugging 🐞
- To debug your program in STM32CubeIDE:
  1. Click on **Debug** to start the debugging process.
  2. Once debugging starts, click **Resume** to run the program.
  3. Transmit your data via serial communication using your chosen serial monitor tool.
  4. Click **Pause** in STM32CubeIDE to inspect what was received.


- Make sure all dependencies are installed and properly configured.
- If the IDE does not detect the STM32F3Discovery board, try restarting your IDE or reconnecting the board.
- If using a MAC, the dongle connectors can be temperamental. Try unplugging and retrying
- Ensure serial connections are properly established between the boards if required

  
---

## Minutes and Meeting Notes 📁

All meeting notes and agendas are stored in the `/minutes/` directory of this repository.



