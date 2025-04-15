#include <string.h>
#include <stdlib.h>
#include "integration.h"
#include "dio.h"
#include "timer_module.h"
#include "serial.h"

//this is for the integration task. it modularises and seperates from
//the serial function and instead just returns the response form each function.


// this function takes a parsed command and optional arguments,
// and returns a string response to be sent over serial.
const char* handle_command(const char *cmd, const char *args) {

	if (strncmp(cmd, "serial", 7) == 0) {
		return args ? args : "";
	}
	else if (strncmp(cmd, "led", 4) == 0) {
		if (!args) return "Missing LED argument\r\n";
		int mask = binary_to_int(args); // convert binary string to int
		dio_set_led_state(mask); // update LED hardware
		return "Finished task. Send new prompt";
	}
	else if (strncmp(cmd, "oneshot", 8) == 0) {
	    if (!args) return "Missing delay argument\r\n";
	    uint32_t delay = atoi(args); // convert delay string to int
	    start_oneshot(delay, dio_invert_leds); // schedule LED inversion
	    return "Finished task. Send new prompt";
	}
	else if (strncmp(cmd, "timer", 6) == 0) {
	    if (!args) return "Missing period argument\r\n";
	    uint32_t period = atoi(args); //convert period string to int
	    dio_start_blinking();
	    timer_init(period, dio_toggle_leds);
	    return "Finished task. Send new prompt";
	}
	// no command detected
	return "Invalid command. Please try again\r\n";

}



