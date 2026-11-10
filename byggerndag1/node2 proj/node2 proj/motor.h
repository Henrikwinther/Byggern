#pragma once

#include <stdint.h>

void motor_init(void);
// Set signed joystick input (-100 to 100); target behavior is selected at compile time in motor.c.
void motor_set(int8_t command);
// Run the encoder-feedback position controller.
void motor_update(void);
