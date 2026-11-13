/*
 * adc.h
 *
 * Created: 08.09.2026 16:00:08
 *  Author: henrikwp
 */ 


#ifndef ADC_H_
#define ADC_H_

#include <stdint.h>


/* Two-axis joystick coordinates. */
typedef struct {
    int16_t x;
    int16_t y;
} pos_t;

/* Direction inferred from the joystick position. */
typedef enum {
	LEFT,
	RIGHT,
	UP,
	DOWN,
	NEUTRAL 
	}JoystickDirection;

/* ADC setup, channel reads, joystick calibration, and input interpretation. */
void init_adc(void);
uint8_t read_channel(uint8_t channel);
pos_t pos_calibrate(void);
pos_t pos_read(pos_t joystick_center);
JoystickDirection joystick_direction(pos_t joystick_center);



#endif /* ADC_H_ */