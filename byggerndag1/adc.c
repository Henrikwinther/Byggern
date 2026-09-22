/*
 * adc.c
 *
 * Created: 08.09.2026 16:00:29
 *  Author: henrikwp
 */ 


#define F_CPU 4915200UL
#include <avr/io.h>
#include <util/delay.h>
#include "adc.h"

#define ADC_BASE_ADDRESS 0x1000

void init_adc(void){

    DDRD |= (1 << PD5); // set PD5 to output

    // OC1A toggle on Compare Match
    TCCR1A &= ~(1 << COM1A1);
    TCCR1A |= (1 << COM1A0);
    
    // set CTC mode 4
    TCCR1A &= ~(1 << WGM11);
    TCCR1A &= ~(1 << WGM10);

    TCCR1B &= ~(1 << WGM13);
    TCCR1B |=  (1 << WGM12);

    TCCR1B |= (1 << CS10); // clock / 1
    TCCR1B &= ~(1 << CS11);
    TCCR1B &= ~(1 << CS12);

    OCR1A = 0; //fOC1A = F_CPU / 2;   
    
    // DDRD &= ~(1 << PD4);
}

uint8_t read_channel(uint8_t channel){
   
    volatile char *adc = (char *) ADC_BASE_ADDRESS;

    *adc = 0; // write to the ADC register to start conversion

    //while (!(PIND & (1 << PD4))); // wait for conversion to complete
	
	_delay_us(50);

    uint8_t value = 0;

     for (uint8_t i = 0; i <= channel; i++) {
        value = adc[0];
    }

    return value;

}

pos_t pos_calibrate(void){
    
    pos_t calibrated_pos = {0,0};

    uint16_t x_calibrated = 0;
    uint16_t y_calibrated = 0;

    for (uint8_t i = 0; i < 50; i++) {
		if (i > 10){
        x_calibrated += read_channel(1);
        y_calibrated += read_channel(0);
        _delay_ms(10);}
    }

    calibrated_pos.x =  x_calibrated / 39;
    calibrated_pos.y =  y_calibrated / 39;
	

    return calibrated_pos;
}


pos_t pos_read(pos_t joystick_center)
{
	pos_t pos;

	int16_t x = read_channel(1);
	int16_t y = read_channel(0);

	x -= joystick_center.x;
	y -= joystick_center.y;

	pos.x = (x * 100) / 91;
	pos.y = (y * 100) / 86;

	if (pos.x > 100)
	pos.x = 100;
	if (pos.x < -100)
	pos.x = -100;

	if (pos.y > 100)
	pos.y = 100;
	if (pos.y < -100)
	pos.y = -100;

	return pos;
}




