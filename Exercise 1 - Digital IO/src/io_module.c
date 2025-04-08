/**
 * @file : io_module.c
 * @brief : Digital I/O interface module implementation
 ******************************************************************************
 */

#include "io_module.h"
#include "stm32f303xc.h"

// Store a pointer to the function that is called when a button is pressed
// Set a default value of NULL so that it won't be called until the
// function pointer is defined
static button_callback_t on_button_press = 0x00;

// Store the current LED state internally
static uint8_t led_state = 0;

void EXTI0_IRQHandler(void) {
    // Run the button press handler (make sure it is not null first!)
    if (on_button_press != 0x00) {
        on_button_press();
    }

    // Reset the interrupt (so it doesn't keep firing until the next trigger)
    EXTI->PR |= EXTI_PR_PR0;
}

void io_init(button_callback_t callback) {
    // Enable the clocks for desired peripherals (GPIOA, C and E)
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN | RCC_AHBENR_GPIOCEN | RCC_AHBENR_GPIOEEN;
    
    // Initialize the discovery board I/O (just outputs: inputs are selected by default)
    // Get a pointer to the second half word of the MODER register (for outputs pe8-15)
    uint16_t *led_output_registers = ((uint16_t *)&(GPIOE->MODER)) + 1;
    *led_output_registers = 0x5555;

    // Set the callback function
    on_button_press = callback;

    // Initialize LED state to 1 (first LED)
    led_state = 1;
    io_set_led_state(led_state);
}

void io_chase_led(void) {
    // Shift the LED state left by one
    led_state <<= 1;

    // If we've shifted beyond the end, reset to the first LED
    if (led_state == 0) {
        led_state = 1;
    }

    // Update the physical LEDs to match our internal state
    io_set_led_state(led_state);
}

void io_enable_button_interrupt(void) {
    // Disable the interrupts while messing around with the settings
    // otherwise can lead to strange behaviour
    __disable_irq();

    // Enable the system configuration controller (SYSCFG in RCC)
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

    // External Interrupts details on large manual page 294)
    // PA0 is on interrupt EXTI0 large manual - page 250
    // EXTI0 in SYSCFG_EXTICR1 needs to be 0x00 (SYSCFG_EXTICR1_EXTI0_PA)
    SYSCFG->EXTICR[0] = SYSCFG_EXTICR1_EXTI0_PA;

    // Select EXTI0 interrupt on rising edge
    EXTI->RTSR |= EXTI_RTSR_TR0; // rising edge of EXTI line 0 (includes PA0)

    // Set the interrupt from EXTI line 0 as 'not masked' - as in, enable it.
    EXTI->IMR |= EXTI_IMR_MR0;

    // Tell the NVIC module that EXTI0 interrupts should be handled
    NVIC_SetPriority(EXTI0_IRQn, 1); // Set Priority
    NVIC_EnableIRQ(EXTI0_IRQn);

    // Re-enable all interrupts (now that we are finished)
    __enable_irq();
}

void io_set_button_callback(button_callback_t callback) {
    on_button_press = callback;
}

uint8_t io_get_led_state(void) {
    return led_state;
}

void io_set_led_state(uint8_t state) {
    led_state = state;

    // Update the physical LEDs
    uint8_t *led_register = ((uint8_t*)&(GPIOE->ODR)) + 1;
    *led_register = led_state;
}

void io_toggle_led(uint8_t led_position) {
    if (led_position < 8) {
        led_state ^= (1 << led_position);
        io_set_led_state(led_state);
    }
}

void io_turn_on_led(uint8_t led_position) {
    if (led_position < 8) {
        led_state |= (1 << led_position);
        io_set_led_state(led_state);
    }
}

void io_turn_off_led(uint8_t led_position) {
    if (led_position < 8) {
        led_state &= ~(1 << led_position);
        io_set_led_state(led_state);
    }
}
