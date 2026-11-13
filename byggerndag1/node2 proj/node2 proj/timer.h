#pragma once

#include <stdint.h>

/* Encoder position access and periodic timer/interrupt setup. */
void init_encoder(void);
int32_t encoder_position(void);
void init_interrupt(void);