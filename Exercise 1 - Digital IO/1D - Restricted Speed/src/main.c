/*
* main.c
* main program that controls leds with button input
* uses the dio module for handling leds with rate limiting
*/

#include <stdint.h>
#include "stm32f303xc.h"
#include "dio.h"

// handles what happens when button pressed
// if one half on, do other
void button_pressed(void)
{
    uint8_t current_state = dio_get_led_state();

    // check which state is currently active
    if ((current_state & 0xF0) == 0xF0) {
        // switch to state 1
        dio_set_led_state(0x0F);
    } else {
        // switch to state 2
        dio_set_led_state(0xF0);
    }
}

//the main run loop
//from here, much of the specific implementation 
int main(void)
{
    // set up and assign button pressed function
    dio_init(&button_pressed);

    // set first state
    dio_set_led_state(0x0F);

    // set change rate (2000ms)
    dio_set_led_rate(2000);

    //loop 4ever
    for(;;) {
        // interrupts mean we dont need anything in the loop
    }
}
