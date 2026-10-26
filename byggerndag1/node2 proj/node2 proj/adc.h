#pragma once

#include <stdint.h>

extern volatile uint8_t score;

void init_adc(void);
uint16_t adc_read(void);
void set_score(void);
