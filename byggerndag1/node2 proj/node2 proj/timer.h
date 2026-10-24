#pragma once

#include <stdint.h>

void init_pwm(void);
void start_pwm(void);
void set_pwm_pulse(uint32_t pulse_us);
void init_interrupt(void);