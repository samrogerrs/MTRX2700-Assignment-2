
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
When SerialInputString is sent a message larger than 64 characters, it stores the first 63 characters then appends a null termination character to the end as expected. The remaining data is lost.

When SerialInputString is sent non-alphanumeric characters such as '#', '%', '\r' '\n', in the centre of the message, it can handle the single character cases, however, be warned it will store '\n' as '\''n' 
</details>
<details>
<summary><strong>Task 2B</strong></summary>

#### **Description**
Insert description

#### **Usage**
Insert how to use

### **Testing**
Insert how module was tested

</details>

<details>
<summary><strong>Task 2C</strong></summary>

#### **Description**
Insert description

#### **Usage**
Insert how to use

### **Testing**
Insert how module was tested

</details>

<details>
<summary><strong>Task 2D</strong></summary>

#### **Description**
Insert description

#### **Usage**
Insert how to use

### **Testing**
Insert how module was tested

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
Insert description

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



