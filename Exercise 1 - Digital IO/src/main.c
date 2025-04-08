/**
 * @file : main.c
 * @brief : Main program body using io_module (Version 3)
 ******************************************************************************
 */

#include <stdint.h>
#include "stm32f303xc.h"
#include "io_module.h"

// Custom button handler function
void custom_button_handler(void) {
    // Get the current LED state
    uint8_t current_state = io_get_led_state();

    // Example of more complex LED behavior:
    // If we're at an odd-numbered LED, do a standard chase
    if (current_state & 0x55) {
        io_chase_led();
    }
    // If we're at an even-numbered LED, toggle the pattern between even and odd LEDs
    else {
        io_set_led_state(current_state ^ 0xFF);
    }
}

int main(void) {
    // Initialize the IO module with the button callback
    io_init(&custom_button_handler);

    // Set initial LED pattern (turn on LED 8)
    io_turn_on_led(0);

    // Enable the button interrupt
    io_enable_button_interrupt();

    /* Loop forever */
    for(;;) {}
}
