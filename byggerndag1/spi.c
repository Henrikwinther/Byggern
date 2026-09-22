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

typedef struct {
	volatile uint8_t *port;
	volatile uint8_t *ddr;
	uint8_t bit;
} cs_pin_t;


static const cs_pin_t cs_pins[SPI_NUM_SLAVES] = {
	[IO_BOARD_MCU] = {&PORTB, &DDRB, PB4},
	[OLED_DISPLAY] = {&PORTB, &DDRB, PB3}
};

// void spi_init(void)
// {
// 	for(uint8_t i = 0; i < SPI_NUM_SLAVES; i++) {
// 		*cs_pins->port |= (1<< cs_pins[i].bit);
// 		*cs_pins->ddr |= (1<< cs_pins[i].bit);
// 	}
// 	
// 	// set SCK, MOSI and SS as output
// 	 DDRB |= (1 << PB7) | (1 << PB5) | (1 << PB4) ;
// 	 
// 	 //set MISO as input
// 	 DDRB &= ~(1 << PD6);
// 	 
// 	 /* Enable SPI, Master, set clock rate fck/16 */
// 	SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0);
// 	
// }

void spi_init(void)
{
	// set SCK, MOSI and SS as output
	DDRB |= (1 << PB7) | (1 << PB5) | (1 << PB4) | (1 << PB3)  | (1 << PB1)  ;
	
	//set MISO as input
	DDRB &= ~(1 << PD6);
	
	// display d/c as output
	DDRB |= (1 << PB2);
	
	/* Enable SPI, Master, set clock rate fck/16 */
	SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0);
	
}

void spi_select(spi_slave_t slave){
	
	if (slave == 0) {
		PORTB &= ~(1 << PD4);
	}
	
	if (slave == 1) {
		PORTB &= ~(1 << PD3);
	}
}


void spi_deselect(void) {
	PORTB |= (1 << PB4);
	PORTB |= (1 << PB3);
}


// void spi_select(spi_slave_t slave) {
// 	spi_deselect();
// 	if(slave < SPI_NUM_SLAVES) {
// 		*cs_pins[slave].port &= ~(1 << cs_pins[slave].bit);
// 	};
// }
// 
// void spi_deselect(void) {
// 	for(uint8_t i = 0; i < SPI_NUM_SLAVES; i++) {
// 		*cs_pins[i].port |= (1 << cs_pins[i].bit);
// 	};
// }

uint8_t spi_transfer(uint8_t out) {
	SPDR = out;
	while(!(SPSR & (1 << SPIF)));
	return SPDR;
}

void spi_write_n(const uint8_t *buf, uint16_t n) {
	while(n--) spi_transfer(*buf++);
}


void spi_write(char cData)
{
	/* Start transmission */
	SPDR = cData;
	/* Wait for transmission complete */
	while(!(SPSR & (1<<SPIF)))
	;
}

uint8_t spi_read(void) {
	
	SPDR = 0xFF; // send dummy data
	
	/* Wait for transmission complete */
	while(!(SPSR & (1<<SPIF)))
	;
	
	// retrieve what the slave sent
	return SPDR;
}

