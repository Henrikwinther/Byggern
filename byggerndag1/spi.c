/*
 * spi.c
 *
 * Created: 15.09.2026 10:55:20
 *  Author: alexanmf
 */ 

#define F_CPU 4915200UL
#include <avr/io.h>
#include <util/delay.h>
#include "spi.h"

void spi_init(void)
{
	
	// set SCK, MOSI and SS as output
	DDRB |= (1 << PB7) | (1 << PB5) | (1 << PB4) | (1 << PB3)  | (1 << PB1);
	
	// set SS to 1
	PORTB |= (1 << PB4) | (1 << PB3)  | (1 << PB1);

	//set MISO as input
	DDRB &= ~(1 << PB6);
	
	// display d/c as output
	DDRB |= (1 << PB2);
	
	/* Enable SPI, Master, set clock rate fck/16 */
	SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0);
	
}

void spi_select(spi_slave_t slave){
	
	if (slave == 0) {
		PORTB &= ~(1 << PB4);
	}
	
	if (slave == 1) {
		PORTB &= ~(1 << PB3);
	}

	if (slave == 2) {
		PORTB &= ~(1 << PB1);
	}
}


void spi_deselect(void) {
	PORTB |= (1 << PB4);
	PORTB |= (1 << PB3);
	PORTB |= (1 << PB1);
}

void spi_write(uint8_t data)
{
	/* Start transmission */
	SPDR = data;
	/* Wait for transmission complete */
	while(!(SPSR & (1<<SPIF)))
	;

	(void)SPDR;
}

uint8_t spi_read(void) {
	
	SPDR = 0x77; // send dummy data
	
	/* Wait for transmission complete */
	while(!(SPSR & (1<<SPIF)))
	;
	
	// retrieve what the slave sent
	return SPDR;
}

