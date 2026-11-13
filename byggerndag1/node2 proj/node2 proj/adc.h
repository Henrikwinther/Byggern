#pragma once

#include <stdint.h>

/* Score shared with the timer interrupt and application loop. */
extern volatile uint8_t score;

/* ADC initialization, sensor reading, and score update functions. */
void init_adc(void);
uint16_t adc_read(void);
void set_score(void);
