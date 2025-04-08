/**
 * @file : io_module.h
 * @brief : Digital I/O interface module header
 ******************************************************************************
 */

#ifndef IO_MODULE_H
#define IO_MODULE_H

#include <stdint.h>

/**
 * @brief Button callback function type definition
 */
typedef void (*button_callback_t)(void);

/**
 * @brief Initialize the Digital I/O module
 * This function initializes the GPIO for LEDs and button
 *
 * @param callback Function to be called when button is pressed, NULL for no callback
 */
void io_init(button_callback_t callback);

/**
 * @brief Chase the LED (shift pattern)
 * This function shifts the LED pattern
 */
void io_chase_led(void);

/**
 * @brief Enable button interrupt
 * This function configures and enables the button interrupt
 */
void io_enable_button_interrupt(void);

/**
 * @brief Set the button callback function
 *
 * @param callback Function to be called when button is pressed, NULL for no callback
 */
void io_set_button_callback(button_callback_t callback);

/**
 * @brief Get the current LED state
 *
 * @return uint8_t The current LED state (8-bit value representing LEDs 8-15)
 */
uint8_t io_get_led_state(void);

/**
 * @brief Set the LED state
 *
 * @param state The LED state to set (8-bit value representing LEDs 8-15)
 */
void io_set_led_state(uint8_t state);

/**
 * @brief Toggle a specific LED
 *
 * @param led_position Position of the LED to toggle (0-7 for LEDs 8-15)
 */
void io_toggle_led(uint8_t led_position);

/**
 * @brief Turn on a specific LED
 *
 * @param led_position Position of the LED to turn on (0-7 for LEDs 8-15)
 */
void io_turn_on_led(uint8_t led_position);

/**
 * @brief Turn off a specific LED
 *
 * @param led_position Position of the LED to turn off (0-7 for LEDs 8-15)
 */
void io_turn_off_led(uint8_t led_position);

#endif /* IO_MODULE_H */
