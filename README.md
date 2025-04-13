
# MTRX2700-Assignment-2 🤖
## Group Number 3 - NAME

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
}
```

### **Testing**
Exceeding buffer length: Stores first 63 characters only

Whitespace: Stores into buffer with no issue, as long as the whitespace does not exceed 63 bytes (if it does only 63 bytes of whitespace will be stored). 

Carriage Return Only (empty string): The carriage return does store in the buffer, as all data is stored before it is checked against the carriage return character to terminate taking in data. This can be seen by going into the memory browser and seeing the hex 0x0D character. 

Double Carriage Return: Unlike during polling, where the carriage return terminates reading from RDR until the function is called again, typing a double carriage return stores them both into the buffer. However, since the receiving buffer index is reset to zero after each string is sent, only one carriage return appears in the buffer after the two have been typed (can check the first one really does store by pausing the code and looking at the memory browser between them). 

Delay (1 minute between letters): Stores with no issue

Quick input (holding down a key to send letters as fast as possible): Stores with no issue

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
}
```

### **Testing**
Whitespace: Stores without issue

Sending data quickly (holding down key to type very fast): Stores without issue

Carriage return only: Stores into the buffer as expected. 

Double carriage return: Stores one into the first buffer, and one into the second as expected. The first carriage return triggers a check of the flags and then swaps the buffers. The second carriage return will repeat the process. 

Change of Buffers - do they actually swap?: Buffers appear to swap when both are ready. This can be checked by loading the memory browser and looking at buffer1 and buffer2. The first input string stores in buffer1, the second stores in buffer2. The third string overwrites the first and stores in buffer1 again as intended. 

Debugging:
If the received strings are not appearing in either of the buffers, it is likely an error either with the USART initialisation, or the pointer to the buffers being initialised incorrectly. Use breakpoints to check if the interrupt handler function is being triggered when data is typed into PuTTy or Cutecom; if it is, the buffers are likely the issue. If it isn't, the user should check they have the correct handler name, and that they have initialised the interrupt correctly. 

If the buffers are not swapping, check if the flags are changing values. Load their address into the memory browser and step through the code. Ensure flags are being reset to zero when operations are finished (e.g. after processing data for the processing buffer, remember to set the finished_processing flag back to 1 to indicate buffers can swap). 


</details>

<details>

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


#### **Usage**
Insert how to use

### **Testing**
Insert how module was tested


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



