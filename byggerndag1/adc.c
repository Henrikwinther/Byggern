/*
 * adc.c
 *
 * Created: 08.09.2026 16:00:29
 *  Author: henrikwp
 */ 


#define F_CPU 4915200UL
#include <avr/io.h>
#include <util/delay.h>

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
}
