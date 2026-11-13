#pragma once

#include <stdint.h>

/* Configure, start, and update the servo PWM output. */
void init_pwm(void);
void start_pwm(void);
void set_pwm_pulse(uint32_t pulse_us);
